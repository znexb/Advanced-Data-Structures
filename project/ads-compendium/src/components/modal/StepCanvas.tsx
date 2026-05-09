import type { TreeState } from '../../types';
import { TreeNode, BTreeNodeRect } from '../viz/TreeNode';

const R = 22;
const MIN_H = 220;

interface Props {
  state: TreeState;
  isBTree?: boolean;
}

export function StepCanvas({ state, isBTree }: Props) {
  const vw = Math.max(360, state.width + 40);
  const vh = Math.max(MIN_H, state.height + 40);

  return (
    <svg
      className="w-full h-full"
      viewBox={`0 0 ${vw} ${vh}`}
      preserveAspectRatio="xMidYMid meet"
      style={{ minHeight: MIN_H }}
    >
      {state.edges.map(edge => {
        const from = state.nodes.find(n => n.id === edge.fromId);
        const to = state.nodes.find(n => n.id === edge.toId);
        if (!from || !to) return null;
        const fromY = isBTree ? from.y + 16 : from.y + R;
        const toY = isBTree ? to.y - 16 : to.y - R;
        return (
          <line
            key={edge.id}
            x1={from.x} y1={fromY}
            x2={to.x}   y2={toY}
            stroke={edge.highlighted ? '#facc15' : '#374151'}
            strokeWidth={edge.highlighted ? 2.5 : 1.5}
          />
        );
      })}
      {state.nodes.map(node =>
        isBTree
          ? <BTreeNodeRect key={node.id} node={node} />
          : <TreeNode key={node.id} node={node} />
      )}
    </svg>
  );
}
