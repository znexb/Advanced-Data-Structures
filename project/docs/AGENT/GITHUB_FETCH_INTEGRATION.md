# GitHub Fetch Integration — Change Notes

Recorded: 2026-05-09  
Scope: `src/types.ts`, `src/components/ImplPanel.tsx`, `src/data/*.ts`, `src/pages/*.tsx`

---

## What Changed

### `StructureInfo` type (`src/types.ts`)

Two new fields added:

```ts
githubUrl: string;    // human-readable GitHub page for the file
rawCodeUrl: string;   // raw.githubusercontent.com URL for live fetch
```

`codeLines: string[]` is retained as an **offline fallback** — rendered if the fetch fails or the network is unavailable.

> **Note:** `rawCodeUrl` was later superseded by `repoDirUrl` in the tree-view rework (see `IMPL_PANEL_TREE_VIEW.md`). This document describes the intermediate single-file state.

---

### `ImplPanel` component (`src/components/ImplPanel.tsx`)

Replaced static code rendering with a fetch-on-mount pattern:

```
mount
  → fetch(rawCodeUrl)
  → on success: split('\n') → render with line numbers + "C" badge
  → on error:   render codeLines fallback + WifiOff indicator
  → while loading: animated shimmer skeleton (6 lines)
```

Visual states:

| State | Badge | Content |
|---|---|---|
| Loading | spinner | shimmer skeleton |
| Success | orange **`C`** | live C source from GitHub |
| Error | `WifiOff` "offline fallback" | static `codeLines` from data file |

---

### Data files

All four `src/data/*.ts` files updated:

| File | `githubUrl` | `rawCodeUrl` |
|---|---|---|
| `bstInfo.ts` | `.../01/BST.c` (blob) | `.../01/BST.c` (raw) |
| `rbtInfo.ts` | `.../02/RBT.c` (blob) | `.../02/RBT.c` (raw) |
| `btreeInfo.ts` | `.../04/BT.c` (blob) | `.../04/BT.c` (raw) |
| `binomialHeapInfo.ts` | `.../05/main.c` (blob) | `.../05/main.c` (raw) |

### Pages

All four `*Page.tsx` files updated to pass `rawCodeUrl={info.rawCodeUrl}` to `ImplPanel`.

---

## Rate Limit Considerations

- Provider: `raw.githubusercontent.com`
- Auth: none (public repo)
- Rate limit: not applicable for raw content CDN (differs from `api.github.com`)
- CORS: enabled by default on `raw.githubusercontent.com`

---

## Known Limitations at This Stage

- Single-file per structure — no support for multi-file directories or headers
- Superseded by `repoDirUrl` directory-tree approach (see next doc)
