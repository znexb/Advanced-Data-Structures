import type { CSSProperties } from 'react';

function Shimmer({ className, style }: { className: string; style?: CSSProperties }) {
  return <div className={`animate-pulse rounded bg-gray-800 ${className}`} style={style} />;
}

export function StructurePageSkeleton() {
  return (
    <div className="flex flex-col h-screen select-none pointer-events-none">

      {/* PageHeader */}
      <div className="px-6 pt-6 pb-4 border-b border-gray-800 shrink-0">
        <div className="flex items-start justify-between gap-4">
          <div className="space-y-2">
            <Shimmer className="h-6 w-52" />
            <Shimmer className="h-3.5 w-80" />
          </div>
          <Shimmer className="h-6 w-24 rounded-full" />
        </div>
      </div>

      {/* Body */}
      <div className="flex flex-1 min-h-0 overflow-hidden">

        {/* Canvas column */}
        <div className="flex flex-col flex-1 min-w-0 border-r border-gray-800">

          {/* SVG canvas area */}
          <div className="flex-1 bg-gray-950 relative overflow-hidden max-h-77">
            {/* Faint ghost tree */}
            <svg className="absolute inset-0 w-full h-full" aria-hidden>
              {/* Root */}
              <circle cx="50%" cy="28%" r="22" fill="none" stroke="#1f2937" strokeWidth="1.5" />
              <line x1="50%" y1="30%" x2="35%" y2="48%" stroke="#1f2937" strokeWidth="1.5" />
              <line x1="50%" y1="30%" x2="65%" y2="48%" stroke="#1f2937" strokeWidth="1.5" />
              {/* Level 2 */}
              <circle cx="35%" cy="52%" r="22" fill="none" stroke="#1f2937" strokeWidth="1.5" />
              <circle cx="65%" cy="52%" r="22" fill="none" stroke="#1f2937" strokeWidth="1.5" />
              <line x1="35%" y1="54%" x2="25%" y2="70%" stroke="#1f2937" strokeWidth="1.5" />
              <line x1="65%" y1="54%" x2="75%" y2="70%" stroke="#1f2937" strokeWidth="1.5" />
              {/* Level 3 */}
              <circle cx="25%" cy="74%" r="22" fill="none" stroke="#1f2937" strokeWidth="1.5" />
              <circle cx="75%" cy="74%" r="22" fill="none" stroke="#1f2937" strokeWidth="1.5" />
            </svg>
          </div>

          {/* ControlBar */}
          <div className="flex items-center gap-3 px-4 py-3 border-t border-gray-800 bg-gray-900 shrink-0">
            <Shimmer className="h-7 w-16" />
            <Shimmer className="h-7 w-20" />
            <Shimmer className="h-7 w-16" />
            <Shimmer className="h-7 w-20" />
            <Shimmer className="h-7 w-16" />
            <Shimmer className="h-7 w-20" />
            <div className="ml-auto flex gap-2">
              <Shimmer className="h-7 w-20" />
              <Shimmer className="h-7 w-16" />
            </div>
          </div>

          {/* ImplPanel */}
          <div className="border-t border-gray-800 shrink-0">
            {/* Header */}
            <div className="flex items-center justify-between px-4 py-2.5 border-b border-gray-800">
              <div className="flex items-center gap-2">
                <Shimmer className="h-3 w-28" />
                <Shimmer className="h-4 w-6 rounded" />
              </div>
              <Shimmer className="h-3 w-24" />
            </div>
            {/* Split: file list + code */}
            <div className="flex max-h-52">
              <div className="w-32 border-r border-gray-800 p-2 space-y-1.5">
                <Shimmer className="h-5 w-full" />
                <Shimmer className="h-5 w-4/5" />
              </div>
              <div className="flex-1 p-4 space-y-2">
                {[80, 55, 90, 65, 75, 45].map((w, i) => (
                  <Shimmer key={i} className="h-3" style={{ width: `${w}%` } as React.CSSProperties} />
                ))}
              </div>
            </div>
          </div>
        </div>

        {/* InfoPanel */}
        <div className="w-72 shrink-0 flex flex-col overflow-hidden">
          {/* Tabs */}
          <div className="flex border-b border-gray-800 px-1 pt-1 gap-1">
            {[20, 24, 20, 22].map((w, i) => (
              <Shimmer key={i} className={`h-7 w-${w}`} />
            ))}
          </div>
          {/* Content lines */}
          <div className="flex-1 p-4 space-y-3">
            {[90, 75, 85, 60, 95, 70, 80, 55, 88, 65].map((w, i) => (
              <Shimmer key={i} className="h-3" style={{ width: `${w}%` } as React.CSSProperties} />
            ))}
          </div>
        </div>
      </div>
    </div>
  );
}
