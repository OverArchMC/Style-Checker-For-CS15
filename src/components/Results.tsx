import React from 'react';
import { Download, RefreshCw, CheckCircle, AlertTriangle, XCircle } from 'lucide-react';
import { motion } from 'motion/react';

interface ResultsProps {
  report: string;
  fileName: string;
  onReset: () => void;
}

export function Results({ report, fileName, onReset }: ResultsProps) {
  const handleDownload = () => {
    const blob = new Blob([report], { type: 'text/plain' });
    const url = URL.createObjectURL(blob);
    const a = document.createElement('a');
    a.href = url;
    a.download = `${fileName}_style_report.txt`;
    document.body.appendChild(a);
    a.click();
    document.body.removeChild(a);
    URL.revokeObjectURL(url);
  };

  // Simple parsing to colorize the output for display
  const renderLine = (line: string, index: number) => {
    if (line.includes('[ERROR]')) {
      return <div key={index} className="text-red-400 font-mono py-1">{line}</div>;
    }
    if (line.includes('[WARNING]')) {
      return <div key={index} className="text-yellow-400 font-mono py-1">{line}</div>;
    }
    if (line.includes('[INFO]')) {
      return <div key={index} className="text-blue-400 font-mono py-1">{line}</div>;
    }
    if (line.startsWith('---') || line.startsWith('STYLE CHECK')) {
      return <div key={index} className="text-white/50 font-bold py-1">{line}</div>;
    }
    return <div key={index} className="text-white/80 font-mono py-1">{line}</div>;
  };

  return (
    <motion.div 
      initial={{ opacity: 0, scale: 0.95 }}
      animate={{ opacity: 1, scale: 1 }}
      className="w-full max-w-4xl flex flex-col gap-6"
    >
      <div className="flex items-center justify-between">
        <div>
          <h2 className="text-2xl font-bold">Analysis Complete</h2>
          <p className="text-white/50 text-sm">Report for <span className="text-white">{fileName}</span></p>
        </div>
        <div className="flex gap-3">
          <button
            onClick={onReset}
            className="px-4 py-2 rounded-lg bg-white/5 hover:bg-white/10 border border-white/10 text-sm font-medium transition-colors flex items-center gap-2"
          >
            <RefreshCw className="w-4 h-4" />
            Check Another
          </button>
          <button
            onClick={handleDownload}
            className="px-4 py-2 rounded-lg bg-[var(--color-tufts-blue)] hover:bg-blue-600 text-white text-sm font-medium transition-colors flex items-center gap-2 shadow-lg shadow-blue-500/20"
          >
            <Download className="w-4 h-4" />
            Download Report
          </button>
        </div>
      </div>

      <div className="bg-black/40 rounded-xl border border-white/10 p-6 overflow-hidden relative group">
        <div className="absolute top-0 left-0 w-full h-8 bg-gradient-to-b from-black/20 to-transparent pointer-events-none" />
        <div className="font-mono text-sm overflow-x-auto whitespace-pre-wrap max-h-[60vh] overflow-y-auto custom-scrollbar">
          {report.split('\n').map((line, i) => renderLine(line, i))}
        </div>
      </div>
    </motion.div>
  );
}
