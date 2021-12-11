#ifndef DYNAMIC_ARRAY_DYNAMIC_ARRAY_H_
#define DYNAMIC_ARRAY_DYNAMIC_ARRAY_H_
#include <sstream>

template<typename T>
class Dynamic_array
{
    using delete_data_func = void (T & param);
    using comparator_func = bool(*)(const T &, const T &);
    using obj_to_str_func = std::string(*)(const T &);
    
    static bool def_more_comparator(const T & lhs, const T & rhs) { return lhs > rhs; }
    static std::string def_obj_to_str_func(const T & rhs) { return std::to_string(rhs); }
    
    int size;
    int capacity;
    T * array;
    
    void doQsort(int start, int end, comparator_func comp);
    
public:
    explicit Dynamic_array(int _capacity = 1);
    Dynamic_array(T * _array, int _size);
    ~Dynamic_array();
    
    void push_back(const T & el);
    T pop_back(delete_data_func del_dat = nullptr);
    void clear(delete_data_func del_dat = nullptr);
    void shrink(int shrink_to = -1);
    void sort(comparator_func comp = def_more_comparator);
    void qsort(comparator_func comp = def_more_comparator) { doQsort(0, size - 1, comp); };
    int get_size() const { return size; };
    T * get_array();
    std::string to_string(int amount = -1, obj_to_str_func ots = def_obj_to_str_func) const;
    
    const T & operator[](int i) const;
    T & operator[](int i);
};

template<typename T>
Dynamic_array<T>::Dynamic_array(int _capacity) : size(0), capacity(_capacity), array(new T[capacity]) { }
template<typename T>
Dynamic_array<T>::Dynamic_array(T * _array, int _size) : size(_size), capacity(size), array(_array) {}
template<typename T>
Dynamic_array<T>::~Dynamic_array() { delete[] array; }

template<typename T>
void Dynamic_array<T>::push_back(const T & el)
{
    if (size + 1 > capacity)
    {
        capacity *= 2;
        T * temp_arr = array;
        array = new T[capacity];
        for (int i{}; i < size; ++i)
            array[i] = temp_arr[i];
        delete[] temp_arr;
    }
    array[size] = el;
    ++size;
}

template<typename T>
T Dynamic_array<T>::pop_back(delete_data_func del_dat)
{
    T result = array[size - 1];
    if (del_dat != nullptr)
        del_dat(array[size - 1]);
    --size;
    return result;
}

template<typename T>
const T & Dynamic_array<T>::operator[](int i) const
{
    if (i < 0 || i >= size)
        throw "Out of range";
    return array[i];
}

template<typename T>
T & Dynamic_array<T>::operator[](int i)
{
    return const_cast<T &>(const_cast<const Dynamic_array &>(*this).operator[](i));
}

template<typename T>
void Dynamic_array<T>::clear(delete_data_func del_dat)
{
    if (del_dat != nullptr)
        for (int i{}; i < size; ++i)
            del_dat(array[i]);
    size = 0;
}

template<typename T>
void Dynamic_array<T>::shrink(int shrink_to)
{
    if (shrink_to > -1)
        capacity = shrink_to;
    else if (size > 0)
        capacity = size;
    else
        capacity = 1;
    
    T * temp_arr = array;
    array = new T[capacity];
    
    for (int i{}; i < size; ++i)
        array[i] = temp_arr[i];
    
    delete[] temp_arr;
}

template<typename T>
void Dynamic_array<T>::sort(comparator_func comp)
{
    for (int i{}; i < size - 1; ++i)
    {
        bool swop_flag { false };
        for (int j{}; j < size - 1 - i; ++j)
        {
            if (comp(array[j], array[j + 1]))
            {
                T temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
                
                swop_flag = true;
            }
        }
        if (!swop_flag)
            break;
    }
}

template<typename T>
std::string Dynamic_array<T>::to_string(int amount, obj_to_str_func ots) const
{
    if (amount > size || amount == -1)
        amount = size;
    
    std::stringstream ss;
    ss << "Size: " << size << std::endl
    << "Capacity: " << capacity << std::endl
    << "Elements:\n";
    
    for (int i{}; i < amount; ++i)
    {
        ss << i << ". " << ots(array[i]) << ";\n";
    }
    
    return ss.str();
}

template<typename T>
void Dynamic_array<T>::doQsort(int start, int end, comparator_func comp)
{
    if (start < end)
    {
        T pivot = array[start];
        int front_it = start;
        int back_it = end;
        while (front_it < back_it)
        {
            while (comp(pivot, array[front_it]))
                ++front_it;
            while (comp(array[back_it], pivot))
                --back_it;
            if (front_it < back_it)
            {
                T temp = array[front_it];
                array[front_it++] = array[back_it];
                array[back_it--] = temp;
            }
            
        }
        
        doQsort(start, back_it, comp);
        doQsort(back_it + 1, end, comp);
    }
}

template<typename T>
T * Dynamic_array<T>::get_array()
{
    T * result = array;
    array = nullptr;
    size = capacity = 0;
    return result;
}
#endif //DYNAMIC_ARRAY_DYNAMIC_ARRAY_H_
