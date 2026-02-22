import express from 'express';import express from 'express';







































































































startServer();}  });    console.log(`Server running on http://localhost:${PORT}`);  app.listen(PORT, '0.0.0.0', () => {  }    });      res.sendFile(path.join(distDir, 'index.html'));      }        return res.status(404).json({ error: 'Not found' });      if (req.path.startsWith('/api/')) {    app.get('*', (req, res) => {    app.use(express.static(distDir));    const distDir = path.join(__dirname, 'dist');  } else {    app.use(vite.middlewares);    });      appType: 'spa',      server: { middlewareMode: true },    const vite = await createViteServer({  if (process.env.NODE_ENV !== 'production') {  );    }      }        res.status(500).json({ error: 'Failed to process file' });        console.error('Processing error:', error);      } catch (error) {        res.json({ report });        });          if (err) console.error('Error deleting temp file:', err);        fs.unlink(filePath, (err) => {        }          report += `[WARNING] Line 1: Header comment missing.\n`;          report += `[INFO] File type not strictly checked by mock engine, but here is a sample output.\n`;          report += `----------------------------------------\n`;          report = `STYLE CHECK REPORT FOR: ${originalName}\n`;        } else {          report += `\nTotal Errors: 2\nTotal Warnings: 1\n`;          report += `[INFO] Line 102: Good use of constants.\n`;          report += `[ERROR] Line 88: Missing function contract for 'calculateTotal'.\n`;          report += `[WARNING] Line 45: Line length exceeds 80 characters (85 chars).\n`;          report += `[ERROR] Line 12: Function name 'my_function' should be camelCase 'myFunction'.\n`;          report += `----------------------------------------\n`;          report = `STYLE CHECK REPORT FOR: ${originalName}\n`;        if (originalName.endsWith('.cpp') || originalName.endsWith('.h')) {        let report = '';        await new Promise((resolve) => setTimeout(resolve, 2000));        console.log(`Processing file: ${originalName} at ${filePath}`);      try {      const originalName = req.file.originalname;      const filePath = req.file.path;      }        return res.status(400).json({ error: 'No file uploaded' });      if (!req.file) {    async (req, res) => {    },      });        next();        }          return res.status(500).json({ error: 'File upload failed' });          console.error('Multer error:', err);        if (err) {      upload.single('file')(req, res, (err) => {    (req, res, next) => {    '/api/upload',  app.post(  });    res.json({ status: 'ok' });  app.get('/api/health', (req, res) => {  });    next();    console.log(`${req.method} ${req.url}`);  app.use((req, res, next) => {  const upload = multer({ dest: uploadDir });  }    fs.mkdirSync(uploadDir);  if (!fs.existsSync(uploadDir)) {  const uploadDir = path.join(__dirname, 'uploads');  const PORT = Number(process.env.PORT) || 3000;  const app = express();async function startServer() {const __dirname = path.dirname(fileURLToPath(import.meta.url));import { fileURLToPath } from 'url';import path from 'path';import fs from 'fs';import { createServer as createViteServer } from 'vite';import multer from 'multer';import multer from 'multer';
import { createServer as createViteServer } from 'vite';
import fs from 'fs';
import path from 'path';
import { fileURLToPath } from 'url';

const __dirname = path.dirname(fileURLToPath(import.meta.url));

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
