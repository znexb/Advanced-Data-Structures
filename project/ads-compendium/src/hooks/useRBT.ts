import { useRef, useState, useCallback } from 'react';
import { RBT } from '../lib/rbt';
import type { Step, TreeState } from '../types';

export function useRBT() {
  const rbt = useRef(new RBT());
  const [treeState, setTreeState] = useState<TreeState>(() => rbt.current.getState());
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
    const s = rbt.current.insert(key);
    setTreeState(rbt.current.getState());
    openModal(`Inserting ${key}`, s);
  }, []);

  const search = useCallback((key: number) => {
    const s = rbt.current.search(key);
    openModal(`Searching for ${key}`, s);
  }, []);

  const reset = useCallback(() => {
    rbt.current.reset();
    setTreeState(rbt.current.getState());
    setModalOpen(false);
    setSteps([]);
  }, []);

  const randomFill = useCallback(() => {
    rbt.current.randomFill();
    setTreeState(rbt.current.getState());
  }, []);

  return { treeState, steps, modalOpen, setModalOpen, operationLabel, insert, search, reset, randomFill };
}
