import type { ComplexityRow } from '../types';

interface Props { rows: ComplexityRow[]; }

export function ComplexityTable({ rows }: Props) {
  return (
    <table className="w-full text-sm">
      <thead>
        <tr className="text-gray-500 border-b border-gray-800">
          <th className="text-left py-2 font-medium">Operation</th>
          <th className="text-right py-2 font-mono font-medium">Average</th>
          <th className="text-right py-2 font-mono font-medium">Worst</th>
        </tr>
      </thead>
      <tbody>
        {rows.map((row) => (
          <tr key={row.operation} className="border-b border-gray-800/50 hover:bg-gray-800/30 transition-colors">
            <td className="py-2 text-gray-300">{row.operation}</td>
            <td className="py-2 text-right font-mono text-indigo-400">{row.average}</td>
            <td className="py-2 text-right font-mono text-orange-400">{row.worst}</td>
          </tr>
        ))}
      </tbody>
    </table>
  );
}
