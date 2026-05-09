import type { NodeData, EdgeData, TreeState, Step } from '../types';

let idCounter = 0;
function nextId() { return `b${++idCounter}`; }

export interface BSTNode {
  id: string;
  key: number;
  left: BSTNode | null;
  right: BSTNode | null;
}

const HGAP = 55;
const VGAP = 75;

function assignLayout(root: BSTNode | null): Map<string, { x: number; y: number }> {
  const pos = new Map<string, { x: number; y: number }>();
  let idx = 0;
  function inorder(n: BSTNode | null) {
    if (!n) return;
    inorder(n.left);
    pos.set(n.id, { x: idx * HGAP + 40, y: 0 });
    idx++;
    inorder(n.right);
  }
  function setDepth(n: BSTNode | null, d: number) {
    if (!n) return;
    pos.get(n.id)!.y = d * VGAP + 40;
    setDepth(n.left, d + 1);
    setDepth(n.right, d + 1);
  }
  inorder(root);
  setDepth(root, 0);
  return pos;
}

function serialize(
  root: BSTNode | null,
  highlightNodeIds: string[] = [],
  highlightEdgeIds: string[] = [],
  newNodeId?: string,
): TreeState {
  const pos = assignLayout(root);
  const nodes: NodeData[] = [];
  const edges: EdgeData[] = [];

  function walk(n: BSTNode | null) {
    if (!n) return;
    const p = pos.get(n.id) ?? { x: 0, y: 0 };
    let color: NodeData['color'] = 'default';
    if (n.id === newNodeId) color = 'new';
    else if (highlightNodeIds.includes(n.id)) color = 'highlighted';
    nodes.push({ id: n.id, label: String(n.key), x: p.x, y: p.y, color });
    if (n.left) {
      const eid = `e-${n.id}-${n.left.id}`;
      edges.push({ id: eid, fromId: n.id, toId: n.left.id, highlighted: highlightEdgeIds.includes(eid) });
      walk(n.left);
    }
    if (n.right) {
      const eid = `e-${n.id}-${n.right.id}`;
      edges.push({ id: eid, fromId: n.id, toId: n.right.id, highlighted: highlightEdgeIds.includes(eid) });
      walk(n.right);
    }
  }
  walk(root);

  const maxX = nodes.reduce((m, n) => Math.max(m, n.x), 0);
  const maxY = nodes.reduce((m, n) => Math.max(m, n.y), 0);
  return { nodes, edges, width: maxX + 60, height: maxY + 60 };
}

export const BST_CODE_LINES = [
  'function insert(root, key):',
  '  if root == null: return new Node(key)',
  '  if key < root.key:',
  '    root.left = insert(root.left, key)',
  '  else if key > root.key:',
  '    root.right = insert(root.right, key)',
  '  return root   // duplicate: ignore',
  '─── delete ───────────────────────',
  'function delete(root, key):',
  '  if root == null: return null',
  '  if key < root.key: go left',
  '  if key > root.key: go right',
  '  // found: leaf → remove',
  '  // found: one child → replace',
  '  // found: two children → swap successor',
  '─── search ───────────────────────',
  'function search(root, key):',
  '  if root == null: return NOT FOUND',
  '  if key == root.key: return FOUND',
  '  if key < root.key: go left',
  '  if key > root.key: go right',
];

export class BST {
  root: BSTNode | null = null;

