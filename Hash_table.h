#ifndef HASH_TABLE_H_
#define HASH_TABLE_H_
#include "Linked_list.h"
#include "Dynamic_array.h"
#include <cmath>

template<typename T>
class Hash_table
{
public:
    struct Pair
    {
        std::string key;
        T data;
        Pair() {}
        Pair(std::string _key) : key(_key) {}
        Pair(std::string _key, T _data) : key(_key), data(_data) {}
        
        static bool equal_comp(const Pair & lhs, const Pair & rhs) { return lhs.key == rhs.key; }
    };
    
private:
    using obj_to_str_func = std::string(*)(const T &);
    using delete_data_func = void (Pair & param);
    
    Dynamic_array<Linked_list<Pair> *> * table;
    int table_size;
    int num_of_el;
    static constexpr float MAX_LOAD_FACTOR = 0.7;
    
    static std::string def_to_cout_func(const T & rhs) { return std::to_string(rhs); }
    
    int hash(std::string _key) const;
    void rehash(delete_data_func del_dat);
    void add_to_table(Dynamic_array<Linked_list<Pair> *> * _table, const Pair & p, int i, delete_data_func del_dat);
    
public:
    Hash_table();
    ~Hash_table();
    void add(std::string _key, T _data, delete_data_func del_dat = nullptr);
    const Pair * const find(std::string _key) const;
    bool remove(std::string _key, delete_data_func del_dat = nullptr);
    void clear(delete_data_func del_dat = nullptr);
    std::string to_string(int amount = -1, obj_to_str_func func = def_to_cout_func) const;
    std::string get_table_stats() const;
};

template<typename T>
int Hash_table<T>::hash(std::string _key) const
{
    long index{};
    for (int i{}; i < _key.size(); ++i)
        index += _key[i] * pow((double)31, (double)(_key.size() - i - 1));
    index %= table_size;
    return static_cast<int>(index);
}

template<typename T>
void Hash_table<T>::rehash(delete_data_func del_dat)
{
    int old_table_size = table_size;
    table_size *= 2;
    Dynamic_array<Linked_list<Pair> *> * temp_table = new Dynamic_array<Linked_list<Pair> *>(table_size);
    for (int i{}; i < table_size; ++i)
        temp_table->push_back(nullptr);
    for (int i{}; i < old_table_size; ++i)
    {
        Linked_list<Pair> * temp_list = table->operator[](i);
        if (temp_list != nullptr)
        {
            while(temp_list->get_size() != 0)
            {
                Pair temp_pair = temp_list->get(0);
                add_to_table(temp_table, temp_pair, hash(temp_pair.key), del_dat);
                temp_list->remove_front();
            }
            delete temp_list;
        }
    }
    delete table;
    table = temp_table;
}

template<typename T>
void Hash_table<T>::add_to_table(Dynamic_array<Linked_list<Pair> *> * _table, const Pair & p, int i, delete_data_func del_dat)
{
    if (_table->operator[](i) != nullptr)
    {
        if(_table->operator[](i)->find_and_remove(p, del_dat, Pair::equal_comp))
            --num_of_el;
        _table->operator[](i)->add_back(p);
    }
    else
    {
        _table->operator[](i) = new Linked_list<Pair>;
        _table->operator[](i)->add_back(p);
    }
}

template<typename T>
Hash_table<T>::Hash_table() : table(new Dynamic_array<Linked_list<Pair> *>(2)), table_size(2), num_of_el(0)
{
    for (int i{}; i < table_size; ++i)
        table->push_back(nullptr);
}

template<typename T>
Hash_table<T>::~Hash_table()
{
    clear();
    delete table;
}

template<typename T>
void Hash_table<T>::add(std::string _key, T _data, delete_data_func del_dat)
{
    if ((double)(num_of_el + 1) / table_size > MAX_LOAD_FACTOR)
        rehash(del_dat);
    add_to_table(table, Pair(_key, _data), hash(_key), del_dat);
    ++num_of_el;
}

template<typename T>
const typename Hash_table<T>::Pair * const Hash_table<T>::find(std::string _key) const
{
    int index = hash(_key);
    if (table->operator[](index) == nullptr)
        return nullptr;
    return table->operator[](index)->find(Pair(_key), Pair::equal_comp);
}

template<typename T>
bool Hash_table<T>::remove(std::string _key, delete_data_func del_dat)
{
    int index = hash(_key);
    if (table->operator[](index) == nullptr)
        return false;
    if(table->operator[](index)->find_and_remove(Pair(_key), del_dat, Pair::equal_comp))
    {
        if (table->operator[](index)->get_size() == 0)
        {
            delete table->operator[](index);
            table->operator[](index) = nullptr;
        }
        --num_of_el;
        return true;
    }
    return false;
}

template<typename T>
void Hash_table<T>::clear(delete_data_func del_dat)
{
    for (int i{}; i < table_size; ++i)
    {
        Linked_list<Pair> * temp_list = table->operator[](i);
        if (temp_list != nullptr)
        {
            while(temp_list->get_size() != 0)
            {
                Pair temp_pair = temp_list->get(0);
                temp_list->remove_front(del_dat);
                --num_of_el;
            }
            delete temp_list;
            table->operator[](i) = nullptr;
        }
    }
}

template<typename T>
std::string Hash_table<T>::to_string(int amount, obj_to_str_func func) const
{
    std::string result{"Hash table\n"};
    result += "Table size: " + std::to_string(table_size) + '\n';
    result += "Number of el: " + std::to_string(num_of_el) + '\n';
    
    if (amount == -1 || amount > num_of_el)
        amount = num_of_el;
    
    result += "table:\n";
    int temp_num_of_el{};
    for (int i{}; i < table_size; ++i)
    {
        Linked_list<Pair> * temp_list = table->operator[](i);
        if (temp_list != nullptr)
        {
            for (int j{}; j < temp_list->get_size(); ++j)
            {
                Pair temp_pair = temp_list->get(j);
                result += temp_pair.key + "->" + func(temp_pair.data) + ' ';
            }
            result += '\n';
            ++temp_num_of_el;
            if(temp_num_of_el >= amount)
                break;
        }
    }
    result += '\n';
    return result;
}

template<typename T>
std::string Hash_table<T>::get_table_stats() const
{
    std::string result{ "stats\n" };
    int min_size{ INT_MAX }, max_size{}, num_of_non_null{};
    for (int i{}; i < table_size; ++i)
    {
        Linked_list<Pair> * temp_list = table->operator[](i);
        if (temp_list != nullptr)
        {
            int temp_size{ temp_list->get_size() };
            ++num_of_non_null;
            if (temp_size > max_size)
                max_size = temp_size;
            if (temp_size < min_size)
                min_size = temp_size;
        }
    }
    result += "list min size: " + std::to_string(min_size) + '\n';
    result += "list max size: " + std::to_string(max_size) + '\n';
    result += "non-null lists: " + std::to_string(num_of_non_null) + '\n';
    result += "list avg size: " + std::to_string((double)num_of_el / num_of_non_null) + '\n';
    return result;
}
#endif
