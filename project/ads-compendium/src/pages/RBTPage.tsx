import { useRBT } from '../hooks/useRBT';
import { useGithubDir } from '../hooks/useGithubDir';
import { rbtInfo } from '../data/rbtInfo';
import { RBT_CODE_LINES } from '../lib/rbt';
import { PageHeader } from '../components/PageHeader';
import { TreeCanvas } from '../components/viz/TreeCanvas';
import { ControlBar } from '../components/viz/ControlBar';
import { InfoPanel } from '../components/InfoPanel';
import { ImplPanel } from '../components/ImplPanel';
import { StructurePageSkeleton } from '../components/StructurePageSkeleton';
import { StepModal } from '../components/modal/StepModal';

export function RBTPage() {
  const { treeState, steps, modalOpen, setModalOpen, operationLabel, insert, search, reset, randomFill } = useRBT();
  const dir = useGithubDir(rbtInfo.repoDirUrl);

  if (dir.dirStatus === 'loading') return <StructurePageSkeleton />;

  return (
    <div className="flex flex-col h-screen">
      <PageHeader title={rbtInfo.title} subtitle={rbtInfo.subtitle} badge={rbtInfo.badge} />

      <div className="flex flex-1 min-h-0 overflow-hidden">
        <div className="flex flex-col flex-1 min-w-0 border-r border-gray-800">
          <div className="flex-1 overflow-hidden">
            <TreeCanvas state={treeState} />
          </div>
          <ControlBar
            onInsert={insert}
            onSearch={search}
            onReset={reset}
            onRandomFill={randomFill}
          />
          <ImplPanel
            codeLines={rbtInfo.codeLines}
            githubUrl={rbtInfo.githubUrl}
            dirStatus={dir.dirStatus}
            files={dir.files}
            selected={dir.selected}
            contentMap={dir.contentMap}
            fileStatus={dir.fileStatus}
            onSelectFile={dir.selectFile}
          />
        </div>
        <div className="w-72 shrink-0 flex flex-col overflow-hidden">
          <InfoPanel info={rbtInfo} />
        </div>
      </div>

      <StepModal
        open={modalOpen}
        onClose={() => setModalOpen(false)}
        steps={steps}
        operationLabel={operationLabel}
        codeLines={RBT_CODE_LINES}
      />
    </div>
  );
}
