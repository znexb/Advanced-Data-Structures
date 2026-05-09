# Architecture Map

## Route Tree

```
/                          Dashboard (overview of all 4 structures)
├── /bst                   Binary Search Tree
├── /rbt                   Red-Black Tree
├── /btree                 B-Tree
└── /binomial-heap         Binomial Heap
```

---

## Page Anatomy

Every structure page (`/bst`, `/rbt`, `/btree`, `/binomial-heap`) shares the same layout shell with structure-specific logic injected via props/hooks.

```
<Layout>
  ├── <Sidebar>                  ← nav links to all 4 structures + home
  └── <PageContent>
        ├── <PageHeader>         ← title, subtitle, complexity badge row
        ├── <CanvasPanel>        ← SVG visualization (tree or heap forest)
        │     └── <ControlBar>  ← insert / delete / search / reset / random fill
        ├── <InfoPanel>          ← tabbed: Notes | Use Cases | Fun Facts | Complexity
        ├── <ImplPanel>          ← syntax-highlighted snippet + GitHub link
        └── <StepModal>          ← portal; triggered by any canvas operation
              ├── <StepHeader>   ← "Step N of M — Inserting key 42"
              ├── <StepCanvas>   ← frozen SVG snapshot with highlight overlay
              ├── <StepCode>     ← code snippet, current line highlighted
              ├── <StepLog>      ← plain-English description of this step
              └── <StepControls> ← Prev | Next | Skip | Speed slider | Auto-play
```

---

## Component Inventory

### Shell & Navigation

| Component | Path | Responsibility |
|---|---|---|
| `App` | `src/App.tsx` | Router setup, global providers |
| `Layout` | `src/components/Layout.tsx` | Sidebar + main content wrapper |
| `Sidebar` | `src/components/Sidebar.tsx` | Nav links, active route highlight |
| `PageHeader` | `src/components/PageHeader.tsx` | Title, subtitle, complexity badges |

### Shared Visualization

| Component | Path | Responsibility |
|---|---|---|
| `TreeCanvas` | `src/components/viz/TreeCanvas.tsx` | SVG viewport, pan/zoom, node/edge render |
| `TreeNode` | `src/components/viz/TreeNode.tsx` | Single node circle + label (color-aware for RBT) |
| `HeapForest` | `src/components/viz/HeapForest.tsx` | Row of binomial trees for heap page |
| `ControlBar` | `src/components/viz/ControlBar.tsx` | Insert / delete / search inputs + action buttons |

### Step Modal

| Component | Path | Responsibility |
|---|---|---|
| `StepModal` | `src/components/modal/StepModal.tsx` | Portal wrapper, open/close state |
| `StepCanvas` | `src/components/modal/StepCanvas.tsx` | Snapshot SVG with animated highlight overlay |
| `StepCode` | `src/components/modal/StepCode.tsx` | Syntax-highlighted code, active line marker |
| `StepLog` | `src/components/modal/StepLog.tsx` | Human-readable step description |
| `StepControls` | `src/components/modal/StepControls.tsx` | Prev/Next/Skip, auto-play, speed slider |

### Info & Implementation Panels

| Component | Path | Responsibility |
|---|---|---|
| `InfoPanel` | `src/components/InfoPanel.tsx` | Tabbed panel: Notes, Use Cases, Fun Facts |
| `ComplexityTable` | `src/components/ComplexityTable.tsx` | Time/space complexity rows |
| `ImplPanel` | `src/components/ImplPanel.tsx` | Code snippet + GitHub link button |

### Pages

| Component | Path | Responsibility |
|---|---|---|
| `DashboardPage` | `src/pages/DashboardPage.tsx` | Overview cards for all 4 structures |
| `BSTPage` | `src/pages/BSTPage.tsx` | BST state + wires TreeCanvas + StepModal |
| `RBTPage` | `src/pages/RBTPage.tsx` | RBT state, color logic, invariant panel |
| `BTreePage` | `src/pages/BTreePage.tsx` | B-Tree state, order selector |
| `BinomialHeapPage` | `src/pages/BinomialHeapPage.tsx` | Heap forest state, merge/extract |

---

## State & Data Flow

Each structure page owns its own state via a dedicated hook. The hook holds the data structure instance, the current step sequence, and modal open/close state. Nothing is shared globally except the router.

