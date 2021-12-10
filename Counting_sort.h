#ifndef COUNTING_SORT_H_
#define COUNTING_SORT_H_
#include <cstring>

int def_get_key(const int & lhs)
{
    return lhs;
}

void counting_sort(int * _arr, int _n, int _m)
{
    int * counters = new int[_m];
    for (int i{}; i < _m; ++i)
        counters[i] = 0;
    
    for (int i{}; i < _n; ++i)
        ++counters[_arr[i]];
    
    int j = 0;
    for (int i{}; i < _m; ++i)
    {
        while(counters[i] != 0)
        {
            _arr[j] = i;
            --counters[i];
            ++j;
        }
    }
}

template<typename T>
void counting_sort(T * _arr, int _n, int _m, int(*_get_key)(const T & lhs) = def_get_key)
{
    T * temp = new T[_n];
    int * counters = new int[_m];
    memcpy(temp, _arr, _n * sizeof(T));
    
    for (int i{}; i < _m; ++i)
        counters[i] = 0;
    
    for (int i{}; i < _n; ++i)
        ++counters[_get_key(_arr[i])];
    
    for (int i{ 1 }; i < _m; ++i)
        counters[i] += counters[i - 1];
    
    for (int i{ _n - 1 }; i >= 0; --i)
        _arr[--counters[_get_key(temp[i])]] = temp[i];
    
    delete[] counters;
    delete[] temp;
}
#endif
