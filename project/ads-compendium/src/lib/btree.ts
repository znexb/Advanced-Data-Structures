import type { NodeData, EdgeData, TreeState, Step } from '../types';

let btIdCounter = 0;
function nextId() { return `bt${++btIdCounter}`; }

export interface BTreeNode {
  id: string;
  keys: number[];
  children: BTreeNode[];
  leaf: boolean;
}

const KEY_W = 28;
const KEY_PAD = 16;
const VGAP = 90;
const NODE_H = 32;

function nodeWidth(n: BTreeNode): number {
  return n.keys.length * KEY_W + KEY_PAD;
}

interface Rect { x: number; y: number; w: number; }

function assignLayout(root: BTreeNode | null): Map<string, Rect> {
  const pos = new Map<string, Rect>();
  if (!root) return pos;

  function computeSubtreeWidth(n: BTreeNode): number {
    if (n.leaf) return nodeWidth(n);
    const childrenW = n.children.reduce((s, c) => s + computeSubtreeWidth(c), 0) + (n.children.length - 1) * 10;
    return Math.max(nodeWidth(n), childrenW);
  }

  function place(n: BTreeNode, xCenter: number, depth: number) {
    const w = nodeWidth(n);
    pos.set(n.id, { x: xCenter - w / 2, y: depth * VGAP + 40, w });
    if (!n.leaf) {
      const childWidths = n.children.map(c => computeSubtreeWidth(c));
      const totalW = childWidths.reduce((s, w) => s + w, 0) + (n.children.length - 1) * 10;
      let cx = xCenter - totalW / 2;
      for (let i = 0; i < n.children.length; i++) {
        const cw = childWidths[i];
        place(n.children[i], cx + cw / 2, depth + 1);
        cx += cw + 10;
      }
    }
  }

  place(root, 400, 0);
  return pos;
}

function serialize(root: BTreeNode | null, highlightNodeIds: string[] = [], newNodeId?: string): TreeState {
  if (!root) return { nodes: [], edges: [], width: 100, height: 100 };
  const pos = assignLayout(root);
  const nodes: NodeData[] = [];
  const edges: EdgeData[] = [];

  function walk(n: BTreeNode) {
    const r = pos.get(n.id)!;
    const w = nodeWidth(n);
    let color: NodeData['color'] = 'default';
    if (n.id === newNodeId) color = 'new';
    else if (highlightNodeIds.includes(n.id)) color = 'highlighted';
    nodes.push({ id: n.id, label: n.keys.join(' | '), x: r.x + w / 2, y: r.y, color, width: w });
    for (const child of n.children) {
      edges.push({ id: `e-${n.id}-${child.id}`, fromId: n.id, toId: child.id });
      walk(child);
    }
  }
  walk(root);

  const maxX = nodes.reduce((m, n) => Math.max(m, n.x + (n.width ?? 0) / 2), 0);
  const maxY = nodes.reduce((m, n) => Math.max(m, n.y), 0);
  return { nodes, edges, width: maxX + 40, height: maxY + NODE_H + 40 };
}

export const BTREE_CODE_LINES = [
  'function insertNonFull(node, key):',
  '  i = node.keys.length - 1',
  '  if node.isLeaf:',
  '    shift keys right, insert key',
  '  else:',
  '    find child[i] where key goes',
  '    if child[i] is full: splitChild(node, i)',
  '    insertNonFull(child[i], key)',
  '─── split ────────────────────────',
  'function splitChild(parent, i):',
  '  y = parent.children[i]  // full node',
  '  z = new BTreeNode()',
  '  z.keys = y.keys[t:]',
  '  y.keys = y.keys[:t-1]',
  '  mid = y.keys[t-1]  // median',
  '  parent.keys.insert(i, mid)',
  '  parent.children.insert(i+1, z)',
];

function makeNode(leaf: boolean): BTreeNode {
  return { id: nextId(), keys: [], children: [], leaf };
}

export class BTree {
  root: BTreeNode | null = null;
  t: number;

  constructor(t = 2) {
    this.t = t;
    this.root = makeNode(true);
  }

  private splitChild(parent: BTreeNode, i: number, steps: Step[]) {
    const t = this.t;
    const y = parent.children[i];
    const z = makeNode(y.leaf);
    z.keys = y.keys.splice(t - 1);
    const mid = z.keys.shift()!;
    if (!y.leaf) z.children = y.children.splice(t);

    parent.keys.splice(i, 0, mid);
    parent.children.splice(i + 1, 0, z);

    steps.push({
      description: `Node full. Split child: median key ${mid} moves up to parent.`,
      highlightNodeIds: [parent.id, y.id, z.id],
      highlightEdgeIds: [],
      codeLineIndex: 9,
      snapshot: serialize(this.root, [parent.id, y.id, z.id]),
    });
  }

