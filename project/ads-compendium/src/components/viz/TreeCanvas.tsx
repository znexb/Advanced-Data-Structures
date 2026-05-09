import { useRef, useState } from 'react';
import { AnimatePresence } from 'framer-motion';
import type { TreeState } from '../../types';
import { TreeNode, BTreeNodeRect } from './TreeNode';

const R = 22;
const MIN_W = 600;
const MIN_H = 300;

interface Props {
  state: TreeState;
  isBTree?: boolean;
  emptyMessage?: string;
}

export function TreeCanvas({ state, isBTree, emptyMessage = 'Tree is empty — insert a key to begin.' }: Props) {
  const svgRef = useRef<SVGSVGElement>(null);
  const [pan, setPan] = useState({ x: 0, y: 0 });
  const [dragging, setDragging] = useState(false);
  const [dragStart, setDragStart] = useState({ x: 0, y: 0 });

  const vw = Math.max(MIN_W, state.width + 40);
  const vh = Math.max(MIN_H, state.height + 40);

  const handleMouseDown = (e: React.MouseEvent) => {
    setDragging(true);
    setDragStart({ x: e.clientX - pan.x, y: e.clientY - pan.y });
  };
  const handleMouseMove = (e: React.MouseEvent) => {
    if (!dragging) return;
    setPan({ x: e.clientX - dragStart.x, y: e.clientY - dragStart.y });
  };
  const handleMouseUp = () => setDragging(false);

  return (
    <div className="relative w-full h-full bg-gray-950 overflow-hidden" style={{ minHeight: MIN_H }}>
      {state.nodes.length === 0 && (
        <div className="absolute inset-0 flex items-center justify-center text-gray-600 text-sm select-none">
          {emptyMessage}
        </div>
      )}
      <svg
        ref={svgRef}
        className="w-full h-full cursor-grab active:cursor-grabbing"
        viewBox={`0 0 ${vw} ${vh}`}
        preserveAspectRatio="xMidYMid meet"
        onMouseDown={handleMouseDown}
        onMouseMove={handleMouseMove}
        onMouseUp={handleMouseUp}
        onMouseLeave={handleMouseUp}
      >
        <g transform={`translate(${pan.x},${pan.y})`}>
          {/* Edges */}
          {state.edges.map(edge => {
            const from = state.nodes.find(n => n.id === edge.fromId);
            const to = state.nodes.find(n => n.id === edge.toId);
            if (!from || !to) return null;
            const fromY = isBTree ? from.y + 16 : from.y + R;
            const toY = isBTree ? to.y - 16 : to.y - R;
            return (
              <line
                key={edge.id}
                x1={from.x}
                y1={fromY}
                x2={to.x}
                y2={toY}
                stroke={edge.highlighted ? '#facc15' : '#374151'}
                strokeWidth={edge.highlighted ? 2.5 : 1.5}
              />
            );
          })}

          {/* Nodes */}
          <AnimatePresence>
            {state.nodes.map(node =>
              isBTree
                ? <BTreeNodeRect key={node.id} node={node} />
                : <TreeNode key={node.id} node={node} />
            )}
          </AnimatePresence>
        </g>
      </svg>
    </div>
  );
}
