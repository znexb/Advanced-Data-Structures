import { NavLink } from 'react-router-dom';
import { LayoutDashboard, GitBranch, Circle, Database, Layers } from 'lucide-react';

const links = [
  { to: '/',              label: 'Dashboard',       icon: LayoutDashboard },
  { to: '/bst',           label: 'Binary Search Tree', icon: GitBranch },
  { to: '/rbt',           label: 'Red-Black Tree',   icon: Circle },
  { to: '/btree',         label: 'B-Tree',           icon: Database },
  { to: '/binomial-heap', label: 'Binomial Heap',    icon: Layers },
];

export function Sidebar() {
  return (
    <aside className="w-56 shrink-0 bg-gray-900 border-r border-gray-800 flex flex-col h-screen sticky top-0">
      <div className="px-5 py-5 border-b border-gray-800">
        <span className="text-indigo-400 font-bold text-sm tracking-widest uppercase">ADS</span>
        <p className="text-gray-500 text-xs mt-0.5">Compendium</p>
      </div>
      <nav className="flex-1 py-4 px-2 space-y-1 overflow-y-auto">
        {links.map(({ to, label, icon: Icon }) => (
          <NavLink
            key={to}
            to={to}
            end={to === '/'}
            className={({ isActive }) =>
              `flex items-center gap-3 px-3 py-2.5 rounded-lg text-sm transition-colors ${
                isActive
                  ? 'bg-indigo-600/20 text-indigo-400 font-medium'
                  : 'text-gray-400 hover:text-gray-200 hover:bg-gray-800'
              }`
            }
          >
            <Icon size={16} className="shrink-0" />
            <span className="truncate">{label}</span>
          </NavLink>
        ))}
      </nav>
    </aside>
  );
}
