#ifndef HEAP_SORT_H_
#define HEAP_SORT_H_
#include "Binary_heap_D_A.h"

template<typename T>
void heap_sort(T * _array, int _n, bool _fix = true, bool(*_comp)(const T &, const T &) = Binary_heap_D_A<T>::def_more_comp)
{
    Binary_heap_D_A<T> * bh = new Binary_heap_D_A<T>(_array, _n, _fix, _comp);
    bh->sort();
    bh->remove_arr_ptr();
    delete bh;
}

#endif
