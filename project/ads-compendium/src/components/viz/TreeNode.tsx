import { motion } from 'framer-motion';
import type { NodeData } from '../../types';

const R = 22;

const COLOR_MAP: Record<string, { fill: string; stroke: string; text: string }> = {
  default:     { fill: '#4f46e5', stroke: '#6366f1', text: '#fff' },
  red:         { fill: '#dc2626', stroke: '#ef4444', text: '#fff' },
  black:       { fill: '#374151', stroke: '#4b5563', text: '#d1d5db' },
  highlighted: { fill: '#ca8a04', stroke: '#facc15', text: '#fff' },
  found:       { fill: '#16a34a', stroke: '#22c55e', text: '#fff' },
  new:         { fill: '#0d9488', stroke: '#14b8a6', text: '#fff' },
};

interface Props {
  node: NodeData;
}

export function TreeNode({ node }: Props) {
  const c = COLOR_MAP[node.color ?? 'default'] ?? COLOR_MAP.default;

  return (
    <motion.g
      initial={{ opacity: 0, scale: 0.5 }}
      animate={{ opacity: 1, scale: 1 }}
      transition={{ type: 'spring', stiffness: 300, damping: 22 }}
    >
      {node.color === 'highlighted' && (
        <motion.circle
          cx={node.x}
          cy={node.y}
          r={R + 6}
          fill="none"
          stroke="#facc15"
          strokeWidth={2}
          opacity={0.4}
          animate={{ r: [R + 4, R + 10, R + 4] }}
          transition={{ duration: 1.2, repeat: Infinity }}
        />
      )}
      <circle
        cx={node.x}
        cy={node.y}
        r={R}
        fill={c.fill}
        stroke={c.stroke}
        strokeWidth={2}
      />
      <text
        x={node.x}
        y={node.y}
        textAnchor="middle"
        dominantBaseline="central"
        fill={c.text}
        fontSize={node.label.length > 3 ? 9 : 11}
        fontFamily="ui-monospace, monospace"
        fontWeight="600"
        pointerEvents="none"
      >
        {node.label}
      </text>
    </motion.g>
  );
}

// B-Tree rectangle node
export function BTreeNodeRect({ node }: { node: NodeData }) {
  const w = node.width ?? 56;
  const h = 32;
  const c = COLOR_MAP[node.color ?? 'default'] ?? COLOR_MAP.default;

  return (
    <motion.g
      initial={{ opacity: 0, scale: 0.8 }}
      animate={{ opacity: 1, scale: 1 }}
      transition={{ type: 'spring', stiffness: 250, damping: 20 }}
    >
      <rect
        x={node.x - w / 2}
        y={node.y - h / 2}
        width={w}
        height={h}
        rx={6}
        fill={c.fill}
        stroke={c.stroke}
        strokeWidth={1.5}
      />
      <text
        x={node.x}
        y={node.y}
        textAnchor="middle"
        dominantBaseline="central"
        fill={c.text}
        fontSize={10}
        fontFamily="ui-monospace, monospace"
        fontWeight="600"
        pointerEvents="none"
      >
        {node.label}
      </text>
    </motion.g>
  );
}
