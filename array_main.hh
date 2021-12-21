#ifndef array_main_hh
#define array_main_hh

#include "array_view.hh"

//Sort Enum
enum SortOptions
{
    INSERTION_SORT,
    BUBBLE_SORT,
    SELECTION_SORT,
    MERGE_SORT,
    QUICK_SORT,
    SHELL_SORT,
    HEAP_SORT
};

void sort_view(AlgorithmViz::ArrayView *view, SortOptions sort);

//Sorts
void bubble_sort(AlgorithmViz::ArrayView *view);
void insertion_sort(AlgorithmViz::ArrayView *view);
void merge_sort(AlgorithmViz::ArrayView *view, int l, int r);
void quick_sort(AlgorithmViz::ArrayView *view, int l, int r);
void heap_sort(AlgorithmViz::ArrayView *view);
void shell_sort(AlgorithmViz::ArrayView *view);

//Helpers
void merge(AlgorithmViz::ArrayView *view, int l, int m, int r);
int partition(AlgorithmViz::ArrayView *view, int l, int r);
void heapify(AlgorithmViz::ArrayView *view, int length, int i);

#endif