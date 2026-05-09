import { useBST } from '../hooks/useBST';
import { useGithubDir } from '../hooks/useGithubDir';
import { bstInfo } from '../data/bstInfo';
import { BST_CODE_LINES } from '../lib/bst';
import { PageHeader } from '../components/PageHeader';
import { TreeCanvas } from '../components/viz/TreeCanvas';
import { ControlBar } from '../components/viz/ControlBar';
import { InfoPanel } from '../components/InfoPanel';
import { ImplPanel } from '../components/ImplPanel';
import { StructurePageSkeleton } from '../components/StructurePageSkeleton';
import { StepModal } from '../components/modal/StepModal';

export function BSTPage() {
  const { treeState, steps, modalOpen, setModalOpen, operationLabel, insert, deleteKey, search, reset, randomFill } = useBST();
  const dir = useGithubDir(bstInfo.repoDirUrl);

  if (dir.dirStatus === 'loading') return <StructurePageSkeleton />;

  return (
    <div className="flex flex-col h-screen">
      <PageHeader title={bstInfo.title} subtitle={bstInfo.subtitle} badge={bstInfo.badge} />

      <div className="flex flex-1 min-h-0 overflow-hidden">
        <div className="flex flex-col flex-1 min-w-0 border-r border-gray-800">
          <div className="flex-1 overflow-hidden">
            <TreeCanvas state={treeState} />
          </div>
          <ControlBar
            onInsert={insert}
            onDelete={deleteKey}
            onSearch={search}
            onReset={reset}
            onRandomFill={randomFill}
          />
          <ImplPanel
            codeLines={bstInfo.codeLines}
            githubUrl={bstInfo.githubUrl}
            dirStatus={dir.dirStatus}
            files={dir.files}
            selected={dir.selected}
            contentMap={dir.contentMap}
            fileStatus={dir.fileStatus}
            onSelectFile={dir.selectFile}
          />
        </div>
        <div className="w-72 shrink-0 flex flex-col overflow-hidden">
          <InfoPanel info={bstInfo} />
        </div>
      </div>

      <StepModal
        open={modalOpen}
        onClose={() => setModalOpen(false)}
        steps={steps}
        operationLabel={operationLabel}
        codeLines={BST_CODE_LINES}
      />
    </div>
  );
}
