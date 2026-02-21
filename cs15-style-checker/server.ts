import express from 'express';
import multer from 'multer';
import { createServer as createViteServer } from 'vite';
import fs from 'fs';
import path from 'path';
import { fileURLToPath } from 'url';

const __dirname = path.dirname(fileURLToPath(import.meta.url));

async function startServer() {
  const app = express();
  const PORT = 3000;

  // Configure multer for file uploads
  const upload = multer({ dest: 'uploads/' });

  // Ensure uploads directory exists
  if (!fs.existsSync('uploads')) {
    fs.mkdirSync('uploads');
  }

  // API Routes
  app.get('/api/health', (req, res) => {
    res.json({ status: 'ok' });
  });

  app.post('/api/upload', upload.single('file'), async (req, res) => {
    if (!req.file) {
      return res.status(400).json({ error: 'No file uploaded' });
    }

    const filePath = req.file.path;
    const originalName = req.file.originalname;

    try {
      // MOCK: Simulate C++ executable processing
      // In production, replace this with:
      // const { exec } = await import('child_process');
      // exec(`./style_checker "${filePath}"`, (error, stdout, stderr) => { ... });
      
      console.log(`Processing file: ${originalName} at ${filePath}`);
      
      // Simulate processing delay
      await new Promise(resolve => setTimeout(resolve, 2000));

      // Generate a mock report based on the file extension
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

      // Clean up the uploaded file
      fs.unlink(filePath, (err) => {
        if (err) console.error('Error deleting temp file:', err);
      });

      res.json({ report });

    } catch (error) {
      console.error('Processing error:', error);
      res.status(500).json({ error: 'Failed to process file' });
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
  }

  app.listen(PORT, '0.0.0.0', () => {
    console.log(`Server running on http://localhost:${PORT}`);
  });
}

startServer();
