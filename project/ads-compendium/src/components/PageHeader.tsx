interface Props {
  title: string;
  subtitle: string;
  badge: string;
}

export function PageHeader({ title, subtitle, badge }: Props) {
  return (
    <div className="px-6 pt-6 pb-4 border-b border-gray-800">
      <div className="flex items-start justify-between gap-4">
        <div>
          <h1 className="text-2xl font-bold text-white">{title}</h1>
          <p className="text-gray-400 text-sm mt-1">{subtitle}</p>
        </div>
        <span className="shrink-0 mt-1 px-3 py-1 rounded-full bg-indigo-600/20 text-indigo-400 text-xs font-mono font-medium border border-indigo-600/30">
          {badge}
        </span>
      </div>
    </div>
  );
}
