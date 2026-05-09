import { useState } from 'react';
import { Plus, Minus, Search, RotateCcw, Shuffle } from 'lucide-react';

interface Props {
  onInsert: (key: number) => void;
  onDelete?: (key: number) => void;
  onSearch?: (key: number) => void;
  onReset: () => void;
  onRandomFill: () => void;
  onExtractMin?: () => void;
  showExtract?: boolean;
  extraControls?: React.ReactNode;
}


export function ControlBar({
  onInsert, onDelete, onSearch, onReset, onRandomFill, onExtractMin, showExtract, extraControls,
}: Props) {
  const [ins, setIns] = useState('');
  const [del, setDel] = useState('');
  const [srch, setSrch] = useState('');

  const tryInsert = () => {
    const n = parseInt(ins, 10);
    if (!isNaN(n)) { onInsert(n); setIns(''); }
  };
  const tryDelete = () => {
    if (!onDelete) return;
    const n = parseInt(del, 10);
    if (!isNaN(n)) { onDelete(n); setDel(''); }
  };
  const trySearch = () => {
    if (!onSearch) return;
    const n = parseInt(srch, 10);
    if (!isNaN(n)) { onSearch(n); setSrch(''); }
  };

  const key = (fn: () => void) => (e: React.KeyboardEvent<HTMLInputElement>) => { if (e.key === 'Enter') fn(); };

  return (
    <div className="flex flex-wrap items-center gap-2 px-4 py-3 border-t border-gray-800 bg-gray-900">
      {/* Insert */}
      <div className="flex items-center gap-1.5">
        <input
          type="number"
          value={ins}
          onChange={e => setIns(e.target.value)}
          onKeyDown={key(tryInsert)}
          placeholder="key"
          className="w-20 bg-gray-800 border border-gray-700 rounded px-2 py-1.5 text-sm text-gray-200 placeholder-gray-600 focus:outline-none focus:border-indigo-500"
        />
        <button onClick={tryInsert} className="flex items-center gap-1 px-3 py-1.5 bg-indigo-600 hover:bg-indigo-500 text-white text-sm rounded transition-colors">
          <Plus size={14} /> Insert
        </button>
      </div>

      {/* Delete */}
      {onDelete && (
        <div className="flex items-center gap-1.5">
          <input
            type="number"
            value={del}
            onChange={e => setDel(e.target.value)}
            onKeyDown={key(tryDelete)}
            placeholder="key"
            className="w-20 bg-gray-800 border border-gray-700 rounded px-2 py-1.5 text-sm text-gray-200 placeholder-gray-600 focus:outline-none focus:border-indigo-500"
          />
          <button onClick={tryDelete} className="flex items-center gap-1 px-3 py-1.5 bg-red-700 hover:bg-red-600 text-white text-sm rounded transition-colors">
            <Minus size={14} /> Delete
          </button>
        </div>
      )}

      {/* Search */}
      {onSearch && (
        <div className="flex items-center gap-1.5">
          <input
            type="number"
            value={srch}
            onChange={e => setSrch(e.target.value)}
            onKeyDown={key(trySearch)}
            placeholder="key"
            className="w-20 bg-gray-800 border border-gray-700 rounded px-2 py-1.5 text-sm text-gray-200 placeholder-gray-600 focus:outline-none focus:border-indigo-500"
          />
          <button onClick={trySearch} className="flex items-center gap-1 px-3 py-1.5 bg-gray-700 hover:bg-gray-600 text-white text-sm rounded transition-colors">
            <Search size={14} /> Search
          </button>
        </div>
      )}

      {/* Extract Min */}
      {showExtract && onExtractMin && (
        <button onClick={onExtractMin} className="flex items-center gap-1 px-3 py-1.5 bg-orange-700 hover:bg-orange-600 text-white text-sm rounded transition-colors">
          <Minus size={14} /> Extract Min
        </button>
      )}

      {extraControls}

      <div className="ml-auto flex items-center gap-2">
        <button onClick={onRandomFill} className="flex items-center gap-1 px-3 py-1.5 bg-gray-700 hover:bg-gray-600 text-gray-200 text-sm rounded transition-colors">
          <Shuffle size={14} /> Random
        </button>
        <button onClick={onReset} className="flex items-center gap-1 px-3 py-1.5 bg-gray-800 hover:bg-gray-700 text-gray-400 text-sm rounded transition-colors">
          <RotateCcw size={14} /> Reset
        </button>
      </div>
    </div>
  );
}
