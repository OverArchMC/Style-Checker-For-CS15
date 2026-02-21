import React from 'react';
import { motion } from 'motion/react';

export function Spinner() {
  return (
    <div className="flex flex-col items-center justify-center gap-8">
      <div className="relative w-32 h-32">
        {/* Outer Ring */}
        <motion.div
          className="absolute inset-0 border-4 border-[var(--color-tufts-blue)]/20 rounded-full"
        />
        
        {/* Spinning Segment */}
        <motion.div
          className="absolute inset-0 border-4 border-t-[var(--color-tufts-blue)] border-r-transparent border-b-transparent border-l-transparent rounded-full"
          animate={{ rotate: 360 }}
          transition={{ duration: 1.5, repeat: Infinity, ease: "linear" }}
        />
        
        {/* Inner Geometric Shape (Square rotating) */}
        <motion.div
          className="absolute inset-8 border-2 border-[var(--color-tufts-brown)] rounded-lg"
          animate={{ rotate: -180, scale: [1, 0.8, 1] }}
          transition={{ duration: 3, repeat: Infinity, ease: "easeInOut" }}
        />

        {/* Center Dot */}
        <div className="absolute inset-0 flex items-center justify-center">
          <motion.div 
            className="w-2 h-2 bg-white rounded-full"
            animate={{ opacity: [0.5, 1, 0.5] }}
            transition={{ duration: 1, repeat: Infinity }}
          />
        </div>
      </div>
      
      <motion.div
        initial={{ opacity: 0 }}
        animate={{ opacity: 1 }}
        transition={{ delay: 0.5 }}
        className="text-center space-y-1"
      >
        <h3 className="text-xl font-medium">Processing File...</h3>
        <p className="text-sm text-white/40 font-mono">Running style checks</p>
      </motion.div>
    </div>
  );
}
