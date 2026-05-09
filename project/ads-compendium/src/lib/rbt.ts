import type { NodeData, EdgeData, TreeState, Step } from '../types';

let rbtIdCounter = 0;
function nextId() { return `r${++rbtIdCounter}`; }

type Color = 'RED' | 'BLACK';

export interface RBTNode {
  id: string;
  key: number;
  color: Color;
  left: RBTNode | null;
  right: RBTNode | null;
  parent: RBTNode | null;
}

const NIL: RBTNode = { id: 'nil', key: -Infinity, color: 'BLACK', left: null, right: null, parent: null };

const HGAP = 55;
const VGAP = 75;

function makeNode(key: number): RBTNode {
  return { id: nextId(), key, color: 'RED', left: NIL, right: NIL, parent: NIL };
}

function assignLayout(root: RBTNode): Map<string, { x: number; y: number }> {
  const pos = new Map<string, { x: number; y: number }>();
  let idx = 0;
  function inorder(n: RBTNode) {
    if (n === NIL) return;
    inorder(n.left!);
    pos.set(n.id, { x: idx * HGAP + 40, y: 0 });
    idx++;
    inorder(n.right!);
  }
  function setDepth(n: RBTNode, d: number) {
    if (n === NIL) return;
    pos.get(n.id)!.y = d * VGAP + 40;
    setDepth(n.left!, d + 1);
    setDepth(n.right!, d + 1);
  }
  inorder(root);
  setDepth(root, 0);
  return pos;
}

function serialize(
  root: RBTNode,
  highlightNodeIds: string[] = [],
  newNodeId?: string,
): TreeState {
  const pos = assignLayout(root);
  const nodes: NodeData[] = [];
  const edges: EdgeData[] = [];

  function walk(n: RBTNode) {
    if (n === NIL) return;
    const p = pos.get(n.id) ?? { x: 0, y: 0 };
    let color: NodeData['color'] = n.color === 'RED' ? 'red' : 'black';
    if (n.id === newNodeId) color = 'new';
    else if (highlightNodeIds.includes(n.id)) color = 'highlighted';
    nodes.push({ id: n.id, label: String(n.key), x: p.x, y: p.y, color });
    if (n.left !== NIL) {
      const eid = `e-${n.id}-${n.left!.id}`;
      edges.push({ id: eid, fromId: n.id, toId: n.left!.id });
      walk(n.left!);
    }
    if (n.right !== NIL) {
      const eid = `e-${n.id}-${n.right!.id}`;
      edges.push({ id: eid, fromId: n.id, toId: n.right!.id });
      walk(n.right!);
    }
  }
  walk(root);

  const maxX = nodes.reduce((m, n) => Math.max(m, n.x), 0);
  const maxY = nodes.reduce((m, n) => Math.max(m, n.y), 0);
  return { nodes, edges, width: maxX + 60, height: maxY + 60 };
}

export const RBT_CODE_LINES = [
  'function insertFixup(z):',
  '  while z.parent.color == RED:',
  '    if z.parent is left child:',
  '      y = uncle (right of grandparent)',
  '      case 1: uncle RED → recolor',
  '      case 2: z is right child → left-rotate',
  '      case 3: z is left child → right-rotate + recolor',
  '    else: symmetric cases',
  '  root.color = BLACK',
  '─── rotations ───────────────────',
  'function leftRotate(x):',
  '  y = x.right',
  '  x.right = y.left',
  '  y.left = x',
  '  fix parent pointers',
  'function rightRotate(y):',
  '  x = y.left',
  '  y.left = x.right',
  '  x.right = y',
  '  fix parent pointers',
];

export class RBT {
  root: RBTNode = NIL;

  private leftRotate(x: RBTNode) {
    const y = x.right!;
    x.right = y.left;
    if (y.left !== NIL) y.left!.parent = x;
    y.parent = x.parent;
    if (x.parent === NIL) this.root = y;
    else if (x === x.parent!.left) x.parent!.left = y;
    else x.parent!.right = y;
    y.left = x;
    x.parent = y;
  }

  private rightRotate(y: RBTNode) {
    const x = y.left!;
    y.left = x.right;
    if (x.right !== NIL) x.right!.parent = y;
    x.parent = y.parent;
    if (y.parent === NIL) this.root = x;
    else if (y === y.parent!.left) y.parent!.left = x;
    else y.parent!.right = x;
    x.right = y;
    y.parent = x;
  }

