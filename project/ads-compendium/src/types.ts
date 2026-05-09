export type NodeColor = 'default' | 'red' | 'black' | 'highlighted' | 'found' | 'new';

export interface NodeData {
  id: string;
  label: string;
  x: number;
  y: number;
  color?: NodeColor;
  width?: number;
}

export interface EdgeData {
  id: string;
  fromId: string;
  toId: string;
  highlighted?: boolean;
}

export interface TreeState {
  nodes: NodeData[];
  edges: EdgeData[];
  width: number;
  height: number;
}

export interface Step {
  description: string;
  highlightNodeIds: string[];
  highlightEdgeIds: string[];
  codeLineIndex: number;
  snapshot: TreeState;
}

export interface ComplexityRow {
  operation: string;
  average: string;
  worst: string;
}

export interface StructureInfo {
  title: string;
  subtitle: string;
  badge: string;
  notes: string[];
  useCases: string[];
  funFacts: string[];
  complexity: ComplexityRow[];
  codeLines: string[];      // fallback shown if fetch fails or offline
  githubUrl: string;        // link to the directory tree on GitHub
  repoDirUrl: string;       // api.github.com contents URL for the assignment directory
}
