import { useBinomialHeap } from '../hooks/useBinomialHeap';
import { useGithubDir } from '../hooks/useGithubDir';
import { binomialHeapInfo } from '../data/binomialHeapInfo';
import { BHEAP_CODE_LINES } from '../lib/binomialHeap';
import { PageHeader } from '../components/PageHeader';
import { HeapForest } from '../components/viz/HeapForest';
import { ControlBar } from '../components/viz/ControlBar';
import { InfoPanel } from '../components/InfoPanel';
import { ImplPanel } from '../components/ImplPanel';
import { StructurePageSkeleton } from '../components/StructurePageSkeleton';
import { StepModal } from '../components/modal/StepModal';

export function BinomialHeapPage() {
  const { treeState, steps, modalOpen, setModalOpen, operationLabel, insert, extractMin, reset, randomFill } = useBinomialHeap();
  const dir = useGithubDir(binomialHeapInfo.repoDirUrl);

  if (dir.dirStatus === 'loading') return <StructurePageSkeleton />;

  return (
    <div className="flex flex-col h-screen">
      <PageHeader title={binomialHeapInfo.title} subtitle={binomialHeapInfo.subtitle} badge={binomialHeapInfo.badge} />

      <div className="flex flex-1 min-h-0 overflow-hidden">
        <div className="flex flex-col flex-1 min-w-0 border-r border-gray-800">
          <div className="flex-1 overflow-hidden">
            <HeapForest state={treeState} />
          </div>
          <ControlBar
            onInsert={insert}
            onReset={reset}
            onRandomFill={randomFill}
            onExtractMin={extractMin}
            showExtract
          />
          <ImplPanel
            codeLines={binomialHeapInfo.codeLines}
            githubUrl={binomialHeapInfo.githubUrl}
            dirStatus={dir.dirStatus}
            files={dir.files}
            selected={dir.selected}
            contentMap={dir.contentMap}
            fileStatus={dir.fileStatus}
            onSelectFile={dir.selectFile}
          />
        </div>
        <div className="w-72 shrink-0 flex flex-col overflow-hidden">
          <InfoPanel info={binomialHeapInfo} />
        </div>
      </div>

      <StepModal
        open={modalOpen}
        onClose={() => setModalOpen(false)}
        steps={steps}
        operationLabel={operationLabel}
        codeLines={BHEAP_CODE_LINES}
      />
    </div>
  );
}
