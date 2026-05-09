# Skeleton Loading Screen — Change Notes

Recorded: 2026-05-09  
Scope: `src/hooks/useGithubDir.ts`, `src/components/StructurePageSkeleton.tsx`, `src/components/ImplPanel.tsx`, `src/pages/*.tsx`

---

## Motivation

Before this change, all four structure pages mounted the full page layout immediately — the `ImplPanel` would fetch the GitHub directory listing internally and show inline shimmer only inside the code area. The `StructureInfo` was always visible and the page shell rendered fully even while network requests were in flight.

The goal was to show a full-page skeleton that fills `<main className="flex-1 overflow-y-auto">` (defined in `Layout.tsx`) while the initial GitHub directory fetch is pending, then swap to the real page once data is ready.

---

## Architecture Change: State Lifted to Page

The directory fetch logic was extracted out of `ImplPanel` into a standalone hook, and pages now own the loading state.

### Before

```
BSTPage
  └── ImplPanel   ← owned fetch, dirStatus, files, selected, contentMap, fileStatus
```

### After

```
BSTPage
  ├── useGithubDir(repoDirUrl)   ← owns all fetch state
  ├── if loading → <StructurePageSkeleton />
  └── else → full page layout
        └── ImplPanel (pure display — receives all state as props)
```

---

## New Files

### `src/hooks/useGithubDir.ts`

Encapsulates the GitHub directory listing and per-file content fetching that was previously inlined in `ImplPanel`.

**Exported types:**

```ts
export interface FileEntry { name: string; downloadUrl: string; }
export type DirStatus = 'loading' | 'ok' | 'error';
export type FileStatus = 'idle' | 'loading' | 'ok' | 'error';
```

**Return value:**

```ts
{ dirStatus, files, selected, contentMap, fileStatus, selectFile }
```

Behaviour:
- On mount, fetches `repoDirUrl` (GitHub API), filters entries to `.c`/`.h`, and auto-selects + loads the first file.
- `selectFile(file)` checks `contentMap` before fetching — subsequent selections of the same file are free.
- Resets all state when `repoDirUrl` changes.

### `src/components/StructurePageSkeleton.tsx`

A pixel-matched shimmer layout that mirrors the real page structure. Fills the entire `<main>` area so there is no layout shift when the real page mounts.

Sections mirrored:

| Section | Shimmer contents |
|---|---|
| `PageHeader` | Title bar + subtitle line + badge pill |
| Canvas area | Faint ghost tree (SVG circles + lines at fixed `%` positions) |
| `ControlBar` | Row of shimmer button/input shapes |
| `ImplPanel` | File list sidebar + 6 code-line shimmers |
| `InfoPanel` | Tab bar + 10 content line shimmers |

The ghost SVG tree uses `stroke="#1f2937"` (Tailwind `gray-800`) — barely visible, just enough to suggest structure without distracting.

`Shimmer` internal component accepts `className` and optional `style?: CSSProperties` (needed for percentage-width lines generated in `.map()`).

---

## Modified Files

### `src/components/ImplPanel.tsx`

Converted from a self-fetching component to a **pure display component**. The `repoDirUrl` prop was removed; all state now arrives from the parent via props:

```ts
interface Props {
  codeLines: string[];
  githubUrl: string;
  dirStatus: DirStatus;
  files: FileEntry[];
  selected: string;
  contentMap: Record<string, string>;
  fileStatus: FileStatus;
  onSelectFile: (file: FileEntry) => void;
}
```

`Loader2` was removed from the lucide-react import (no longer needed — loading is handled at page level by the skeleton).

### `src/pages/{BST,RBT,BTree,BinomialHeap}Page.tsx`

All four pages updated with the same pattern:

```tsx
const dir = useGithubDir(info.repoDirUrl);
if (dir.dirStatus === 'loading') return <StructurePageSkeleton />;
// ... full layout with ImplPanel receiving dir.* props
```

---

## Visual States After This Change

| Phase | What the user sees |
|---|---|
| Initial load (fetching) | Full-page shimmer skeleton |
| Directory fetch error | Real page mounts with `WifiOff` indicator in ImplPanel, falls back to `codeLines` |
| File fetch loading | Real page visible; code area shows shimmer |
| Nominal | Real page with live C code, file tree sidebar |

---

## Build Verification

`npm run build` (tsc + vite) completed with 0 errors after:
- Adding `style?: CSSProperties` to `Shimmer` component (TS required it for inline width styles)
- Removing unused `Loader2` import from `ImplPanel.tsx`
