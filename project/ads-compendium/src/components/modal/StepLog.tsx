interface Props {
  description: string;
}

export function StepLog({ description }: Props) {
  return (
    <div className="px-4 py-3 border-t border-gray-700 bg-gray-800/50">
      <p className="text-sm text-gray-300 leading-relaxed">{description}</p>
    </div>
  );
}
