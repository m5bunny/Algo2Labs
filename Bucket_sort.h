#ifndef BUCKET_SORT_H_
#define BUCKET_SORT_H_

bool def_int_comp(const int & lhs, const int & rhs)
{
    return lhs > rhs;
}

template<typename T>
void insertion_sort(T * _array, int _n, bool (*_comp)(const T &, const T &) = def_int_comp)
{
    int j;
    T key;
    for (int i{ 1 }; i < _n; i++)
    {
        key = _array[i];
        j = i - 1;
        while (j >= 0 && _comp(_array[j], key))
        {
            _array[j + 1] = _array[j];
            --j;
        }
        _array[j + 1] = key;
    }
}

void bucket_sort(int * _array, int _n, int _m)
{
    Dynamic_array<int> * buckets = new Dynamic_array<int>[_n + 1];
    for (int i{}; i < _n; ++i)
        buckets[(long long)(_n  * (double)_array[i] / _m)].push_back(_array[i]);
    
    int * temp_arr;
    int temp_arr_size, k{};
    
    for (int i{}; i <= _n; ++i)
    {
        temp_arr_size = buckets[i].get_size();
        if (temp_arr_size > 0)
        {
            temp_arr = buckets[i].get_array();
            insertion_sort(temp_arr, temp_arr_size);
            for (int j{}; j < temp_arr_size; ++j)
                _array[k++] = temp_arr[j];
            delete[] temp_arr;
        }
    }
    delete[] buckets;
}


template<typename T, typename V>
void bucket_sort(T * _array, int _n, V _m, V (*_get_key)(const T &), bool (*_comp)(const T &, const T &))
{
    Dynamic_array<T> * buckets = new Dynamic_array<T>[_n + 1];
    for (int i{}; i < _n; ++i)
        buckets[(long long)(_n * _get_key(_array[i]) / _m)].push_back(_array[i]);
    
    T * temp_arr;
    int temp_arr_size, k{};
    for (int i{}; i <= _n; ++i)
    {
        temp_arr_size = buckets[i].get_size();
        if (temp_arr_size > 0 )
        {
            temp_arr = buckets[i].get_array();
            insertion_sort(temp_arr, temp_arr_size, _comp);
            for (int j{}; j < temp_arr_size; ++j)
                _array[k++] = temp_arr[j];
            delete[] temp_arr;
        }
    }
    delete[] buckets;
}
#endif
