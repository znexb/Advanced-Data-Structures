import { useBTree } from '../hooks/useBTree';
import { useGithubDir } from '../hooks/useGithubDir';
import { btreeInfo } from '../data/btreeInfo';
import { BTREE_CODE_LINES } from '../lib/btree';
import { PageHeader } from '../components/PageHeader';
import { TreeCanvas } from '../components/viz/TreeCanvas';
import { ControlBar } from '../components/viz/ControlBar';
import { InfoPanel } from '../components/InfoPanel';
import { ImplPanel } from '../components/ImplPanel';
import { StructurePageSkeleton } from '../components/StructurePageSkeleton';
import { StepModal } from '../components/modal/StepModal';

export function BTreePage() {
  const { treeState, steps, modalOpen, setModalOpen, operationLabel, order, insert, search, setOrder, reset, randomFill } = useBTree();
  const dir = useGithubDir(btreeInfo.repoDirUrl);

  // if (true) return <StructurePageSkeleton />
  if (dir.dirStatus === 'loading') return <StructurePageSkeleton />;

  const orderControl = (
    <div className="flex items-center gap-2 text-sm">
      <span className="text-gray-500">Min degree t =</span>
      {[2, 3, 4].map(t => (
        <button
          key={t}
          onClick={() => setOrder(t)}
          className={`w-8 h-8 rounded text-sm font-mono transition-colors ${
            order === t ? 'bg-indigo-600 text-white' : 'bg-gray-800 text-gray-400 hover:bg-gray-700'
          }`}
        >
          {t}
        </button>
      ))}
    </div>
  );

  return (
    <div className="flex flex-col h-screen">
      <PageHeader title={btreeInfo.title} subtitle={btreeInfo.subtitle} badge={btreeInfo.badge} />

      <div className="flex flex-1 min-h-0 overflow-hidden">
        <div className="flex flex-col flex-1 min-w-0 border-r border-gray-800">
          <div className="flex-1 overflow-hidden">
            <TreeCanvas state={treeState} isBTree />
          </div>
          <ControlBar
            onInsert={insert}
            onSearch={search}
            onReset={reset}
            onRandomFill={randomFill}
            extraControls={orderControl}
          />
          <ImplPanel
            codeLines={btreeInfo.codeLines}
            githubUrl={btreeInfo.githubUrl}
            dirStatus={dir.dirStatus}
            files={dir.files}
            selected={dir.selected}
            contentMap={dir.contentMap}
            fileStatus={dir.fileStatus}
            onSelectFile={dir.selectFile}
          />
        </div>
        <div className="w-72 shrink-0 flex flex-col overflow-hidden">
          <InfoPanel info={btreeInfo} />
        </div>
      </div>

      <StepModal
        open={modalOpen}
        onClose={() => setModalOpen(false)}
        steps={steps}
        operationLabel={operationLabel}
        codeLines={BTREE_CODE_LINES}
        isBTree
      />
    </div>
  );
}