  insert(key: number): Step[] {
    const steps: Step[] = [];
    const z = makeNode(key);

    steps.push({
      description: `Insert ${key} as a new RED node and find its BST position.`,
      highlightNodeIds: [],
      highlightEdgeIds: [],
      codeLineIndex: 0,
      snapshot: serialize(this.root),
    });

    // BST insert
    let y: RBTNode = NIL;
    let x: RBTNode = this.root;
    while (x !== NIL) {
      y = x;
      if (key < x.key) {
        steps.push({ description: `${key} < ${x.key} → go LEFT`, highlightNodeIds: [x.id], highlightEdgeIds: [], codeLineIndex: 0, snapshot: serialize(this.root, [x.id]) });
        x = x.left!;
      } else if (key > x.key) {
        steps.push({ description: `${key} > ${x.key} → go RIGHT`, highlightNodeIds: [x.id], highlightEdgeIds: [], codeLineIndex: 0, snapshot: serialize(this.root, [x.id]) });
        x = x.right!;
      } else {
        steps.push({ description: `Key ${key} already exists.`, highlightNodeIds: [x.id], highlightEdgeIds: [], codeLineIndex: 0, snapshot: serialize(this.root, [x.id]) });
        return steps;
      }
    }
    z.parent = y;
    if (y === NIL) this.root = z;
    else if (key < y.key) y.left = z;
    else y.right = z;

    steps.push({
      description: `Placed ${key} as RED node. Running fixup to restore RBT invariants.`,
      highlightNodeIds: [z.id],
      highlightEdgeIds: [],
      codeLineIndex: 1,
      snapshot: serialize(this.root, [z.id], z.id),
    });

    // Fixup
    let cur = z;
    while (cur.parent !== NIL && cur.parent!.color === 'RED') {
      const parent = cur.parent!;
      const grandparent = parent.parent!;

      if (grandparent === NIL) break;

      if (parent === grandparent.left) {
        const uncle = grandparent.right!;
        if (uncle !== NIL && uncle.color === 'RED') {
          // Case 1
          parent.color = 'BLACK';
          uncle.color = 'BLACK';
          grandparent.color = 'RED';
          steps.push({
            description: `Case 1: Uncle is RED. Recolor parent ${parent.key} and uncle ${uncle.key} BLACK, grandparent ${grandparent.key} RED.`,
            highlightNodeIds: [parent.id, uncle.id, grandparent.id],
            highlightEdgeIds: [],
            codeLineIndex: 4,
            snapshot: serialize(this.root, [parent.id, uncle.id, grandparent.id]),
          });
          cur = grandparent;
        } else {
          if (cur === parent.right) {
            // Case 2
            cur = parent;
            steps.push({
              description: `Case 2: Node is right child. Left-rotate at ${cur.key}.`,
              highlightNodeIds: [cur.id],
              highlightEdgeIds: [],
              codeLineIndex: 5,
              snapshot: serialize(this.root, [cur.id]),
            });
            this.leftRotate(cur);
          }
          // Case 3
          cur.parent!.color = 'BLACK';
          cur.parent!.parent!.color = 'RED';
          steps.push({
            description: `Case 3: Right-rotate at grandparent ${cur.parent!.parent!.key} and recolor.`,
            highlightNodeIds: [cur.id, cur.parent!.id],
            highlightEdgeIds: [],
            codeLineIndex: 6,
            snapshot: serialize(this.root, [cur.id, cur.parent!.id]),
          });
          this.rightRotate(cur.parent!.parent!);
        }
      } else {
        // Mirror cases
        const uncle = grandparent.left!;
        if (uncle !== NIL && uncle.color === 'RED') {
          parent.color = 'BLACK';
          uncle.color = 'BLACK';
          grandparent.color = 'RED';
          steps.push({
            description: `Case 1 (mirror): Uncle is RED. Recolor parent, uncle, grandparent.`,
            highlightNodeIds: [parent.id, uncle.id, grandparent.id],
            highlightEdgeIds: [],
            codeLineIndex: 4,
            snapshot: serialize(this.root, [parent.id, uncle.id, grandparent.id]),
          });
          cur = grandparent;
        } else {
          if (cur === parent.left) {
            cur = parent;
            steps.push({
              description: `Case 2 (mirror): Node is left child. Right-rotate at ${cur.key}.`,
              highlightNodeIds: [cur.id],
              highlightEdgeIds: [],
              codeLineIndex: 5,
              snapshot: serialize(this.root, [cur.id]),
            });
            this.rightRotate(cur);
          }
          cur.parent!.color = 'BLACK';
          cur.parent!.parent!.color = 'RED';
          steps.push({
            description: `Case 3 (mirror): Left-rotate at grandparent and recolor.`,
            highlightNodeIds: [cur.id, cur.parent!.id],
            highlightEdgeIds: [],
            codeLineIndex: 6,
            snapshot: serialize(this.root, [cur.id, cur.parent!.id]),
          });
          this.leftRotate(cur.parent!.parent!);
        }
      }
    }
    this.root.color = 'BLACK';
    steps.push({
      description: `Fixup complete. Root is BLACK. Tree is balanced.`,
      highlightNodeIds: [this.root.id],
      highlightEdgeIds: [],
      codeLineIndex: 8,
      snapshot: serialize(this.root, [this.root.id]),
    });
    return steps;
  }

