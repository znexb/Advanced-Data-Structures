import { AnimatePresence } from 'framer-motion';
import type { TreeState } from '../../types';
import { TreeNode } from './TreeNode';

const R = 22;
const MIN_H = 300;

interface Props {
  state: TreeState;
}

export function HeapForest({ state }: Props) {
  const vw = Math.max(600, state.width + 40);
  const vh = Math.max(MIN_H, state.height + 40);

  return (
    <div className="relative w-full bg-gray-950 overflow-hidden" style={{ minHeight: MIN_H }}>
      {state.nodes.length === 0 && (
        <div className="absolute inset-0 flex items-center justify-center text-gray-600 text-sm select-none">
          Heap is empty — insert a key to begin.
        </div>
      )}
      <svg
        className="w-full h-full"
        viewBox={`0 0 ${vw} ${vh}`}
        preserveAspectRatio="xMidYMid meet"
      >
        {state.edges.map(edge => {
          const from = state.nodes.find(n => n.id === edge.fromId);
          const to = state.nodes.find(n => n.id === edge.toId);
          if (!from || !to) return null;
          return (
            <line
              key={edge.id}
              x1={from.x} y1={from.y + R}
              x2={to.x}   y2={to.y   - R}
              stroke={edge.highlighted ? '#facc15' : '#374151'}
              strokeWidth={1.5}
            />
          );
        })}
        <AnimatePresence>
          {state.nodes.map(node => <TreeNode key={node.id} node={node} />)}
        </AnimatePresence>
      </svg>
    </div>
  );
}
