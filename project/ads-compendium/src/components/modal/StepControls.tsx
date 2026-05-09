import { ChevronLeft, ChevronRight, ChevronsRight, Play, Pause } from 'lucide-react';

interface Props {
  current: number;
  total: number;
  playing: boolean;
  speed: number;
  onPrev: () => void;
  onNext: () => void;
  onSkip: () => void;
  onTogglePlay: () => void;
  onSpeedChange: (v: number) => void;
}

export function StepControls({ current, total, playing, speed, onPrev, onNext, onSkip, onTogglePlay, onSpeedChange }: Props) {
  return (
    <div className="flex items-center gap-3 px-4 py-3 border-t border-gray-700 bg-gray-900">
      <button
        onClick={onPrev}
        disabled={current === 0}
        className="p-1.5 rounded hover:bg-gray-700 text-gray-400 disabled:opacity-30 disabled:cursor-not-allowed transition-colors"
      >
        <ChevronLeft size={18} />
      </button>

      <button
        onClick={onTogglePlay}
        className="p-1.5 rounded hover:bg-gray-700 text-indigo-400 transition-colors"
      >
        {playing ? <Pause size={18} /> : <Play size={18} />}
      </button>

      <button
        onClick={onNext}
        disabled={current >= total - 1}
        className="p-1.5 rounded hover:bg-gray-700 text-gray-400 disabled:opacity-30 disabled:cursor-not-allowed transition-colors"
      >
        <ChevronRight size={18} />
      </button>

      <button
        onClick={onSkip}
        disabled={current >= total - 1}
        className="p-1.5 rounded hover:bg-gray-700 text-gray-500 disabled:opacity-30 disabled:cursor-not-allowed transition-colors"
        title="Skip to end"
      >
        <ChevronsRight size={18} />
      </button>

      <span className="text-xs text-gray-500 font-mono ml-1">
        {current + 1} / {total}
      </span>

      <div className="ml-auto flex items-center gap-2 text-xs text-gray-500">
        <span>Speed</span>
        <input
          type="range"
          min={200}
          max={2000}
          step={100}
          value={2200 - speed}
          onChange={e => onSpeedChange(2200 - Number(e.target.value))}
          className="w-20 accent-indigo-500"
        />
      </div>
    </div>
  );
}
