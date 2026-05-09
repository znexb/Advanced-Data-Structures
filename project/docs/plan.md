# Data Structures Dashboard — Project Plan

## Overview

A React web application presenting a visual, interactive dashboard for the four core data structures covered in the course: **Binary Search Trees**, **Red-Black Trees**, **B-Trees**, and **Binomial Heaps**. Each structure gets its own dedicated page with animated step-by-step visualization modals for insert and delete operations, alongside implementation notes, use cases, and fun facts.

---

## Goals

- Satisfy the two-data-structure minimum (covering four)
- Provide a visually compelling demo suitable for a class presentation
- Include personal implementations linked from GitHub
- Stay within the 2–4 minute oral presentation window with a clear demo flow

---

## Tech Stack

| Layer | Choice | Reason |
|---|---|---|
| Framework | React (Vite) | Fast dev, component model, artifact-friendly |
| Styling | Tailwind CSS | Utility classes, no build overhead |
| Animation | Framer Motion | Smooth tree/heap transitions |
| Routing | React Router v6 | One page per data structure |
| Icons | Lucide React | Clean, consistent iconography |
| Visualization | Custom SVG rendering | Full control over node/edge layout |

---

## App Structure

```
/                        → Dashboard home (overview of all 4 structures)
/bst                     → Binary Search Tree page
/rbt                     → Red-Black Tree page
/btree                   → B-Tree page
/binomial-heap           → Binomial Heap page
```

Each route contains:
- **Info panel** — description, time complexities, use cases, fun facts
- **Interactive canvas** — live tree/heap rendered in SVG
- **Control bar** — insert key, delete key, reset, random fill
- **Step modal** — triggered on any operation; animates each algorithmic step

---

## Pages & Features

### 1. Dashboard Home
- Grid cards for each data structure
- Quick-stat badges (e.g., "O(log n) search", "Self-balancing")
- "Jump in" buttons per structure

### 2. Binary Search Tree (`/bst`)
- Insert, search, delete with animated node traversal
- Highlight path taken during search
- In-order / pre-order / post-order traversal visualizer
- Notes: simplest structure, foundation for the others

### 3. Red-Black Tree (`/rbt`)
- Color-coded nodes (red/black)
- Step modal shows rotations and recoloring cases
- Invariant checker panel (live validation display)

### 4. B-Tree (`/btree`)
- Configurable order (t = 2, 3, or 4)
- Node splitting animation on overflow
- Visual distinction between internal and leaf nodes

### 5. Binomial Heap (`/binomial-heap`)
- Heap forest visualization (multiple binomial trees)
- Merge, insert, extract-min animated
- Degree labels on each tree root

---

## Visualization Modal Design

Each modal walks through the operation step-by-step:

```
[ Step 1 of 5 ] Inserting key: 42
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  → Compare 42 with root (30): go RIGHT
  [ animated arrow highlights right child ]
[ Prev ] [ Next ] [ Skip to End ]
```

- Auto-play mode with configurable speed slider
- Code snippet panel alongside tree view (highlights current line)

---

## Implementation Section

Each page links to the GitHub repo with the raw implementation:
- Language: [choose — Python / C++ / Java / TypeScript]
- Embed a syntax-highlighted code snippet for the core struct/class
- "View full implementation →" button

---

## Documentation (1–2 pages, separate MD or PDF)

Sections:
1. What the application does
2. Data structures covered and why they were chosen
3. How each structure is implemented
4. How to run the application locally
5. Key algorithmic operations demonstrated

---

## Presentation Flow (2–4 min)

| Time | Content |
|---|---|
| 0:00–0:30 | Dashboard overview, explain the concept |
| 0:30–1:15 | BST live demo — insert + search + delete |
| 1:15–2:00 | RBT demo — show rebalancing via modal |
| 2:00–2:40 | B-Tree or Binomial Heap (pick one to go deep) |
| 2:40–3:00 | Show GitHub implementation, close |

---

## Milestones

### Week 1 — Foundation + BST + RBT
- [ ] Scaffold Vite + React + Router + Tailwind
- [ ] Reusable SVG tree renderer + step modal component
- [ ] BST: insert, delete, search logic + modal
- [ ] RBT: rotation/recolor logic + color-coded nodes + modal

### Week 2 — B-Tree + Binomial Heap + Dashboard
- [ ] B-Tree: split logic, configurable order (t = 2/3/4), modal
- [ ] Binomial Heap: forest renderer, merge + extract-min, modal
- [ ] Dashboard home: overview cards, quick stats, navigation
- [ ] GitHub implementation links + code snippet panels

### Week 3 — Polish + Docs + Presentation
- [ ] Info panels: use cases, fun facts, complexity tables per page
- [ ] UI polish: animations, transitions, responsive layout
- [ ] Write 1–2 page documentation
- [ ] Dry-run presentation (time it), fix any rough edges
- [ ] Present 🎉

---

## Grading Targets

| Requirement | Status |
|---|---|
| ≥ 2 data structures implemented | ✅ (4 planned) |
| Visual representation | ✅ SVG + animated modals |
| 1–2 page documentation | ⬜ Write after Week 6 |
| 2–4 min oral presentation | ⬜ Rehearse Week 7 |
| Explain how structures work | ✅ Step modals + info panels |

---

## Notes

- Keep the demo data small (≤ 15 keys) so the tree fits on screen during presentation
- Prepare a "happy path" preset for each structure so the live demo never breaks
- The step modal is the biggest differentiator — prioritize its quality
