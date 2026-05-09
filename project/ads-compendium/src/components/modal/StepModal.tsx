import { useEffect, useRef, useState } from 'react';
import { createPortal } from 'react-dom';
import { X } from 'lucide-react';
import { AnimatePresence, motion } from 'framer-motion';
import type { Step } from '../../types';
import { StepCanvas } from './StepCanvas';
import { StepCode } from './StepCode';
import { StepLog } from './StepLog';
import { StepControls } from './StepControls';

interface Props {
  open: boolean;
  onClose: () => void;
  steps: Step[];
  operationLabel: string;
  codeLines: string[];
  isBTree?: boolean;
}

export function StepModal({ open, onClose, steps, operationLabel, codeLines, isBTree }: Props) {
  const [current, setCurrent] = useState(0);
  const [playing, setPlaying] = useState(false);
  const [speed, setSpeed] = useState(800);
  const timerRef = useRef<ReturnType<typeof setTimeout> | undefined>(undefined);

  // Reset when opened with new steps
  useEffect(() => {
    if (open) { setCurrent(0); setPlaying(false); }
  }, [open, steps]);

  // Auto-play timer
  useEffect(() => {
    if (!playing) { clearTimeout(timerRef.current); return; }
    timerRef.current = setTimeout(() => {
      setCurrent(c => {
        if (c >= steps.length - 1) { setPlaying(false); return c; }
        return c + 1;
      });
    }, speed);
    return () => clearTimeout(timerRef.current);
  }, [playing, current, speed, steps.length]);

  // Keyboard navigation
  useEffect(() => {
    if (!open) return;
    const handler = (e: KeyboardEvent) => {
      if (e.key === 'ArrowRight' || e.key === ' ') { e.preventDefault(); setCurrent(c => Math.min(c + 1, steps.length - 1)); setPlaying(false); }
      if (e.key === 'ArrowLeft') { e.preventDefault(); setCurrent(c => Math.max(c - 1, 0)); setPlaying(false); }
      if (e.key === 'Escape') onClose();
    };
    window.addEventListener('keydown', handler);
    return () => window.removeEventListener('keydown', handler);
  }, [open, steps.length, onClose]);

  if (!open || steps.length === 0) return null;

  const step = steps[current];

  const modal = (
    <AnimatePresence>
      <motion.div
        className="fixed inset-0 z-50 flex items-center justify-center p-4"
        initial={{ opacity: 0 }}
        animate={{ opacity: 1 }}
        exit={{ opacity: 0 }}
      >
        {/* Backdrop */}
        <div className="absolute inset-0 bg-black/70 backdrop-blur-sm" onClick={onClose} />

        {/* Dialog */}
        <motion.div
          className="relative z-10 w-full max-w-4xl bg-gray-900 border border-gray-700 rounded-xl shadow-2xl overflow-hidden flex flex-col"
          style={{ maxHeight: '90vh' }}
          initial={{ scale: 0.95, opacity: 0 }}
          animate={{ scale: 1, opacity: 1 }}
          exit={{ scale: 0.95, opacity: 0 }}
          transition={{ type: 'spring', stiffness: 300, damping: 28 }}
        >
          {/* Header */}
          <div className="flex items-center justify-between px-4 py-3 border-b border-gray-700">
            <div>
              <span className="text-xs text-gray-500 font-mono">Step {current + 1} of {steps.length}</span>
              <h2 className="text-sm font-semibold text-white mt-0.5">{operationLabel}</h2>
            </div>
            <button onClick={onClose} className="p-1.5 rounded hover:bg-gray-700 text-gray-500 transition-colors">
              <X size={16} />
            </button>
          </div>

          {/* Body: Canvas + Code side by side */}
          <div className="flex flex-1 min-h-0 overflow-hidden">
            {/* Canvas */}
            <div className="flex-1 border-r border-gray-700 bg-gray-950 overflow-hidden">
              <StepCanvas state={step.snapshot} isBTree={isBTree} />
            </div>
            {/* Code */}
            <div className="w-72 shrink-0 overflow-hidden">
              <StepCode codeLines={codeLines} activeLine={step.codeLineIndex} />
            </div>
          </div>

          {/* Log */}
          <StepLog description={step.description} />

          {/* Controls */}
          <StepControls
            current={current}
            total={steps.length}
            playing={playing}
            speed={speed}
            onPrev={() => { setCurrent(c => Math.max(c - 1, 0)); setPlaying(false); }}
            onNext={() => { setCurrent(c => Math.min(c + 1, steps.length - 1)); setPlaying(false); }}
            onSkip={() => { setCurrent(steps.length - 1); setPlaying(false); }}
            onTogglePlay={() => setPlaying(p => !p)}
            onSpeedChange={setSpeed}
          />
        </motion.div>
      </motion.div>
    </AnimatePresence>
  );

  return createPortal(modal, document.body);
}
