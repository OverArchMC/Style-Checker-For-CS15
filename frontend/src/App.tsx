import React, { useState } from 'react';
import { Layout } from './components/Layout';
import { DropZone } from './components/DropZone';
import { Spinner } from './components/Spinner';
import { Results } from './components/Results';
import { AnimatePresence, motion } from 'motion/react';

type AppState = 'idle' | 'uploading' | 'processing' | 'complete' | 'error';

export default function App() {
  const [state, setState] = useState<AppState>('idle');
  const [file, setFile] = useState<File | null>(null);
  const [report, setReport] = useState<string>('');
  const [error, setError] = useState<string>('');

  const handleFileSelect = async (selectedFile: File) => {
    setFile(selectedFile);
    setState('processing'); // Skip 'uploading' state for smoother UX, or use it if we want a progress bar

    const formData = new FormData();
    formData.append('file', selectedFile);

    try {
      const response = await fetch('/api/upload', {
        method: 'POST',
        body: formData,
      });

      if (!response.ok) {
        const rawError = await response.text();
        let serverMessage = `Upload failed: ${response.status} ${response.statusText}`;

        try {
          const parsed = JSON.parse(rawError);
          if (parsed?.error && typeof parsed.error === 'string') {
            serverMessage = parsed.error;
          }
        } catch {
          if (rawError.trim().length > 0) {
            serverMessage = rawError;
          }
        }

        console.error('Server Error:', serverMessage);
        throw new Error(serverMessage);
      }

      const data = await response.json();
      setReport(data.report);
      setState('complete');
    } catch (err) {
      console.error(err);
      if (err instanceof Error && err.message.trim().length > 0) {
        setError(err.message);
      } else {
        setError('Failed to process file. Please try again.');
      }
      setState('error');
    }
  };

  const handleReset = () => {
    setFile(null);
    setReport('');
    setError('');
    setState('idle');
  };

  return (
    <Layout>
      <AnimatePresence mode="wait">
        {state === 'idle' && (
          <motion.div 
            key="idle"
            exit={{ opacity: 0, y: -20 }}
            className="w-full"
          >
            <DropZone onFileSelect={handleFileSelect} />
          </motion.div>
        )}

        {(state === 'processing' || state === 'uploading') && (
          <motion.div 
            key="processing"
            initial={{ opacity: 0 }}
            animate={{ opacity: 1 }}
            exit={{ opacity: 0 }}
          >
            <Spinner />
          </motion.div>
        )}

        {state === 'complete' && (
          <motion.div 
            key="complete"
            initial={{ opacity: 0 }}
            animate={{ opacity: 1 }}
            className="w-full"
          >
            <Results 
              report={report} 
              fileName={file?.name || 'unknown_file'} 
              onReset={handleReset} 
            />
          </motion.div>
        )}

        {state === 'error' && (
          <motion.div 
            key="error"
            initial={{ opacity: 0 }}
            animate={{ opacity: 1 }}
            className="text-center space-y-4"
          >
            <div className="text-red-400 text-xl font-bold">Error</div>
            <p className="text-white/60">{error}</p>
            <button 
              onClick={handleReset}
              className="px-6 py-2 bg-white/10 hover:bg-white/20 rounded-lg transition-colors"
            >
              Try Again
            </button>
          </motion.div>
        )}
      </AnimatePresence>
    </Layout>
  );
}
  );
}
