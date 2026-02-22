
import multer from 'multer';
import { createServer as createViteServer } from 'vite';
import fs from 'fs';
import path from 'path';
import { fileURLToPath } from 'url';

const __dirname = path.dirname(fileURLToPath(import.meta.url));

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

      try {
        console.log(`Processing file: ${originalName} at ${filePath}`);
        await new Promise((resolve) => setTimeout(resolve, 2000));

        let report = '';
        if (originalName.endsWith('.cpp') || originalName.endsWith('.h')) {
          report = `STYLE CHECK REPORT FOR: ${originalName}\n`;
          report += `----------------------------------------\n`;
          report += `[ERROR] Line 12: Function name 'my_function' should be camelCase 'myFunction'.\n`;
          report += `[WARNING] Line 45: Line length exceeds 80 characters (85 chars).\n`;
          report += `[ERROR] Line 88: Missing function contract for 'calculateTotal'.\n`;
          report += `[INFO] Line 102: Good use of constants.\n`;
          report += `\nTotal Errors: 2\nTotal Warnings: 1\n`;
        } else {
          report = `STYLE CHECK REPORT FOR: ${originalName}\n`;
          report += `----------------------------------------\n`;
          report += `[INFO] File type not strictly checked by mock engine, but here is a sample output.\n`;
          report += `[WARNING] Line 1: Header comment missing.\n`;
        }

        fs.unlink(filePath, (err) => {
          if (err) console.error('Error deleting temp file:', err);
        });

        res.json({ report });
      } catch (error) {
        console.error('Processing error:', error);
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

startServer();