```
BSTPage
  └── useBST()
        ├── tree: BSTNode | null          ← live data structure
        ├── steps: Step[]                 ← generated on each operation
        ├── currentStep: number
        ├── modalOpen: boolean
        └── actions: { insert, delete, search, reset, randomFill }
```

`Step` is the universal unit passed into `StepModal`:

```ts
type Step = {
  description: string;       // "Compare 42 with root (30): go RIGHT"
  highlightNodeIds: string[]; // nodes to pulse/color in StepCanvas
  highlightEdgeIds: string[]; // edges to highlight
  codeLineIndex: number;      // line to highlight in StepCode
  snapshot: TreeState;        // full immutable tree state at this moment
}
```

The same `Step[]` shape is used by every structure — BST, RBT, B-Tree, and Binomial Heap all feed into the identical `StepModal`.

---

## File Structure

```
src/
├── App.tsx
├── main.tsx
├── pages/
│   ├── DashboardPage.tsx
│   ├── BSTPage.tsx
│   ├── RBTPage.tsx
│   ├── BTreePage.tsx
│   └── BinomialHeapPage.tsx
├── components/
│   ├── Layout.tsx
│   ├── Sidebar.tsx
│   ├── PageHeader.tsx
│   ├── InfoPanel.tsx
│   ├── ComplexityTable.tsx
│   ├── ImplPanel.tsx
│   ├── viz/
│   │   ├── TreeCanvas.tsx
│   │   ├── TreeNode.tsx
│   │   ├── HeapForest.tsx
│   │   └── ControlBar.tsx
│   └── modal/
│       ├── StepModal.tsx
│       ├── StepCanvas.tsx
│       ├── StepCode.tsx
│       ├── StepLog.tsx
│       └── StepControls.tsx
├── hooks/
│   ├── useBST.ts
│   ├── useRBT.ts
│   ├── useBTree.ts
│   └── useBinomialHeap.ts
├── lib/
│   ├── bst.ts               ← BST implementation
│   ├── rbt.ts               ← RBT implementation
│   ├── btree.ts             ← B-Tree implementation
│   ├── binomialHeap.ts      ← Binomial Heap implementation
│   └── stepBuilder.ts       ← generates Step[] arrays from operations
└── data/
    ├── bstInfo.ts           ← notes, use cases, fun facts, code snippet
    ├── rbtInfo.ts
    ├── btreeInfo.ts
    └── binomialHeapInfo.ts
```

---

## Dashboard Page Layout

```
┌─────────────────────────────────────────────────────┐
│  Data Structures Explorer                           │
│  An interactive visual reference                    │
├───────────┬───────────┬───────────┬─────────────────┤
│    BST    │    RBT    │  B-Tree   │  Binomial Heap  │
│  O(log n) │ Balanced  │ Disk I/O  │  Priority queue │
│           │           │           │                 │
│  [ Open ] │  [ Open ] │  [ Open ] │    [ Open ]     │
└───────────┴───────────┴───────────┴─────────────────┘
```

---

## Structure Page Layout

```
┌──────┬──────────────────────────────────────────────┐
│      │  Binary Search Tree        O(log n) avg      │
│  S   ├──────────────────────────────┬───────────────┤
│  I   │                              │  Notes        │
│  D   │      SVG Canvas              │  Use Cases    │
│  E   │      (tree here)             │  Fun Facts    │
│  B   │                              │  Complexity   │
│  A   ├──────────────────────────────┴───────────────┤
│  R   │  Insert: [___]  Delete: [___]  [Reset] [Rnd] │
│      ├──────────────────────────────────────────────┤
│      │  Implementation  [TypeScript]  → GitHub ↗    │
│      │  ```ts  class BSTNode { ... }  ```           │
└──────┴──────────────────────────────────────────────┘
```

When an operation is triggered → `StepModal` opens as a full overlay:

```
┌─────────────────────────────────────────────────────┐
│  Step 2 of 6  —  Inserting key: 42                 │
├─────────────────────────┬───────────────────────────┤
│                         │  insert(node, key) {      │
│   SVG snapshot          │    if key < node.val      │  ← highlighted
│   (node 30 pulsing)     │      go left              │
│                         │    else go right          │
├─────────────────────────┴───────────────────────────┤
│  Compare 42 with node 30: 42 > 30, traverse RIGHT  │
├─────────────────────────────────────────────────────┤
│  [← Prev]  [Next →]  [Skip to end]  Speed: ●───    │
└─────────────────────────────────────────────────────┘
```
