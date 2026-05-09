import { BrowserRouter, Routes, Route } from 'react-router-dom';
import { Layout } from './components/Layout';
import { DashboardPage } from './pages/DashboardPage';
import { BSTPage } from './pages/BSTPage';
import { RBTPage } from './pages/RBTPage';
import { BTreePage } from './pages/BTreePage';
import { BinomialHeapPage } from './pages/BinomialHeapPage';

export default function App() {
  return (
    <BrowserRouter>
      <Routes>
        <Route element={<Layout />}>
          <Route path="/"               element={<DashboardPage />} />
          <Route path="/bst"            element={<BSTPage />} />
          <Route path="/rbt"            element={<RBTPage />} />
          <Route path="/btree"          element={<BTreePage />} />
          <Route path="/binomial-heap"  element={<BinomialHeapPage />} />
        </Route>
      </Routes>
    </BrowserRouter>
  );
}
