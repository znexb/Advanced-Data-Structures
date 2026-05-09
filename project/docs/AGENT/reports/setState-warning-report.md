# React setState Warning Report

## Issue
**File:** `src/hooks/useGithubDir.ts`  
**Line:** 22  
**Warning Type:** Synchronous setState calls within effect

## Problem Description

The `useGithubDir` hook is calling multiple `setState` functions synchronously at the beginning of the effect (lines 22-25):

```typescript
useEffect(() => {
  setDirStatus('loading');      // Line 22
  setFiles([]);                 // Line 23
  setSelected('');              // Line 24
  setContentMap({});            // Line 25
  setFileStatus('idle');        // Line 26
  
  fetch(repoDirUrl)...
}, [repoDirUrl]);
```

React's linter flags this as an anti-pattern because it can cause cascading renders and performance issues.

## Why It's Problematic

1. **Multiple State Updates:** Calling several `setState` functions synchronously in effect body triggers unnecessary re-renders
2. **Not the Intended Pattern:** React effects are designed to synchronize with external systems (like API calls), not to reset internal state
3. **Performance Impact:** Cascading updates can degrade performance
4. **Best Practices Violation:** React documentation recommends against this pattern

## Recommended Solutions

### Option 1: Lazy Initialization (Preferred)
Initialize state variables with default values instead of resetting them in the effect:

```typescript
const [dirStatus, setDirStatus] = useState<DirStatus>('loading');
const [files, setFiles] = useState<FileEntry[]>([]);
const [selected, setSelected] = useState('');
const [contentMap, setContentMap] = useState<Record<string, string>>({});
const [fileStatus, setFileStatus] = useState<FileStatus>('idle');

useEffect(() => {
  // Remove the synchronous setState calls here
  fetch(repoDirUrl)...
}, [repoDirUrl]);
```

**Why this works:** State is already initialized to these values, so the reset is redundant.

### Option 2: Use a Ref as Abort Signal
Track the effect lifecycle with a ref to avoid stale updates:

```typescript
useEffect(() => {
  let isMounted = true;
  
  const performFetch = async () => {
    try {
      const response = await fetch(repoDirUrl);
      if (!response.ok) throw new Error(`${response.status}`);
      const entries = await response.json();
      
      if (!isMounted) return; // Don't update if unmounted
      
      const filtered = entries
        .filter(e => e.type === 'file' && CODE_EXTS.has(getExt(e.name)))
        .map(e => ({ name: e.name, downloadUrl: e.download_url }));
      
      setFiles(filtered);
      setDirStatus('ok');
      // ... rest of logic
    } catch {
      if (isMounted) setDirStatus('error');
    }
  };
  
  performFetch();
  
  return () => { isMounted = false; }; // Cleanup
}, [repoDirUrl]);
```

### Option 3: Reset via Component Prop
Use a key prop on the component to trigger complete state reset:

```typescript
// In parent component:
<YourComponent key={repoDirUrl} repoDirUrl={repoDirUrl} />
```

This causes React to unmount and remount the component, resetting all state naturally.

## Current Impact

- The hook functions correctly despite the warning
- Performance impact is minimal for this use case
- Warning indicates code style/pattern violation rather than functionality bug

## Next Steps

1. Review which solution fits your architecture best
2. Implement the chosen solution
3. Verify warning is resolved
4. Run performance tests if applicable