  search(key: number): Step[] {
    const steps: Step[] = [];
    steps.push({ description: `Searching for ${key}.`, highlightNodeIds: [], highlightEdgeIds: [], codeLineIndex: 0, snapshot: serialize(this.root) });
    let cur = this.root;
    while (cur !== NIL) {
      steps.push({ description: `Visit ${cur.key} (${cur.color}).`, highlightNodeIds: [cur.id], highlightEdgeIds: [], codeLineIndex: 0, snapshot: serialize(this.root, [cur.id]) });
      if (key === cur.key) {
        steps.push({ description: `Found ${key}!`, highlightNodeIds: [cur.id], highlightEdgeIds: [], codeLineIndex: 0, snapshot: serialize(this.root, [cur.id]) });
        return steps;
      }
      cur = key < cur.key ? cur.left! : cur.right!;
    }
    steps.push({ description: `${key} not found.`, highlightNodeIds: [], highlightEdgeIds: [], codeLineIndex: 0, snapshot: serialize(this.root) });
    return steps;
  }

  reset() {
    this.root = NIL;
    rbtIdCounter = 0;
  }

  randomFill(count = 8) {
    this.reset();
    const pool = Array.from({ length: 90 }, (_, i) => i + 10);
    for (let i = pool.length - 1; i > 0; i--) {
      const j = Math.floor(Math.random() * (i + 1));
      [pool[i], pool[j]] = [pool[j], pool[i]];
    }
    for (const k of pool.slice(0, count)) {
      // Silent insert (no step generation)
      const z = makeNode(k);
      let y: RBTNode = NIL;
      let x: RBTNode = this.root;
      while (x !== NIL) {
        y = x;
        if (k < x.key) x = x.left!;
        else if (k > x.key) x = x.right!;
        else break;
      }
      if (x !== NIL) continue;
      z.parent = y;
      if (y === NIL) this.root = z;
      else if (k < y.key) y.left = z;
      else y.right = z;
      // Fixup (silent)
      let cur = z;
      while (cur.parent !== NIL && cur.parent!.color === 'RED') {
        const p = cur.parent!; const g = p.parent!;
        if (g === NIL) break;
        if (p === g.left) {
          const u = g.right!;
          if (u !== NIL && u.color === 'RED') { p.color = 'BLACK'; u.color = 'BLACK'; g.color = 'RED'; cur = g; }
          else { if (cur === p.right) { cur = p; this.leftRotate(cur); } cur.parent!.color = 'BLACK'; cur.parent!.parent!.color = 'RED'; this.rightRotate(cur.parent!.parent!); }
        } else {
          const u = g.left!;
          if (u !== NIL && u.color === 'RED') { p.color = 'BLACK'; u.color = 'BLACK'; g.color = 'RED'; cur = g; }
          else { if (cur === p.left) { cur = p; this.rightRotate(cur); } cur.parent!.color = 'BLACK'; cur.parent!.parent!.color = 'RED'; this.leftRotate(cur.parent!.parent!); }
        }
      }
      this.root.color = 'BLACK';
    }
  }

  getState(): TreeState {
    return serialize(this.root);
  }
}
