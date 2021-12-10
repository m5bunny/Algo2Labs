
#include "Dynamic_array.h"

template<typename T>
class Binary_heap_D_A
{
private:
    using delete_data_func = void (T & param);
    using comparator_func = bool(*)(const T &, const T &);
    using obj_to_str_func = std::string(*)(const T &);
    
    static bool def_more_comp(const T & lhs, const T & rhs) { return lhs > rhs; }
    static std::string def_obj_to_str(const T & lhs) { return std::to_string(lhs); }
    
    Dynamic_array<T> * heap_array;
    comparator_func comp;

    void go_up(int el_index);
    void go_down(int el_index);
    
    void bottom_up_fix();
    void top_down_fix();
    
    int get_parent_index(int el_index) { return (el_index - 1) / 2; }
    int get_left_ch_index(int el_index) { return 2 * el_index + 1; }
    int get_right_ch_index(int el_index) { return 2 * el_index + 2; }
public:
    
    Binary_heap_D_A(comparator_func _comp = def_more_comp);
    Binary_heap_D_A(T * _array, int _size, bool _fix = true, comparator_func _comp = def_more_comp);
    ~Binary_heap_D_A();
    void insert(const T & el);
    T pop_root(delete_data_func del_dat = nullptr);
    void sort();
    void remove_arr_ptr();
    void clear(delete_data_func del_dat = nullptr);
    
    std::string to_string(int n = -1, obj_to_str_func ots = def_obj_to_str);
};

template<typename T>
Binary_heap_D_A<T>::Binary_heap_D_A(comparator_func _comp) : comp(_comp), heap_array(new Dynamic_array<T>) {}

template<typename T>
Binary_heap_D_A<T>::Binary_heap_D_A(T * _array, int _size, bool _fix, comparator_func _comp) : heap_array(new Dynamic_array<T>(_array, _size)), comp(_comp)
{
    if (_fix)
        bottom_up_fix();
    else
        top_down_fix();
}

template<typename T>
Binary_heap_D_A<T>::~Binary_heap_D_A()
{
    delete heap_array;
}


template<typename T>
void Binary_heap_D_A<T>::go_down(int el_index)
{
    int left_child_index = get_left_ch_index(el_index);
    int right_child_index = get_right_ch_index(el_index);
    int swap_index;
    while (left_child_index < heap_array->get_size())
    {
        if (right_child_index < heap_array->get_size())
            swap_index = comp(heap_array->operator[](left_child_index), heap_array->operator[](right_child_index)) ? left_child_index : right_child_index;
        else
            swap_index = left_child_index;
        if (!comp(heap_array->operator[](el_index), heap_array->operator[](swap_index)))
        {
            T temp = heap_array->operator[](swap_index);
            heap_array->operator[](swap_index) = heap_array->operator[](el_index);
            heap_array->operator[](el_index) = temp;
            el_index = swap_index;
            left_child_index = get_left_ch_index(el_index);
            right_child_index = get_right_ch_index(el_index);
        }
        else
        {
            break;
        }
    }
}

template<typename T>
void Binary_heap_D_A<T>::go_up(int el_index)
{
    int parent_index = get_parent_index(el_index);
    while (comp(heap_array->operator[](el_index), heap_array->operator[](parent_index)))
    {
        T temp = heap_array->operator[](parent_index);
        heap_array->operator[](parent_index) = heap_array->operator[](el_index);
        heap_array->operator[](el_index) = temp;
        el_index = parent_index;
        parent_index = get_parent_index(el_index);
    }
}

template<typename T>
void Binary_heap_D_A<T>::insert(const T & el)
{
    heap_array->push_back(el);
    int el_index = heap_array->get_size() - 1;
    go_up(el_index);
}

template<typename T>
std::string Binary_heap_D_A<T>::to_string(int n, obj_to_str_func ots)
{
    std::string result{ "Binary heap:\nsize: " };
    result += std::to_string(heap_array->get_size()) + '\n';
    
    if (n == -1 || n > heap_array->get_size())
        n = heap_array->get_size();
    for (int i{}, j{}, k{}; i < n; ++i)
    {
        result += std::to_string(i) + ")";
        if (i > 0)
            result += " p:" + std::to_string(get_parent_index(i));
        result += " l:" + ((get_left_ch_index(i) < heap_array->get_size()) ? std::to_string(get_left_ch_index(i)) : "null");
        result += " r:" + ((get_right_ch_index(i) < heap_array->get_size()) ? std::to_string(get_right_ch_index(i)) : "null");
        result += " {" + ots(heap_array->operator[](i)) + "}  ";
        ++k;
        if (k == pow((double)2, (double)j))
        {
            result += '\n';
            k = 0;
            ++j;
        }
    }
    
    return result;
}

template<typename T>
T Binary_heap_D_A<T>::pop_root(delete_data_func del_dat)
{
    T result = heap_array->operator[](0);
    if (del_dat != nullptr)
        del_dat(heap_array->operator[](0));
    if (heap_array->get_size() > 1)
    {
        heap_array->operator[](0) = heap_array->pop_back();
        go_down(0);
    }
    else
        heap_array->pop_back();
    return result;
}

template<typename T>
void Binary_heap_D_A<T>::clear(delete_data_func del_dat)
{
    heap_array->clear(del_dat);
    heap_array->shrink();
}

template<typename T>
void Binary_heap_D_A<T>::top_down_fix()
{
    int size = heap_array->get_size();
    for (int i{}; i < size; ++i)
        go_up(i);
}

template<typename T>
void Binary_heap_D_A<T>::bottom_up_fix()
{
    for (int i {heap_array->get_size() - 1}; i >= 0; --i)
        go_down(i);
}

template<typename T>
void Binary_heap_D_A<T>::sort()
{
    while (heap_array->get_size() > 1)
    {
        T temp = heap_array->operator[](heap_array->get_size() - 1);
        heap_array->operator[](heap_array->get_size() - 1) = heap_array->operator[](0);
        heap_array->operator[](0) = temp;
        heap_array->pop_back();
        go_down(0);
    }
}

template<typename T>
void Binary_heap_D_A<T>::remove_arr_ptr()
{
    heap_array->get_array();
}
#ifndef BINARY_HEAP_D_A_H_
#define BINARY_HEAP_D_A_H_
#endif
