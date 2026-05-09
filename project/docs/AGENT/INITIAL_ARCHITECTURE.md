# Initial Architecture — Agent Notes & Decisions

Generated: 2026-05-09  
Agent: Claude Sonnet 4.6

---

## Source Analysis

`docs/architecture.md` describes a 5-route React SPA:

| Route | Page |
|---|---|
| `/` | DashboardPage — 4-card overview grid |
| `/bst` | Binary Search Tree |
| `/rbt` | Red-Black Tree |
| `/btree` | B-Tree |
| `/binomial-heap` | Binomial Heap |

Every structure page shares an identical shell (Layout → Sidebar + PageContent) with structure-specific logic injected through a hook. The step-by-step modal (`StepModal`) is universal — all 4 structures feed the same `Step[]` type into it.

---

## Dependency Decisions

| Package | Version target | Reason |
|---|---|---|
| `react-router-dom` | v7 | Required for `<BrowserRouter>` / `<Routes>` / `<Route>` |
| `tailwindcss` | v4 | Spec requirement; v4 integrates cleanly with Vite via `@tailwindcss/vite` |
| `@tailwindcss/vite` | latest | Vite 8 plugin; replaces PostCSS setup in v3 |
| `framer-motion` | v12 | Node enter/exit animations + SVG path animations |
| `lucide-react` | latest | Icon set for sidebar + buttons |

---

## Type System

### Core shared types (`src/types.ts`)

```ts
type NodeColor = 'default' | 'red' | 'black' | 'highlighted' | 'found' | 'new';

interface NodeData {
  id: string;
  label: string;    // text displayed inside node
  x: number;
  y: number;
  color?: NodeColor;
  width?: number;   // B-Tree multi-key node rectangle width
}

interface EdgeData {
  id: string;
  fromId: string;
  toId: string;
  highlighted?: boolean;
}

interface TreeState {
  nodes: NodeData[];
  edges: EdgeData[];
  width: number;
  height: number;
}

interface Step {
  description: string;
  highlightNodeIds: string[];
  highlightEdgeIds: string[];
  codeLineIndex: number;
  snapshot: TreeState;
}
```

`Step[]` is the universal contract between every lib/ implementation and the `StepModal`.

---

## Layout Algorithms

### Binary trees (BST, RBT)
**Algorithm:** Knuth in-order rank assignment  
- `x = inorder_index × HGAP` where `HGAP = 55 px`  
- `y = depth × VGAP` where `VGAP = 75 px`  
- Produces a non-overlapping layout in O(n) — simple, correct, and looks fine for ≤ 15 nodes  

### B-Tree
- BFS to enumerate levels  
- Each leaf node's width = `keys.length × 28 + 16` px  
- Parent x = average of its children's x  
- y = `level × VGAP`  

### Binomial Heap
- Each binomial tree of degree k gets its own sub-layout (recursive)  
- Trees placed side-by-side with 60 px horizontal gap  
- Within a tree: parent at top, children spread below with degree-proportional spacing  

---

## Step Generation Strategy

Each lib class's `insert(key)`, `delete(key)`, and `search(key)` methods:
1. Traverse the tree, recording one `Step` per decision point
2. Steps during traversal snapshot the **pre-mutation** tree with the current node highlighted
3. The final step after mutation snapshots the **post-mutation** tree with the new/deleted node highlighted
4. `codeLineIndex` matches line indices in the `codeLines` array stored in `src/data/<structure>Info.ts`

This means intermediate steps show "where we are in the algorithm" and the last step reveals the structural change — pedagogically clear.

### RBT exception
RBT rotations and recolors mutate the tree mid-operation. Each rotation generates its own step by deep-cloning the tree at that moment. This is more expensive but necessary for correctness.

---

## State & Hook Pattern

Each structure page uses a dedicated hook (`useBST`, `useRBT`, `useBTree`, `useBinomialHeap`):

```ts
const bstRef = useRef(new BST());           // mutable DS instance survives renders
const [treeState, setTreeState] = useState(...);  // serialized for React rendering
const [steps, setSteps] = useState<Step[]>([]);
const [modalOpen, setModalOpen] = useState(false);

const insert = useCallback((key: number) => {
  const generatedSteps = bstRef.current.insert(key);
  setTreeState(bstRef.current.getState());
  setSteps(generatedSteps);
  setModalOpen(true);
}, []);
```

Using `useRef` for the DS instance avoids the "double-update" problem with `useState` for mutable objects while keeping React state in sync via explicit `setTreeState` calls.

---

## Styling

- **Tailwind v4** utility classes throughout components  
- **Theme**: dark-only (`bg-gray-950` / `bg-gray-900` base)  
- **Accent**: indigo-500 / indigo-600 for primary actions  
- **Node colors**:
  - Default: `fill-indigo-500` stroke `indigo-600`
  - RBT red: `fill-red-500`
  - RBT black: `fill-gray-700`
  - Highlighted: `fill-yellow-400`
  - Found: `fill-green-500`
  - New node: `fill-emerald-500`

---

## File Generation Plan

```
src/
├── types.ts                         ← shared types (Step, TreeState, NodeData, …)
├── App.tsx                          ← BrowserRouter + Routes
├── main.tsx                         ← unchanged
├── index.css                        ← @import "tailwindcss" + resets
├── pages/
│   ├── DashboardPage.tsx
│   ├── BSTPage.tsx
│   ├── RBTPage.tsx
│   ├── BTreePage.tsx
│   └── BinomialHeapPage.tsx
├── components/
│   ├── Layout.tsx                   ← Sidebar + main wrapper
│   ├── Sidebar.tsx                  ← nav links
│   ├── PageHeader.tsx               ← title + badges
│   ├── InfoPanel.tsx                ← tabbed Notes/UseCases/FunFacts
│   ├── ComplexityTable.tsx          ← complexity grid
│   ├── ImplPanel.tsx                ← code snippet + GitHub link
│   ├── viz/
│   │   ├── TreeCanvas.tsx           ← SVG viewport
│   │   ├── TreeNode.tsx             ← circle + label
│   │   ├── HeapForest.tsx           ← heap-specific SVG wrapper
│   │   └── ControlBar.tsx           ← insert/delete/search/reset
│   └── modal/
│       ├── StepModal.tsx            ← portal overlay
│       ├── StepCanvas.tsx           ← snapshot SVG with highlights
│       ├── StepCode.tsx             ← syntax-highlighted code
│       ├── StepLog.tsx              ← step description text
│       └── StepControls.tsx         ← prev/next/skip/auto-play
├── hooks/
│   ├── useBST.ts
│   ├── useRBT.ts
│   ├── useBTree.ts
│   └── useBinomialHeap.ts
├── lib/
│   ├── bst.ts
│   ├── rbt.ts
│   ├── btree.ts
│   ├── binomialHeap.ts
│   └── stepBuilder.ts              ← shared step helpers
└── data/
    ├── bstInfo.ts
    ├── rbtInfo.ts
    ├── btreeInfo.ts
    └── binomialHeapInfo.ts
```

---

## Open Questions / Future Work

- **Pan/zoom** on SVG canvas: deferred to polish pass; current viewport uses `viewBox` auto-fitting
- **Traversal orders** (in-order, pre-order, post-order): noted in plan.md for BST but deferred
- **RBT delete fixup**: full delete with double-black fixup is complex; initial implementation covers insert with fixup + delete by key only
- **GitHub links**: placeholder URLs in data files; replace when repo is public
