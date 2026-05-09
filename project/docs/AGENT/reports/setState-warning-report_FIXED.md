# React setState Warning — Fix Notes

Recorded: 2026-05-09  
Resolves: `setState-warning-report.md`  
File: `src/hooks/useGithubDir.ts`

---

## Solution Applied

**Option 1 + Option 2 combined** — single state object (eliminates multi-call reset) with `AbortController` cleanup (cancels stale fetches on fast navigation).

---

## Changes Made

### 1. State collapsed into one object

**Before:** 5 separate state variables, each reset individually at the top of the effect.

```ts
const [dirStatus, setDirStatus] = useState<DirStatus>('loading');
const [files, setFiles] = useState<FileEntry[]>([]);
const [selected, setSelected] = useState('');
const [contentMap, setContentMap] = useState<Record<string, string>>({});
const [fileStatus, setFileStatus] = useState<FileStatus>('idle');

useEffect(() => {
  setDirStatus('loading');  // ← 5 separate setState calls
  setFiles([]);
  setSelected('');
  setContentMap({});
  setFileStatus('idle');
  ...
}, [repoDirUrl]);
```

**After:** Single `DirState` interface with a `RESET` constant — one `setState` call resets everything atomically.

```ts
interface DirState {
  dirStatus: DirStatus;
  files: FileEntry[];
  selected: string;
  contentMap: Record<string, string>;
  fileStatus: FileStatus;
}

const RESET: DirState = { dirStatus: 'loading', files: [], selected: '', contentMap: {}, fileStatus: 'idle' };

const [state, setState] = useState<DirState>(RESET);

useEffect(() => {
  setState(RESET);  // ← single call
  ...
}, [repoDirUrl]);
```

---

### 2. `AbortController` added for fetch cleanup

Prevents stale fetch callbacks from updating state after the component unmounts or `repoDirUrl` changes mid-flight.

```ts
const controller = new AbortController();

fetch(repoDirUrl, { signal: controller.signal })
  ...
  .catch(err => { if (err.name !== 'AbortError') setState(s => ({ ...s, dirStatus: 'error' })); });

return () => controller.abort();
```

`AbortError` is explicitly ignored so cancelled requests do not trigger the error UI.

---

### 3. `setState` inside `setState` updater removed (`selectFile`)

**Before:** `setFileStatus` was called inside the `setContentMap` functional updater — a React anti-pattern (side effects inside updaters can run multiple times in StrictMode).

```ts
function selectFile(file: FileEntry) {
  setSelected(file.name);
  setContentMap(prev => {
    if (prev[file.name] !== undefined) return prev;
    setFileStatus('loading');   // ← setState inside setState updater
    fetch(...)...
    return prev;
  });
}
```

**After:** Reads `state` directly (safe in an event handler), calls `setState` at the top level only.

```ts
function selectFile(file: FileEntry) {
  if (state.contentMap[file.name] !== undefined) {
    setState(s => ({ ...s, selected: file.name }));
    return;
  }
  setState(s => ({ ...s, selected: file.name, fileStatus: 'loading' }));
  fetch(file.downloadUrl)
    .then(...)
    .then(text => setState(s => ({ ...s, contentMap: { ...s.contentMap, [file.name]: text }, fileStatus: 'ok' })))
    .catch(() => setState(s => ({ ...s, fileStatus: 'error' })));
}
```

---

## Verification

`npm run build` (tsc + vite) passed with 0 errors after the refactor.

---

## Why Not Option 3 (key prop)?

A `key={repoDirUrl}` on the page component would force a full unmount/remount on navigation, discarding DS visualisation state (tree nodes, step history, modal). State lifting via `useGithubDir` keeps the fetch lifecycle isolated without touching the rest of the page.
