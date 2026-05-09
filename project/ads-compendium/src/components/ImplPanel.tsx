import { ExternalLink, FileCode, AlertCircle, WifiOff } from 'lucide-react';
import type { FileEntry, DirStatus, FileStatus } from '../hooks/useGithubDir';

interface Props {
  codeLines: string[];        // offline fallback
  githubUrl: string;
  dirStatus: DirStatus;
  files: FileEntry[];
  selected: string;
  contentMap: Record<string, string>;
  fileStatus: FileStatus;
  onSelectFile: (file: FileEntry) => void;
}

export function ImplPanel({
  codeLines, githubUrl,
  dirStatus, files, selected, contentMap, fileStatus, onSelectFile,
}: Props) {
  const isLive = dirStatus === 'ok';
  const showFallback = dirStatus === 'error';

  const displayLines: string[] =
    showFallback
      ? codeLines
      : selected && contentMap[selected]
        ? contentMap[selected].split('\n')
        : [];

  return (
    <div className="border-t border-gray-800 flex flex-col shrink-0">
      {/* Header */}
      <div className="flex items-center justify-between px-4 py-2.5 border-b border-gray-800">
        <div className="flex items-center gap-2">
          <span className="text-xs font-medium text-gray-400 uppercase tracking-wider">
            Implementation
          </span>
          {isLive && (
            <span className="px-1.5 py-0.5 rounded text-xs font-mono font-medium bg-orange-500/15 text-orange-400 border border-orange-500/20">
              C
            </span>
          )}
          {showFallback && (
            <span className="flex items-center gap-1 text-xs text-gray-600">
              <WifiOff size={11} /> offline fallback
            </span>
          )}
        </div>
        <a
          href={githubUrl}
          target="_blank"
          rel="noopener noreferrer"
          className="flex items-center gap-1.5 text-xs text-indigo-400 hover:text-indigo-300 transition-colors"
        >
          <ExternalLink size={12} />
          View on GitHub
        </a>
      </div>

      {/* Body: file tree + code */}
      <div className="flex max-h-52 min-h-0">

        {/* File tree — hidden when directory failed (fallback mode) */}
        {!showFallback && (
          <div className="w-32 shrink-0 border-r border-gray-800 overflow-y-auto py-1">
            {files.length === 0 ? (
              <p className="text-xs text-gray-700 p-3">No C files</p>
            ) : (
              files.map(file => (
                <button
                  key={file.name}
                  onClick={() => onSelectFile(file)}
                  className={`w-full flex items-center gap-1.5 px-2 py-1.5 text-left text-xs truncate transition-colors ${
                    selected === file.name
                      ? 'bg-indigo-600/20 text-indigo-300'
                      : 'text-gray-400 hover:text-gray-200 hover:bg-gray-800'
                  }`}
                >
                  <FileCode size={11} className="shrink-0 opacity-60" />
                  <span className="truncate">{file.name}</span>
                </button>
              ))
            )}
          </div>
        )}

        {/* Code content */}
        <div className="flex-1 overflow-auto">
          {fileStatus === 'loading' ? (
            <div className="p-4 space-y-2">
              {[80, 55, 90, 65, 75, 45].map((w, i) => (
                <div
                  key={i}
                  className="h-3 rounded bg-gray-800 animate-pulse"
                  style={{ width: `${w}%` }}
                />
              ))}
            </div>
          ) : fileStatus === 'error' ? (
            <div className="flex items-center gap-2 p-4 text-xs text-gray-600">
              <AlertCircle size={13} /> Could not load file
            </div>
          ) : displayLines.length === 0 ? (
            <p className="p-4 text-xs text-gray-700 select-none">Select a file</p>
          ) : (
            <pre className="text-xs text-gray-300 font-mono p-3 leading-relaxed">
              {displayLines.map((line, i) => (
                <div key={i} className="flex">
                  <span className="w-8 shrink-0 text-gray-700 select-none text-right mr-4">
                    {i + 1}
                  </span>
                  <span className="whitespace-pre">{line}</span>
                </div>
              ))}
            </pre>
          )}
        </div>
      </div>
    </div>
  );
}
