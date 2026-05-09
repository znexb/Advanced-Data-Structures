import type { NodeData, EdgeData, TreeState, Step } from '../types';

let bhIdCounter = 0;
function nextId() { return `h${++bhIdCounter}`; }

export interface BinomialNode {
  id: string;
  key: number;
  degree: number;
  children: BinomialNode[];
}

// Layout: place each binomial tree left to right, then lay out each tree's nodes
const HGAP = 50;
const VGAP = 70;

function layoutTree(root: BinomialNode, startX: number, depth: number, pos: Map<string, { x: number; y: number }>) {
  function place(n: BinomialNode, xBase: number, d: number): number {
    const subtreeWidth = n.children.reduce((sum, c) => {
      return sum + Math.pow(2, c.degree);
    }, 1);
    pos.set(n.id, { x: xBase + (subtreeWidth - 1) * HGAP / 2, y: d * VGAP + 40 });
    let cx = xBase;
    for (const child of n.children) {
      const cw = Math.max(1, Math.pow(2, child.degree));
      place(child, cx, d + 1);
      cx += cw * HGAP;
    }
    return subtreeWidth * HGAP;
  }
  place(root, startX, depth);
}

function serializeForest(trees: BinomialNode[], highlightNodeIds: string[] = [], newNodeId?: string): TreeState {
  const pos = new Map<string, { x: number; y: number }>();
  let x = 40;
  for (const tree of trees) {
    const w = Math.max(1, Math.pow(2, tree.degree)) * HGAP;
    layoutTree(tree, x, 0, pos);
    x += w + 30;
  }

  const nodes: NodeData[] = [];
  const edges: EdgeData[] = [];

  function walk(n: BinomialNode) {
    const p = pos.get(n.id) ?? { x: 0, y: 0 };
    let color: NodeData['color'] = 'default';
    if (n.id === newNodeId) color = 'new';
    else if (highlightNodeIds.includes(n.id)) color = 'highlighted';
    nodes.push({ id: n.id, label: String(n.key), x: p.x, y: p.y, color });
    for (const child of n.children) {
      edges.push({ id: `e-${n.id}-${child.id}`, fromId: n.id, toId: child.id });
      walk(child);
    }
  }
  for (const tree of trees) walk(tree);

  const maxX = nodes.reduce((m, n) => Math.max(m, n.x), 0);
  const maxY = nodes.reduce((m, n) => Math.max(m, n.y), 0);
  return { nodes, edges, width: maxX + 60, height: maxY + 60 };
}


export const BHEAP_CODE_LINES = [
  'function insert(heap, key):',
  '  h = new BinomialHeap(key)',
  '  heap = union(heap, h)',
  '─── union ────────────────────────',
  'function union(h1, h2):',
  '  merged = mergeLists(h1, h2)',
  '  carry = null',
  '  for each tree in merged:',
  '    if degree(tree) != degree(carry):',
  '      carry = tree',
  '    else:',
  '      carry = link(carry, tree)',
  '─── link (degree k trees) ────────',
  'function link(y, z):',
  '  if y.key > z.key: swap(y, z)',
  '  y.children.prepend(z)',
  '  y.degree++',
  '─── extractMin ───────────────────',
  'function extractMin(heap):',
  '  min = tree with minimum root',
  '  heap.remove(min)',
  '  heap = union(heap, reverse(min.children))',
];

function makeNode(key: number): BinomialNode {
  return { id: nextId(), key, degree: 0, children: [] };
}

function linkTrees(y: BinomialNode, z: BinomialNode): BinomialNode {
  if (y.key > z.key) [y, z] = [z, y];
  z.id = z.id; // keep id
  y.children = [z, ...y.children];
  y.degree++;
  return y;
}

export class BinomialHeap {
  trees: BinomialNode[] = [];

  private unionWith(other: BinomialNode[]): void {
    const merged: BinomialNode[] = [];
    let i = 0, j = 0;
    while (i < this.trees.length && j < other.length) {
      if (this.trees[i].degree <= other[j].degree) merged.push(this.trees[i++]);
      else merged.push(other[j++]);
    }
    while (i < this.trees.length) merged.push(this.trees[i++]);
    while (j < other.length) merged.push(other[j++]);

    if (merged.length === 0) { this.trees = []; return; }

    const result: BinomialNode[] = [];
    let carry: BinomialNode | null = null;
    for (const tree of merged) {
      if (!carry) {
        if (result.length > 0 && result[result.length - 1].degree === tree.degree) {
          carry = linkTrees(result.pop()!, tree);
        } else {
          result.push(tree);
        }
      } else {
        if (carry.degree === tree.degree) {
          carry = linkTrees(carry, tree);
        } else {
          result.push(carry);
          carry = null;
          if (result.length > 0 && result[result.length - 1].degree === tree.degree) {
            carry = linkTrees(result.pop()!, tree);
          } else {
            result.push(tree);
          }
        }
      }
    }
    if (carry) result.push(carry);
    this.trees = result;
  }

  insert(key: number): Step[] {
    const steps: Step[] = [];
    const node = makeNode(key);

    steps.push({
      description: `Insert key ${key}. Create single-node B₀ heap, then union with existing heap.`,
      highlightNodeIds: [],
      highlightEdgeIds: [],
      codeLineIndex: 0,
      snapshot: serializeForest(this.trees),
    });

    this.unionWith([node]);

    steps.push({
      description: `After union: heap consolidated. Nodes with same degree get linked.`,
      highlightNodeIds: [node.id],
      highlightEdgeIds: [],
      codeLineIndex: 2,
      snapshot: serializeForest(this.trees, [node.id], node.id),
    });

    return steps;
  }

  extractMin(): Step[] {
    const steps: Step[] = [];
    if (this.trees.length === 0) {
      steps.push({ description: 'Heap is empty.', highlightNodeIds: [], highlightEdgeIds: [], codeLineIndex: 18, snapshot: serializeForest([]) });
      return steps;
    }

    let minIdx = 0;
    for (let i = 1; i < this.trees.length; i++) {
      if (this.trees[i].key < this.trees[minIdx].key) minIdx = i;
    }
    const minTree = this.trees[minIdx];

    steps.push({
      description: `Minimum is ${minTree.key} (root of B${minTree.degree} tree). Remove it.`,
      highlightNodeIds: [minTree.id],
      highlightEdgeIds: [],
      codeLineIndex: 19,
      snapshot: serializeForest(this.trees, [minTree.id]),
    });

    this.trees.splice(minIdx, 1);
    const children = [...minTree.children].reverse();

    steps.push({
      description: `Merge ${minTree.key}'s children (reversed) back into the heap.`,
      highlightNodeIds: children.map(c => c.id),
      highlightEdgeIds: [],
      codeLineIndex: 21,
      snapshot: serializeForest([...this.trees, ...children], children.map(c => c.id)),
    });

    this.unionWith(children);

    steps.push({
      description: `Extracted minimum ${minTree.key}. Heap consolidated.`,
      highlightNodeIds: [],
      highlightEdgeIds: [],
      codeLineIndex: 21,
      snapshot: serializeForest(this.trees),
    });

    return steps;
  }

  reset() {
    this.trees = [];
    bhIdCounter = 0;
  }

  randomFill(count = 7) {
    this.reset();
    const pool: number[] = [];
    while (pool.length < count) {
      const v = Math.floor(Math.random() * 90) + 10;
      if (!pool.includes(v)) pool.push(v);
    }
    for (const k of pool) {
      const node = makeNode(k);
      this.unionWith([node]);
    }
  }

  getState(): TreeState {
    return serializeForest(this.trees);
  }
}
