import { useEffect, useState } from 'react';

const CODE_EXTS = new Set(['c', 'h']);
const getExt = (name: string) => name.split('.').pop()?.toLowerCase() ?? '';

export interface FileEntry {
  name: string;
  downloadUrl: string;
}

export type DirStatus = 'loading' | 'ok' | 'error';
export type FileStatus = 'idle' | 'loading' | 'ok' | 'error';

interface DirState {
  dirStatus: DirStatus;
  files: FileEntry[];
  selected: string;
  contentMap: Record<string, string>;
  fileStatus: FileStatus;
}

const RESET: DirState = {
  dirStatus: 'loading',
  files: [],
  selected: '',
  contentMap: {},
  fileStatus: 'idle',
};

export function useGithubDir(repoDirUrl: string) {
  const [state, setState] = useState<DirState>(RESET);

  useEffect(() => {
    setState(RESET);
    const controller = new AbortController();

    fetch(repoDirUrl, { signal: controller.signal })
      .then(r => { if (!r.ok) throw new Error(`${r.status}`); return r.json(); })
      .then(async (entries: { name: string; type: string; download_url: string }[]) => {
        const filtered = entries
          .filter(e => e.type === 'file' && CODE_EXTS.has(getExt(e.name)))
          .map(e => ({ name: e.name, downloadUrl: e.download_url }));

        await new Promise(resolve => setTimeout(resolve, 500));

        setState(s => ({ ...s, files: filtered, dirStatus: 'ok' }));

        if (filtered.length === 0) return;
        const first = filtered[0];
        setState(s => ({ ...s, selected: first.name, fileStatus: 'loading' }));

        fetch(first.downloadUrl, { signal: controller.signal })
          .then(r => { if (!r.ok) throw new Error(); return r.text(); })
          .then(text => setState(s => ({ ...s, contentMap: { [first.name]: text }, fileStatus: 'ok' })))
          .catch(err => { if (err.name !== 'AbortError') setState(s => ({ ...s, fileStatus: 'error' })); });
      })
      .catch(err => { if (err.name !== 'AbortError') setState(s => ({ ...s, dirStatus: 'error' })); });

    return () => controller.abort();
  }, [repoDirUrl]);

  function selectFile(file: FileEntry) {
    if (state.contentMap[file.name] !== undefined) {
      setState(s => ({ ...s, selected: file.name }));
      return;
    }
    setState(s => ({ ...s, selected: file.name, fileStatus: 'loading' }));
    fetch(file.downloadUrl)
      .then(r => { if (!r.ok) throw new Error(); return r.text(); })
      .then(text => setState(s => ({ ...s, contentMap: { ...s.contentMap, [file.name]: text }, fileStatus: 'ok' })))
      .catch(() => setState(s => ({ ...s, fileStatus: 'error' })));
  }

  return { ...state, selectFile };
}
