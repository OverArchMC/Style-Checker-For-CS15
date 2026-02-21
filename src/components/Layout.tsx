import React from 'react';

interface LayoutProps {
  children: React.ReactNode;
}

export function Layout({ children }: LayoutProps) {
  return (
    <div className="min-h-screen flex flex-col items-center justify-center bg-[var(--color-tufts-dark)] text-[var(--color-tufts-light)] p-4">
      <header className="absolute top-0 left-0 w-full p-6 flex justify-between items-center">
        <div className="flex items-center gap-2">
          <div className="w-8 h-8 bg-[var(--color-tufts-blue)] rounded-full flex items-center justify-center font-bold text-white">
            CS
          </div>
          <h1 className="text-xl font-bold tracking-tight">CS15 Style Checker</h1>
        </div>
        <div className="text-sm opacity-50 font-mono">Tufts University</div>
      </header>
      
      <main className="w-full max-w-3xl flex flex-col items-center justify-center flex-grow">
        {children}
      </main>

      <footer className="absolute bottom-0 w-full p-4 text-center text-xs opacity-30">
        Built for students.
      </footer>
    </div>
  );
}
