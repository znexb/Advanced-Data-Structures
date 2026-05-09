import { useRef, useState, useCallback } from 'react';
import { BinomialHeap } from '../lib/binomialHeap';
import type { Step, TreeState } from '../types';

export function useBinomialHeap() {
  const heap = useRef(new BinomialHeap());
  const [treeState, setTreeState] = useState<TreeState>(() => heap.current.getState());
  const [steps, setSteps] = useState<Step[]>([]);
  const [modalOpen, setModalOpen] = useState(false);
  const [operationLabel, setOperationLabel] = useState('');

  const openModal = (label: string, generated: Step[]) => {
    if (generated.length > 0) {
      setSteps(generated);
      setOperationLabel(label);
      setModalOpen(true);
    }
  };

  const insert = useCallback((key: number) => {
    const s = heap.current.insert(key);
    setTreeState(heap.current.getState());
    openModal(`Inserting ${key}`, s);
  }, []);

  const extractMin = useCallback(() => {
    const s = heap.current.extractMin();
    setTreeState(heap.current.getState());
    openModal('Extract Minimum', s);
  }, []);

  const reset = useCallback(() => {
    heap.current.reset();
    setTreeState(heap.current.getState());
    setModalOpen(false);
    setSteps([]);
  }, []);

  const randomFill = useCallback(() => {
    heap.current.randomFill();
    setTreeState(heap.current.getState());
  }, []);

  return { treeState, steps, modalOpen, setModalOpen, operationLabel, insert, extractMin, reset, randomFill };
}