  insert(key: number): Step[] {
    const steps: Step[] = [];
    steps.push({
      description: `Start inserting key ${key}. Traverse from the root.`,
      highlightNodeIds: [],
      highlightEdgeIds: [],
      codeLineIndex: 0,
      snapshot: serialize(this.root),
    });

    let cur = this.root;
    let parent: BSTNode | null = null;
    let goLeft = false;

    while (cur) {
      parent = cur;
      if (key < cur.key) {
        steps.push({
          description: `${key} < ${cur.key} → go LEFT`,
          highlightNodeIds: [cur.id],
          highlightEdgeIds: [],
          codeLineIndex: 2,
          snapshot: serialize(this.root, [cur.id]),
        });
        goLeft = true;
        cur = cur.left;
      } else if (key > cur.key) {
        steps.push({
          description: `${key} > ${cur.key} → go RIGHT`,
          highlightNodeIds: [cur.id],
          highlightEdgeIds: [],
          codeLineIndex: 4,
          snapshot: serialize(this.root, [cur.id]),
        });
        goLeft = false;
        cur = cur.right;
      } else {
        steps.push({
          description: `Key ${key} already exists — ignoring duplicate.`,
          highlightNodeIds: [cur.id],
          highlightEdgeIds: [],
          codeLineIndex: 6,
          snapshot: serialize(this.root, [cur.id]),
        });
        return steps;
      }
    }

    const node: BSTNode = { id: nextId(), key, left: null, right: null };
    if (!parent) {
      this.root = node;
    } else if (goLeft) {
      parent.left = node;
    } else {
      parent.right = node;
    }

    steps.push({
      description: `Inserted ${key} as a new leaf node.`,
      highlightNodeIds: [node.id],
      highlightEdgeIds: [],
      codeLineIndex: 1,
      snapshot: serialize(this.root, [], [], node.id),
    });
    return steps;
  }

  search(key: number): Step[] {
    const steps: Step[] = [];
    steps.push({
      description: `Searching for key ${key}. Start at root.`,
      highlightNodeIds: [],
      highlightEdgeIds: [],
      codeLineIndex: 16,
      snapshot: serialize(this.root),
    });

    let cur = this.root;
    while (cur) {
      steps.push({
        description: `Visit node ${cur.key}. Compare ${key} with ${cur.key}.`,
        highlightNodeIds: [cur.id],
        highlightEdgeIds: [],
        codeLineIndex: 18,
        snapshot: serialize(this.root, [cur.id]),
      });
      if (key === cur.key) {
        steps.push({
          description: `Found! Key ${key} is here.`,
          highlightNodeIds: [cur.id],
          highlightEdgeIds: [],
          codeLineIndex: 18,
          snapshot: serialize(this.root, [cur.id]),
        });
        return steps;
      } else if (key < cur.key) {
        const eid = cur.left ? `e-${cur.id}-${cur.left.id}` : '';
        steps.push({
          description: `${key} < ${cur.key} → go LEFT`,
          highlightNodeIds: [cur.id],
          highlightEdgeIds: eid ? [eid] : [],
          codeLineIndex: 19,
          snapshot: serialize(this.root, [cur.id], eid ? [eid] : []),
        });
        cur = cur.left;
      } else {
        const eid = cur.right ? `e-${cur.id}-${cur.right.id}` : '';
        steps.push({
          description: `${key} > ${cur.key} → go RIGHT`,
          highlightNodeIds: [cur.id],
          highlightEdgeIds: eid ? [eid] : [],
          codeLineIndex: 20,
          snapshot: serialize(this.root, [cur.id], eid ? [eid] : []),
        });
        cur = cur.right;
      }
    }

    steps.push({
      description: `Key ${key} not found — reached null.`,
      highlightNodeIds: [],
      highlightEdgeIds: [],
      codeLineIndex: 17,
      snapshot: serialize(this.root),
    });
    return steps;
  }

