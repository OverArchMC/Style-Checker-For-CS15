import express from 'express';
import multer from 'multer';
import { createServer as createViteServer } from 'vite';
import fs from 'fs';
import path from 'path';
import { fileURLToPath } from 'url';
import { execFile } from 'child_process';
import { promisify } from 'util';

const __dirname = path.dirname(fileURLToPath(import.meta.url));
const execFileAsync = promisify(execFile);

async function startServer() {
  const app = express();
  const PORT = Number(process.env.PORT) || 3000;

  const uploadDir = path.join(__dirname, 'uploads');
  if (!fs.existsSync(uploadDir)) {
    fs.mkdirSync(uploadDir);
  }

  const upload = multer({ dest: uploadDir });

  app.use((req, res, next) => {
    console.log(`${req.method} ${req.url}`);
    next();
  });

  app.get('/api/health', (req, res) => {
    res.json({ status: 'ok' });
  });

  app.post(
    '/api/upload',
    (req, res, next) => {
      upload.single('file')(req, res, (err) => {
        if (err) {
          console.error('Multer error:', err);
          return res.status(500).json({ error: 'File upload failed' });
        }
        next();
      });
    },
    async (req, res) => {
      if (!req.file) {
        return res.status(400).json({ error: 'No file uploaded' });
      }

      const filePath = req.file.path;
      const originalName = req.file.originalname;
      const reportPath = path.join(uploadDir, `${path.basename(originalName)}.${Date.now()}.report.txt`);
      const checkerPath = process.env.CHECKER_PATH || path.join(__dirname, '..', 'backend', 'checker');

      try {
        if (!fs.existsSync(checkerPath)) {
          return res.status(500).json({
            error: 'Style checker binary not found. Build backend/checker or set CHECKER_PATH.',
          });
        }

        console.log(`Running checker for ${originalName}`);
        const { stderr } = await execFileAsync(checkerPath, [filePath, reportPath]);
        if (stderr && stderr.trim().length > 0) {
          console.error('Checker stderr:', stderr);
        }

        const report = await fs.promises.readFile(reportPath, 'utf8');

        fs.unlink(filePath, (err) => {
          if (err) console.error('Error deleting temp file:', err);
        });
        fs.unlink(reportPath, (err) => {
          if (err) console.error('Error deleting report file:', err);
        });

        res.json({ report });
      } catch (error) {
        console.error('Processing error:', error);
        fs.unlink(filePath, () => {});
        fs.unlink(reportPath, () => {});
        res.status(500).json({ error: 'Failed to process file' });
      }
    }
  );

  if (process.env.NODE_ENV !== 'production') {
    const vite = await createViteServer({
      server: { middlewareMode: true },
      appType: 'spa',
    });
    app.use(vite.middlewares);
  } else {
    const distDir = path.join(__dirname, 'dist');
    app.use(express.static(distDir));
    app.get('*', (req, res) => {
      if (req.path.startsWith('/api/')) {
        return res.status(404).json({ error: 'Not found' });
      }
      res.sendFile(path.join(distDir, 'index.html'));
    });
  }

  app.listen(PORT, '0.0.0.0', () => {
    console.log(`Server running on http://localhost:${PORT}`);
  });
}

startServer().catch((error) => {
  console.error('Server startup failed:', error);
  process.exit(1);
});