  private insertNonFull(node: BTreeNode, key: number, steps: Step[]) {
    let i = node.keys.length - 1;
    if (node.leaf) {
      steps.push({
        description: `In leaf node. Insert ${key} in sorted order.`,
        highlightNodeIds: [node.id],
        highlightEdgeIds: [],
        codeLineIndex: 2,
        snapshot: serialize(this.root, [node.id]),
      });
      while (i >= 0 && key < node.keys[i]) i--;
      node.keys.splice(i + 1, 0, key);
      steps.push({
        description: `Inserted ${key} into leaf.`,
        highlightNodeIds: [node.id],
        highlightEdgeIds: [],
        codeLineIndex: 3,
        snapshot: serialize(this.root, [], node.id),
      });
    } else {
      while (i >= 0 && key < node.keys[i]) i--;
      i++;
      steps.push({
        description: `Internal node. Key ${key} goes into child[${i}].`,
        highlightNodeIds: [node.id],
        highlightEdgeIds: [],
        codeLineIndex: 5,
        snapshot: serialize(this.root, [node.id]),
      });
      if (node.children[i].keys.length === 2 * this.t - 1) {
        this.splitChild(node, i, steps);
        if (key > node.keys[i]) i++;
      }
      this.insertNonFull(node.children[i], key, steps);
    }
  }

  insert(key: number): Step[] {
    const steps: Step[] = [];
    steps.push({
      description: `Insert key ${key} into B-Tree (min degree t = ${this.t}).`,
      highlightNodeIds: [],
      highlightEdgeIds: [],
      codeLineIndex: 0,
      snapshot: serialize(this.root),
    });

    const r = this.root!;
    if (r.keys.length === 2 * this.t - 1) {
      const s = makeNode(false);
      s.children.push(r);
      this.root = s;
      steps.push({
        description: `Root is full. Split root and create new root.`,
        highlightNodeIds: [r.id],
        highlightEdgeIds: [],
        codeLineIndex: 9,
        snapshot: serialize(this.root, [r.id]),
      });
      this.splitChild(s, 0, steps);
      this.insertNonFull(s, key, steps);
    } else {
      this.insertNonFull(r, key, steps);
    }
    return steps;
  }

  search(key: number): Step[] {
    const steps: Step[] = [];
    steps.push({ description: `Search for key ${key}.`, highlightNodeIds: [], highlightEdgeIds: [], codeLineIndex: 0, snapshot: serialize(this.root) });

    function searchNode(n: BTreeNode | null): void {
      if (!n) return;
      steps.push({ description: `Visit node [${n.keys.join(', ')}].`, highlightNodeIds: [n.id], highlightEdgeIds: [], codeLineIndex: 0, snapshot: serialize(n, [n.id]) });
      let i = 0;
      while (i < n.keys.length && key > n.keys[i]) i++;
      if (i < n.keys.length && key === n.keys[i]) {
        steps.push({ description: `Found ${key} at position ${i}.`, highlightNodeIds: [n.id], highlightEdgeIds: [], codeLineIndex: 0, snapshot: serialize(n, [n.id]) });
        return;
      }
      if (n.leaf) { steps.push({ description: `${key} not found.`, highlightNodeIds: [], highlightEdgeIds: [], codeLineIndex: 0, snapshot: serialize(n) }); return; }
      searchNode(n.children[i]);
    }

    searchNode(this.root);
    return steps;
  }

  reset(t?: number) {
    if (t !== undefined) this.t = t;
    btIdCounter = 0;
    this.root = makeNode(true);
  }

  randomFill(count = 7) {
    this.reset();
    const pool: number[] = [];
    while (pool.length < count) {
      const v = Math.floor(Math.random() * 90) + 10;
      if (!pool.includes(v)) pool.push(v);
    }
    for (const k of pool) {
      const steps: Step[] = [];
      const r = this.root!;
      if (r.keys.length === 2 * this.t - 1) {
        const s = makeNode(false);
        s.children.push(r);
        this.root = s;
        this.splitChild(s, 0, steps);
        this.insertNonFull(s, k, steps);
      } else {
        this.insertNonFull(r, k, steps);
      }
    }
  }

  getState(): TreeState {
    return serialize(this.root);
  }
}
