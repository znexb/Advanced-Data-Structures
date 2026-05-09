import { useRef, useState, useCallback } from 'react';
import { BTree } from '../lib/btree';
import type { Step, TreeState } from '../types';

export function useBTree() {
  const btree = useRef(new BTree(2));
  const [treeState, setTreeState] = useState<TreeState>(() => btree.current.getState());
  const [steps, setSteps] = useState<Step[]>([]);
  const [modalOpen, setModalOpen] = useState(false);
  const [operationLabel, setOperationLabel] = useState('');
  const [order, setOrderState] = useState(2);

  const openModal = (label: string, generated: Step[]) => {
    if (generated.length > 0) {
      setSteps(generated);
      setOperationLabel(label);
      setModalOpen(true);
    }
  };

  const insert = useCallback((key: number) => {
    const s = btree.current.insert(key);
    setTreeState(btree.current.getState());
    openModal(`Inserting ${key}`, s);
  }, []);

  const search = useCallback((key: number) => {
    const s = btree.current.search(key);
    openModal(`Searching for ${key}`, s);
  }, []);

  const setOrder = useCallback((t: number) => {
    btree.current.reset(t);
    setOrderState(t);
    setTreeState(btree.current.getState());
    setModalOpen(false);
    setSteps([]);
  }, []);

  const reset = useCallback(() => {
    btree.current.reset();
    setTreeState(btree.current.getState());
    setModalOpen(false);
    setSteps([]);
  }, []);

  const randomFill = useCallback(() => {
    btree.current.randomFill();
    setTreeState(btree.current.getState());
  }, []);

  return { treeState, steps, modalOpen, setModalOpen, operationLabel, order, insert, search, setOrder, reset, randomFill };
}
