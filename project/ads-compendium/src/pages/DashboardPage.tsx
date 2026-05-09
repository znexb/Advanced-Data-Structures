import { Link } from 'react-router-dom';
import { GitBranch, Circle, Database, Layers, ArrowRight } from 'lucide-react';

const cards = [
  {
    to: '/bst',
    title: 'Binary Search Tree',
    icon: GitBranch,
    badge: 'O(log n) avg',
    description: 'The simplest ordered tree — insert, search, and delete all follow the BST property left to right.',
    tags: ['Insert', 'Delete', 'Search', 'Traversal'],
    accent: 'from-indigo-600/20 to-indigo-600/5 border-indigo-600/30',
    iconColor: 'text-indigo-400',
  },
  {
    to: '/rbt',
    title: 'Red-Black Tree',
    icon: Circle,
    badge: 'O(log n) worst',
    description: 'A self-balancing BST that guarantees O(log n) even in pathological cases via color invariants and rotations.',
    tags: ['Self-balancing', 'Rotations', 'Invariants'],
    accent: 'from-red-600/20 to-red-600/5 border-red-600/30',
    iconColor: 'text-red-400',
  },
  {
    to: '/btree',
    title: 'B-Tree',
    icon: Database,
    badge: 'O(log_t n) I/O',
    description: 'A multi-way balanced tree designed for disk storage. Nodes hold many keys to minimize page reads.',
    tags: ['Disk I/O', 'Node splits', 'Configurable order'],
    accent: 'from-orange-600/20 to-orange-600/5 border-orange-600/30',
    iconColor: 'text-orange-400',
  },
  {
    to: '/binomial-heap',
    title: 'Binomial Heap',
    icon: Layers,
    badge: 'O(log n) merge',
    description: 'A forest of binomial trees supporting priority-queue operations including O(log n) heap union.',
    tags: ['Forest', 'Extract Min', 'Merge / Union'],
    accent: 'from-teal-600/20 to-teal-600/5 border-teal-600/30',
    iconColor: 'text-teal-400',
  },
];

export function DashboardPage() {
  return (
    <div className="p-6 max-w-5xl mx-auto">
      <div className="mb-8">
        <h1 className="text-3xl font-bold text-white">Data Structures Explorer</h1>
        <p className="text-gray-400 mt-2">An interactive visual reference for four core ADS structures.</p>
      </div>

      <div className="grid grid-cols-1 md:grid-cols-2 gap-4">
        {cards.map(({ to, title, icon: Icon, badge, description, tags, accent, iconColor }) => (
          <Link
            key={to}
            to={to}
            className={`group block bg-gradient-to-br ${accent} border rounded-xl p-5 hover:scale-[1.01] transition-all duration-200`}
          >
            <div className="flex items-start justify-between mb-3">
              <div className="flex items-center gap-2">
                <Icon size={20} className={iconColor} />
                <h2 className="text-white font-semibold">{title}</h2>
              </div>
              <span className="text-xs font-mono text-gray-400 bg-gray-800/80 px-2 py-0.5 rounded-full border border-gray-700/50">
                {badge}
              </span>
            </div>
            <p className="text-gray-400 text-sm leading-relaxed mb-4">{description}</p>
            <div className="flex items-center justify-between">
              <div className="flex flex-wrap gap-1.5">
                {tags.map(tag => (
                  <span key={tag} className="text-xs px-2 py-0.5 rounded-full bg-gray-800/60 text-gray-400 border border-gray-700/40">
                    {tag}
                  </span>
                ))}
              </div>
              <ArrowRight size={16} className="text-gray-600 group-hover:text-gray-400 group-hover:translate-x-1 transition-all" />
            </div>
          </Link>
        ))}
      </div>

      <div className="mt-10 p-5 rounded-xl border border-gray-800 bg-gray-900/50 text-sm text-gray-400">
        <p className="font-medium text-gray-300 mb-2">How to use</p>
        <ul className="space-y-1.5">
          <li><span className="text-indigo-400">→</span> Pick a structure from the sidebar or the cards above.</li>
          <li><span className="text-indigo-400">→</span> Use the control bar to insert, delete, or search keys.</li>
          <li><span className="text-indigo-400">→</span> A step-by-step modal walks through every algorithmic decision.</li>
          <li><span className="text-indigo-400">→</span> Use <kbd className="px-1.5 py-0.5 rounded bg-gray-800 text-xs font-mono">←</kbd> / <kbd className="px-1.5 py-0.5 rounded bg-gray-800 text-xs font-mono">→</kbd> or auto-play to advance the visualization.</li>
        </ul>
      </div>
    </div>
  );
}
