import React, { useCallback, useState } from 'react';
import { Upload, FileCode, AlertCircle } from 'lucide-react';
import { motion } from 'motion/react';

interface DropZoneProps {
  onFileSelect: (file: File) => void;
}

export function DropZone({ onFileSelect }: DropZoneProps) {
  const [isDragging, setIsDragging] = useState(false);
  const [error, setError] = useState<string | null>(null);

  const handleDragOver = useCallback((e: React.DragEvent) => {
    e.preventDefault();
    setIsDragging(true);
  }, []);

  const handleDragLeave = useCallback((e: React.DragEvent) => {
    e.preventDefault();
    setIsDragging(false);
  }, []);

  const handleDrop = useCallback((e: React.DragEvent) => {
    e.preventDefault();
    setIsDragging(false);
    setError(null);

    const files = e.dataTransfer.files;
    if (files.length > 1) {
      setError("Please upload only one file at a time.");
      return;
    }
    
    const file = files[0];
    if (file) {
      // Optional: Check file extension
      // const validExtensions = ['.cpp', '.h', '.txt'];
      // if (!validExtensions.some(ext => file.name.endsWith(ext))) {
      //   setError("Invalid file type. Please upload .cpp, .h, or .txt");
      //   return;
      // }
      onFileSelect(file);
    }
  }, [onFileSelect]);

  const handleFileInput = useCallback((e: React.ChangeEvent<HTMLInputElement>) => {
    setError(null);
    const files = e.target.files;
    if (files && files.length > 0) {
      onFileSelect(files[0]);
    }
  }, [onFileSelect]);

  return (
    <motion.div 
      initial={{ opacity: 0, y: 20 }}
      animate={{ opacity: 1, y: 0 }}
      className="w-full"
    >
      <div
        className={`
          relative group cursor-pointer
          w-full aspect-[3/2] max-h-[400px]
          rounded-3xl border-2 border-dashed
          flex flex-col items-center justify-center
          transition-all duration-300 ease-out
          ${isDragging 
            ? 'border-[var(--color-tufts-blue)] bg-[var(--color-tufts-blue)]/10 scale-[1.02]' 
            : 'border-white/10 hover:border-[var(--color-tufts-blue)]/50 hover:bg-white/5'
          }
        `}
        onDragOver={handleDragOver}
        onDragLeave={handleDragLeave}
        onDrop={handleDrop}
        onClick={() => document.getElementById('file-input')?.click()}
      >
        <input
          type="file"
          id="file-input"
          className="hidden"
          onChange={handleFileInput}
          accept=".cpp,.h,.txt"
        />
        
        <div className="flex flex-col items-center gap-6 p-8 text-center">
          <div className={`
            w-20 h-20 rounded-2xl flex items-center justify-center
            bg-gradient-to-br from-[var(--color-tufts-blue)] to-blue-600
            shadow-lg shadow-blue-500/20
            transition-transform duration-300 group-hover:scale-110 group-hover:rotate-3
          `}>
            <Upload className="w-10 h-10 text-white" />
          </div>
          
          <div className="space-y-4">
            <h2 className="text-2xl font-bold tracking-tight">
              Upload your code
            </h2>
            <p className="text-white/50 max-w-xs mx-auto">
              Drag and drop your .cpp, .h, or .txt file here
            </p>
            <button 
              type="button"
              className="px-6 py-2 bg-white/10 hover:bg-white/20 rounded-full text-sm font-medium transition-colors border border-white/10"
              onClick={(e) => {
                e.stopPropagation();
                document.getElementById('file-input')?.click();
              }}
            >
              Browse Files
            </button>
          </div>
        </div>

        {/* Corner Accents */}
        <div className="absolute top-4 left-4 w-4 h-4 border-t-2 border-l-2 border-white/20 rounded-tl-lg" />
        <div className="absolute top-4 right-4 w-4 h-4 border-t-2 border-r-2 border-white/20 rounded-tr-lg" />
        <div className="absolute bottom-4 left-4 w-4 h-4 border-b-2 border-l-2 border-white/20 rounded-bl-lg" />
        <div className="absolute bottom-4 right-4 w-4 h-4 border-b-2 border-r-2 border-white/20 rounded-br-lg" />
      </div>

      {error && (
        <motion.div 
          initial={{ opacity: 0, y: 10 }}
          animate={{ opacity: 1, y: 0 }}
          className="mt-4 p-4 bg-red-500/10 border border-red-500/20 rounded-xl flex items-center gap-3 text-red-400"
        >
          <AlertCircle className="w-5 h-5" />
          <span>{error}</span>
        </motion.div>
      )}
    </motion.div>
  );
}
