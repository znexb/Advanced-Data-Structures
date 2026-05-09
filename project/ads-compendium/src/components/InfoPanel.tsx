import { useState } from 'react';
import type { StructureInfo } from '../types';
import { ComplexityTable } from './ComplexityTable';

type Tab = 'notes' | 'usecases' | 'funfacts' | 'complexity';

interface Props { info: StructureInfo; }

const TABS: { id: Tab; label: string }[] = [
  { id: 'notes',      label: 'Notes' },
  { id: 'usecases',   label: 'Use Cases' },
  { id: 'funfacts',   label: 'Fun Facts' },
  { id: 'complexity', label: 'Complexity' },
];

export function InfoPanel({ info }: Props) {
  const [tab, setTab] = useState<Tab>('notes');

  return (
    <div className="flex flex-col h-full">
      <div className="flex border-b border-gray-800">
        {TABS.map(({ id, label }) => (
          <button
            key={id}
            onClick={() => setTab(id)}
            className={`px-4 py-2.5 text-xs font-medium transition-colors ${
              tab === id
                ? 'text-indigo-400 border-b-2 border-indigo-500'
                : 'text-gray-500 hover:text-gray-300'
            }`}
          >
            {label}
          </button>
        ))}
      </div>

      <div className="flex-1 overflow-y-auto p-4">
        {tab === 'notes' && (
          <ul className="space-y-3">
            {info.notes.map((n, i) => (
              <li key={i} className="flex gap-2 text-sm text-gray-300">
                <span className="text-indigo-500 mt-0.5 shrink-0">▸</span>
                {n}
              </li>
            ))}
          </ul>
        )}

        {tab === 'usecases' && (
          <ul className="space-y-3">
            {info.useCases.map((u, i) => (
              <li key={i} className="flex gap-2 text-sm text-gray-300">
                <span className="text-green-500 mt-0.5 shrink-0">◆</span>
                {u}
              </li>
            ))}
          </ul>
        )}

        {tab === 'funfacts' && (
          <ul className="space-y-3">
            {info.funFacts.map((f, i) => (
              <li key={i} className="flex gap-2 text-sm text-gray-300">
                <span className="text-yellow-400 mt-0.5 shrink-0">★</span>
                {f}
              </li>
            ))}
          </ul>
        )}

        {tab === 'complexity' && <ComplexityTable rows={info.complexity} />}
      </div>
    </div>
  );
}
