# ImplPanel Tree View — Change Notes

Recorded: 2026-05-09  
Scope: `src/types.ts`, `src/components/ImplPanel.tsx`, `src/data/*.ts`, `src/pages/*.tsx`

---

## Motivation

Each assignment directory may contain multiple C source files (e.g. `BT.c` + `temp.c` in `assignments/04`) and future `.h` headers. The previous single-`rawCodeUrl` model only loaded one file per structure. A directory-aware tree view was needed.

---

## Directory Inventory (as of 2026-05-09)

| Assignment | Relevant files | Notes |
|---|---|---|
| `assignments/01` (BST) | `BST.c` | Single file |
| `assignments/02` (RBT) | `RBT.c` | Single file; `.jpg` diagrams excluded |
| `assignments/04` (B-Tree) | `BT.c`, `temp.c` | Two C files; `temp.c` is a scratch/work file |
| `assignments/05` (Binomial Heap) | `main.c` | Single file |

Filter applied: extensions `.c` and `.h` only. `.exe`, `.jpg`, `.md` are excluded.

---

## Type Changes (`src/types.ts`)

`rawCodeUrl: string` removed, replaced with:

```ts
repoDirUrl: string;  // GitHub API contents URL for the assignment directory
```

`githubUrl` now points to the **directory tree view** rather than a single file.

---

## Data File URL Updates

| File | `repoDirUrl` | `githubUrl` |
|---|---|---|
| `bstInfo.ts` | `api.github.com/.../contents/assignments/01` | `github.com/.../tree/master/assignments/01` |
| `rbtInfo.ts` | `api.github.com/.../contents/assignments/02` | `github.com/.../tree/master/assignments/02` |
| `btreeInfo.ts` | `api.github.com/.../contents/assignments/04` | `github.com/.../tree/master/assignments/04` |
| `binomialHeapInfo.ts` | `api.github.com/.../contents/assignments/05` | `github.com/.../tree/master/assignments/05` |

---

## ImplPanel Architecture

### Layout

```
┌─────────────────────────────────────────────────────┐
│ Implementation  [C]         View directory on GH ↗  │
├──────────────┬──────────────────────────────────────┤
│ 📄 BST.c  ◀ │  1  #include <stdio.h>               │
│ 📄 BST.h    │  2  #include <stdlib.h>              │
│             │  3                                    │
│             │  ...                                  │
└──────────────┴──────────────────────────────────────┘
```

Left panel: fixed `w-32`, lists `.c`/`.h` files; active file highlighted.  
Right panel: `flex-1 overflow-auto`, shows line-numbered content.  
Total height: `max-h-52`, consistent with previous ImplPanel.

### State Machine

```
mount
  → fetch(repoDirUrl)               // GitHub API directory listing
  → filter entries by .c / .h ext
  → setFiles([...])
  → auto-select files[0]
  → fetch(files[0].download_url)    // raw.githubusercontent.com
  → cache in contentMap[name]
  → render

user clicks file
  → if contentMap[name] exists: render from cache (no network)
  → else: fetch(file.download_url) → cache → render
```

### Fetch Strategy

| Request | URL | Frequency |
|---|---|---|
| Directory listing | `api.github.com/repos/.../contents/{path}` | Once on mount, per structure page |
| File content | `raw.githubusercontent.com/.../{file}` | Once per unique file selected |

Content is cached in `contentMap: Record<string, string>` (React state). Navigating between files within one page visit never re-fetches.

### Rate Limits

- `api.github.com`: 60 unauthenticated requests/hour. Each page visit = 1 directory listing. Four structure pages = 4 requests. Well within limit.
- `raw.githubusercontent.com`: CDN, no rate limit concerns for this use case.

### Error / Loading States

| Scenario | Left panel | Right panel |
|---|---|---|
| Directory loading | shimmer skeleton list | empty |
| Directory error | "Could not load directory" message | `codeLines` fallback |
| File loading | file selected (dim) | shimmer skeleton |
| File error | file selected (error icon) | "Could not load file" message |
| Offline / fallback | — | `codeLines` from data file |

---

## Future Considerations

- If subdirectories appear (e.g. a `src/` subfolder inside an assignment), the GitHub API returns `type: "dir"` entries. Current filter ignores them. Recursive tree loading is a future enhancement.
- Adding a GitHub Personal Access Token as a Vite env variable (`VITE_GH_TOKEN`) would raise the rate limit to 5000/hour if needed.
- The `temp.c` file in `assignments/04` is visible in the tree — the student may want to remove or rename it in the repo to clean up the presentation.
