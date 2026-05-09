import { useEffect, useRef } from 'react';

interface Props {
  codeLines: string[];
  activeLine: number;
}

export function StepCode({ codeLines, activeLine }: Props) {
  const activeRef = useRef<HTMLDivElement>(null);

  useEffect(() => {
    activeRef.current?.scrollIntoView({ block: 'nearest', behavior: 'smooth' });
  }, [activeLine]);

  return (
    <div className="overflow-y-auto h-full">
      <pre className="text-xs font-mono p-3 leading-relaxed">
        {codeLines.map((line, i) => (
          <div
            key={i}
            ref={i === activeLine ? activeRef : undefined}
            className={`flex px-1 rounded transition-colors ${
              i === activeLine ? 'bg-indigo-600/30 text-indigo-200' : 'text-gray-400'
            }`}
          >
            <span className="w-6 shrink-0 text-gray-700 text-right mr-3 select-none">{i + 1}</span>
            <span className="whitespace-pre">{line || ' '}</span>
          </div>
        ))}
      </pre>
    </div>
  );
}