  delete(key: number): Step[] {
    const steps: Step[] = [];
    steps.push({
      description: `Deleting key ${key}. Search for it first.`,
      highlightNodeIds: [],
      highlightEdgeIds: [],
      codeLineIndex: 8,
      snapshot: serialize(this.root),
    });

    let parent: BSTNode | null = null;
    let cur = this.root;
    let isLeft = false;

    while (cur && cur.key !== key) {
      parent = cur;
      if (key < cur.key) {
        steps.push({ description: `${key} < ${cur.key} → go LEFT`, highlightNodeIds: [cur.id], highlightEdgeIds: [], codeLineIndex: 10, snapshot: serialize(this.root, [cur.id]) });
        isLeft = true; cur = cur.left;
      } else {
        steps.push({ description: `${key} > ${cur.key} → go RIGHT`, highlightNodeIds: [cur.id], highlightEdgeIds: [], codeLineIndex: 11, snapshot: serialize(this.root, [cur.id]) });
        isLeft = false; cur = cur.right;
      }
    }

    if (!cur) {
      steps.push({ description: `Key ${key} not found.`, highlightNodeIds: [], highlightEdgeIds: [], codeLineIndex: 9, snapshot: serialize(this.root) });
      return steps;
    }

    steps.push({ description: `Found node ${key}. Determine case.`, highlightNodeIds: [cur.id], highlightEdgeIds: [], codeLineIndex: 12, snapshot: serialize(this.root, [cur.id]) });

    if (!cur.left && !cur.right) {
      steps.push({ description: `Case 1: leaf node. Simply remove it.`, highlightNodeIds: [cur.id], highlightEdgeIds: [], codeLineIndex: 12, snapshot: serialize(this.root, [cur.id]) });
      if (!parent) this.root = null;
      else if (isLeft) parent.left = null;
      else parent.right = null;
      steps.push({ description: `Node ${key} removed.`, highlightNodeIds: [], highlightEdgeIds: [], codeLineIndex: 12, snapshot: serialize(this.root) });
    } else if (!cur.left || !cur.right) {
      const child = (cur.left ?? cur.right)!;
      steps.push({ description: `Case 2: one child. Replace node with child ${child.key}.`, highlightNodeIds: [cur.id, child.id], highlightEdgeIds: [], codeLineIndex: 13, snapshot: serialize(this.root, [cur.id, child.id]) });
      if (!parent) this.root = child;
      else if (isLeft) parent.left = child;
      else parent.right = child;
      steps.push({ description: `Node ${key} replaced by ${child.key}.`, highlightNodeIds: [child.id], highlightEdgeIds: [], codeLineIndex: 13, snapshot: serialize(this.root, [child.id]) });
    } else {
      steps.push({ description: `Case 3: two children. Find in-order successor (min of right subtree).`, highlightNodeIds: [cur.id], highlightEdgeIds: [], codeLineIndex: 14, snapshot: serialize(this.root, [cur.id]) });
      let succParent = cur;
      let succ = cur.right!;
      while (succ.left) {
        steps.push({ description: `Going left to find minimum: at ${succ.key}`, highlightNodeIds: [succ.id], highlightEdgeIds: [], codeLineIndex: 14, snapshot: serialize(this.root, [succ.id]) });
        succParent = succ; succ = succ.left;
      }
      steps.push({ description: `In-order successor is ${succ.key}. Copy its key, then delete successor.`, highlightNodeIds: [cur.id, succ.id], highlightEdgeIds: [], codeLineIndex: 14, snapshot: serialize(this.root, [cur.id, succ.id]) });
      cur.key = succ.key;
      if (succParent === cur) succParent.right = succ.right;
      else succParent.left = succ.right;
      steps.push({ description: `Replacement done. Node now holds key ${cur.key}.`, highlightNodeIds: [cur.id], highlightEdgeIds: [], codeLineIndex: 14, snapshot: serialize(this.root, [cur.id]) });
    }
    return steps;
  }

  reset() {
    this.root = null;
    idCounter = 0;
  }

  randomFill(count = 8) {
    this.reset();
    const pool = Array.from({ length: 90 }, (_, i) => i + 10);
    for (let i = pool.length - 1; i > 0; i--) {
      const j = Math.floor(Math.random() * (i + 1));
      [pool[i], pool[j]] = [pool[j], pool[i]];
    }
    for (const k of pool.slice(0, count)) {
      const node: BSTNode = { id: nextId(), key: k, left: null, right: null };
      if (!this.root) { this.root = node; continue; }
      let cur = this.root;
      while (true) {
        if (k < cur.key) { if (!cur.left) { cur.left = node; break; } cur = cur.left; }
        else if (k > cur.key) { if (!cur.right) { cur.right = node; break; } cur = cur.right; }
        else break;
      }
    }
  }

  getState(): TreeState {
    return serialize(this.root);
  }
}
