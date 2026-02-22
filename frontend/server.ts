import express from 'express';
import multer from 'multer';
import { createServer as createViteServer } from 'vite';
import fs from 'fs';
import path from 'path';
import { fileURLToPath } from 'url';
import { exec } from 'child_process';
import { promisify } from 'util';

const execAsync = promisify(exec);

const __dirname = path.dirname(fileURLToPath(import.meta.url));

function resolveBackendExecutable(): string | null {
  const backendDir = path.join(__dirname, '..', 'backend');
  const candidates = [
    path.join(backendDir, 'checker.exe'),
    path.join(backendDir, 'checker'),
  ];

  for (const candidate of candidates) {
    if (fs.existsSync(candidate)) {
      return candidate;
    }
  }

  return null;
}

function resolveRuntimePathEntries(): string[] {
  const entries: string[] = [];

  const localAppData = process.env.LOCALAPPDATA;
  if (localAppData) {
    const winlibsBin = path.join(
      localAppData,
      'Microsoft',
      'WinGet',
      'Packages',
      'BrechtSanders.WinLibs.POSIX.UCRT.LLVM_Microsoft.Winget.Source_8wekyb3d8bbwe',
      'mingw64',
      'bin'
    );

    if (fs.existsSync(winlibsBin)) {
      entries.push(winlibsBin);
    }
  }

  return entries;
}

async function startServer() {
  const app = express();
  const PORT = Number(process.env.PORT) || 3000;

  // Ensure uploads directory exists
  const uploadDir = path.join(__dirname, 'uploads');
  if (!fs.existsSync(uploadDir)) {
    fs.mkdirSync(uploadDir);
  }

  // Configure multer
  const upload = multer({ dest: uploadDir });

  // Request logging middleware
  app.use((req, res, next) => {
    console.log(`${req.method} ${req.url}`);
    next();
  });

  app.use((req, res, next) => {
    res.header('Access-Control-Allow-Origin', '*');
    res.header('Access-Control-Allow-Methods', 'GET,POST,OPTIONS');
    res.header('Access-Control-Allow-Headers', 'Content-Type');
    if (req.method === 'OPTIONS') {
      return res.sendStatus(204);
    }
    next();
  });

  // API Routes
  app.get('/api/health', (req, res) => {
    res.json({ status: 'ok' });
  });

  app.post('/api/upload', (req, res, next) => {
    upload.single('file')(req, res, (err) => {
      if (err) {
        console.error('Multer error:', err);
        return res.status(500).json({ error: 'File upload failed' });
      }
      next();
    });
  }, async (req, res) => {
    if (!req.file) {
      return res.status(400).json({ error: 'No file uploaded' });
    }

    const filePath = req.file.path;
    const originalName = req.file.originalname;

    try {
      console.log(`Processing file: ${originalName} at ${filePath}`);
      
      const backendPath = resolveBackendExecutable();
      if (!backendPath) {
        fs.unlink(filePath, () => {});
        return res.status(500).json({
          error: 'Backend executable not found. Build backend/checker.exe first.',
        });
      }

      const outputPath = path.join(uploadDir, `output_${Date.now()}.txt`);
      
      // Execute the C++ style checker
      const command = `"${backendPath}" "${filePath}" "${outputPath}"`;
      console.log(`Executing: ${command}`);

      const runtimePaths = resolveRuntimePathEntries();
      const mergedPath = [
        ...runtimePaths,
        process.env.PATH ?? '',
      ].filter(Boolean).join(path.delimiter);
      
      const { stderr } = await execAsync(command, {
        env: {
          ...process.env,
          PATH: mergedPath,
        },
      });
      if (stderr?.trim().length) {
        console.warn('Checker stderr:', stderr);
      }

      if (!fs.existsSync(outputPath)) {
        throw new Error('Checker finished without generating an output file.');
      }
      
      // Read the output file
      const report = fs.readFileSync(outputPath, 'utf-8');
      
      // Clean up the uploaded file and output file
      fs.unlink(filePath, (err) => {
        if (err) console.error('Error deleting temp file:', err);
      });
      fs.unlink(outputPath, (err) => {
        if (err) console.error('Error deleting output file:', err);
      });

      res.json({ report });

    } catch (error) {
      fs.unlink(filePath, () => {});
      console.error('Processing error:', error);
      const message = error instanceof Error ? error.message : 'Failed to process file';
      res.status(500).json({ error: message });
    }
  });

  // Vite middleware for development
  if (process.env.NODE_ENV !== 'production') {
    const vite = await createViteServer({
      server: { middlewareMode: true },
      appType: 'spa',
    });
    app.use(vite.middlewares);
  } else {
    // Production static file serving (if built)
    app.use(express.static(path.join(__dirname, 'dist')));

    app.get('*', (req, res, next) => {
      if (req.path.startsWith('/api')) {
        return next();
      }
      return res.sendFile(path.join(__dirname, 'dist', 'index.html'));
    });
  }

  app.listen(PORT, '0.0.0.0', () => {
    console.log(`Server running on http://localhost:${PORT}`);
  });
}

startServer();
