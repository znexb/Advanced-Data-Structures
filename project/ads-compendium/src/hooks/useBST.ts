import { useRef, useState, useCallback } from 'react';
import { BST } from '../lib/bst';
import type { Step, TreeState } from '../types';

export function useBST() {
  const bst = useRef(new BST());
  const [treeState, setTreeState] = useState<TreeState>(() => bst.current.getState());
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
    const s = bst.current.insert(key);
    setTreeState(bst.current.getState());
    openModal(`Inserting ${key}`, s);
  }, []);

  const deleteKey = useCallback((key: number) => {
    const s = bst.current.delete(key);
    setTreeState(bst.current.getState());
    openModal(`Deleting ${key}`, s);
  }, []);

  const search = useCallback((key: number) => {
    const s = bst.current.search(key);
    openModal(`Searching for ${key}`, s);
  }, []);

  const reset = useCallback(() => {
    bst.current.reset();
    setTreeState(bst.current.getState());
    setModalOpen(false);
    setSteps([]);
  }, []);

  const randomFill = useCallback(() => {
    bst.current.randomFill();
    setTreeState(bst.current.getState());
  }, []);

  return { treeState, steps, modalOpen, setModalOpen, operationLabel, insert, deleteKey, search, reset, randomFill };
}
