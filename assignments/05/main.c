#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct binary_heap {
    int *array;
    int length;
    int heap_size;
} binary_heap;

int left(int i) {
    return 2 * i + 1;
}

int right(int i) {
    return 2 * i + 2;
}

int parent(int i) {
    return (i - 1) / 2;
}

void max_heapify(binary_heap *A, int i) {
    int l = left(i);
    int r = right(i);
    int largest = i;

    // Check if left child is larger than root
    if (l < A->heap_size && A->array[l] > A->array[largest]) {
        largest = l;
    }

    // Check if right child is larger than largest so far
    if (r < A->heap_size && A->array[r] > A->array[largest]) {
        largest = r;
    }

    // If largest is not root
    if (largest != i) {
        // Swap A->array[i] and A->array[largest]
        int temp = A->array[i];
        A->array[i] = A->array[largest];
        A->array[largest] = temp;

        // Recursively heapify the affected sub-tree
        max_heapify(A, largest);
    }
}

void build_heap(binary_heap *A) {
    A->heap_size = A->length;
    // Every node from (length / 2) - 1 down to 0 is a non-leaf node
    for (int i = (A->length / 2) - 1; i >= 0; i--) {
        max_heapify(A, i);
    }
}

void heap_sort(binary_heap *A) {
    build_heap(A);
    
    // Repeat process, shrinking the heap by 1 each time
    for (int i = A->length - 1; i >= 1; i--) {
        // Swap maximum element A->array[0] with the last element of the current heap
        int temp = A->array[0];
        A->array[0] = A->array[i];
        A->array[i] = temp;
        
        // Extract A[i] from the heap by decrementing heap_size
        A->heap_size--;
        
        // Rearrange A[0..i-1] into a binary heap
        max_heapify(A, 0);
    }
}

int extract_max(binary_heap *A) {
    if (A->heap_size < 1) {
        printf("Heap underflow\n");
        return -1;
    }

    int max = A->array[0];
    A->array[0] = A->array[A->heap_size - 1];
    A->heap_size--;
    max_heapify(A, 0);
    return max;
}

void heap_insert(binary_heap *A, int key) {
    if (A->heap_size == A->length) {
        printf("Heap overflow\n");
        return;
    }

    A->heap_size++;
    int i = A->heap_size - 1;
    A->array[i] = key;

    while (i > 0 && A->array[parent(i)] < A->array[i]) {
        int temp = A->array[i];
        A->array[i] = A->array[parent(i)];
        A->array[parent(i)] = temp;
        i = parent(i);
    }
}

int partition(int *A, int p, int r);

void quick_sort(int *A, int p, int r) {
    if (p < r) {
        int q = partition(A, p, r);
        quick_sort(A, p, q - 1);
        quick_sort(A, q + 1, r);
    }
}

int partition(int *A, int p, int r) {
    int x = A[r];
    int i = p - 1;

    for (int j = p; j < r; j++) {
        if (A[j] <= x) {
            i++;
            int temp = A[i];
            A[i] = A[j];
            A[j] = temp;
        }
    }

    int temp = A[i + 1];
    A[i + 1] = A[r];
    A[r] = temp;
    return i + 1;
}

void header (char* s) { printf ("%s", s); }

void endl () { printf ("\n"); }

int main() {
    header ("Heap Functions\n\n");
    int initial_data[] = {4, 1, 3, 2, 16, 9, 10, 14, 8, 7};
    int n = sizeof(initial_data)/sizeof(initial_data[0]);

    // Allocate extra space so heap_insert has room
    int capacity = 20;
    int *heap_arr = (int*)malloc(capacity * sizeof(int));
    for (int i = 0; i < n; i++) {
        heap_arr[i] = initial_data[i];
    }

    binary_heap heap;
    heap.array = heap_arr;
    heap.length = n; // Set to n at first so heap_sort doesn't try to sort garbage
    heap.heap_size = n;

    printf ("Original array       ::: ");
    for (int i = 0; i < heap.length; i++) printf ("%d ", heap.array[i]);
    endl ();

    build_heap(&heap);
    printf ("After build_heap     ::: ");
    for (int i = 0; i < heap.heap_size; i++) printf ("%d ", heap.array[i]);
    endl ();

    int max_val = extract_max(&heap);
    printf ("Extracted max        ::: %d\n", max_val);
    printf ("After extract_max    ::: ");
    for (int i = 0; i < heap.heap_size; i++) printf ("%d ", heap.array[i]);
    endl ();

    // Inform the heap it has capacity for insertion
    heap.length = capacity; 
    
    printf ("Inserting 15         ::: ");
    heap_insert(&heap, 15);
    for (int i = 0; i < heap.heap_size; i++) printf ("%d ", heap.array[i]);
    endl ();

    // Adjust length to match the current valid heap elements before sorting
    heap.length = heap.heap_size; 
    heap_sort(&heap);
    printf ("After heap_sort      ::: ");
    for (int i = 0; i < heap.length; i++) printf ("%d ", heap.array[i]);
    endl ();
    endl ();
    endl ();

    free(heap_arr);

    header ("Quicksort tests\n\n");
    int qs_arr[] = {24, 9, 29, 14, 19, 27};
    int qs_n = sizeof(qs_arr)/sizeof(qs_arr[0]);

    printf ("Original array       ::: ");
    for (int i = 0; i < qs_n; i++) printf ("%d ", qs_arr[i]);
    endl ();

    quick_sort(qs_arr, 0, qs_n - 1);

    printf ("After quick_sort     ::: ");
    for (int i = 0; i < qs_n; i++) printf ("%d ", qs_arr[i]);
    endl ();
    endl ();

    printf ("Success!\n");
    endl ();
    return 0;
}
