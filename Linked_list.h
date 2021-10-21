#ifndef LINKED_LIST_H_
#define LINKED_LIST_H_
#include <sstream>

template<typename T>
class Linked_list
{
    using comparator_func = bool(*)(const T &, const T &);
    using to_cout_func = std::string(*)(const T &);
    
    struct List_item
    {
        T data;
        List_item * next;
        List_item * prev;
        explicit List_item (const T & _data) { data = _data; next = prev = nullptr; }
    };
    
    List_item * head;
    List_item * tail;
    int size;
    
    List_item * const doGet(int index);
    List_item * const doFind(const T & _data, comparator_func comp);
    List_item * const find_insertby_item(const T & _data, comparator_func comp);
    void add_first(const T & _data);
    void remove_last(bool clear_data);
    
    static bool def_equal_comparator(const T & lhs, const T & rhs) { return lhs == rhs; }
    static bool def_less_comparator(const T & lhs, const T & rhs) { return lhs <= rhs; }
    static std::string def_to_cout_func(const T & rhs) { return std::to_string(rhs); }
    
public:
    Linked_list();
    ~Linked_list();
    explicit Linked_list(const T & _data);
    
    void add_back(const T & _data);
    void add_front(const T & _data);
    void add_in_order(const T & _data, comparator_func comp = def_less_comparator);
    
    void remove_back(bool clear_data = false);
    void remove_front(bool clear_data = false);
    
    const T & get(int index) const
    {
        return const_cast<Linked_list *>(this)->doGet(index)->data;
    }
    void set(int index, const T & _data);
    
    const T * const find(const T & _data, comparator_func comp = def_equal_comparator) const
    {
        return &(const_cast<Linked_list *>(this)->doFind(_data, comp)->data);
    }
    bool find_and_remove(const T & _data, bool clear_data = false,comparator_func comp = def_equal_comparator);
    
    void clear(bool clear_data = false);
    std::string to_stirng(int amount, to_cout_func func = def_to_cout_func) const;
};

template<typename T>
Linked_list<T>::Linked_list() : head(nullptr), tail(nullptr), size(0) {}

template<typename T>
Linked_list<T>::Linked_list(const T & _data) : head(new List_item(_data)), tail(head), size(1) {}


template<typename  T>
void Linked_list<T>::add_first(const T &_data)
{
    head = tail = new List_item(_data);
    size = 1;
}

template<typename  T>
void Linked_list<T>::add_back(const T & _data)
{
    if (size == 0)
        add_first(_data);
    else
    {
        tail->next = new List_item(_data);
        tail->next->prev = tail;
        tail = tail->next;
        ++size;
    }
}

template<typename T>
void Linked_list<T>::add_front(const T &_data)
{
    if (size == 0)
        add_first(_data);
    else
    {
        head->prev = new List_item(_data);
        head->prev->next = head;
        head = head->prev;
        ++size;
    }
}

template<typename T>
void Linked_list<T>::remove_last(bool clear_data)
{
    if (clear_data)
        delete head->data;
    delete head;
    head = tail = nullptr;
    size = 0;
}

template<typename T>
void Linked_list<T>::remove_back(bool clear_data)
{
    if (size != 0)
    {
        if (tail->prev != nullptr)
        {
            List_item * temp = tail->prev;
            temp->next = nullptr;
            
            if (clear_data)
                delete tail->data;
            delete tail;
            
            tail = temp;
            --size;
        }
        else
            remove_last(clear_data);
    }
    else
        throw "the list is empty";
}

template<typename T>
void Linked_list<T>::remove_front(bool clear_data)
{
    if (size != 0)
    {
        if (head->next != nullptr)
        {
            List_item * temp = head->next;
            temp->prev = nullptr;
            
            if (clear_data)
                delete head->data;
            delete head;
            
            head = temp;
            --size;
        }
        else
            remove_last();
    }
    else
        throw "the list is empty";
}

template<typename T>
typename Linked_list<T>::List_item * const Linked_list<T>::doGet(int index)
{
    if (index >= 0 && index < size)
    {
        List_item * result_item;
        if (index <= size / 2)
        {
            result_item = head;
            while (index)
            {
                result_item = result_item->next;
                --index;
            }
        }
        else
        {
            result_item = tail;
            int temp_index = size - 1;
            while (temp_index > index)
            {
                result_item = result_item->prev;
                --temp_index;
            }
        }
        
        return result_item;
    }
    throw "out of range";
}

template<typename T>
void Linked_list<T>::set(int index, const T &_data)
{
    if (index >= 0 && index < size)
        doGet(index)->data = _data;
    else
        throw "out of range";
}

template<typename T>
typename Linked_list<T>::List_item * const Linked_list<T>::doFind(const T & _data, comparator_func comp)
{
    List_item * temp = head;
    for (int i{}; i < size; ++i)
    {
        if (comp(temp->data, _data))
            return temp;
        temp = temp->next;
    }
    return nullptr;
}

template<typename T>
bool Linked_list<T>::find_and_remove(const T & _data, bool clear_data, comparator_func comp)
{
    List_item * remove_item = doFind(_data, comp);
    if (remove_item != nullptr)
    {
        if (size == 1)
            remove_last(clear_data);
        else
        {
            if (remove_item == head)
            {
                head = remove_item->next;
                head->prev = nullptr;
            }
            else
                remove_item->prev->next = remove_item->next;
            if (remove_item == tail)
            {
                tail = remove_item->prev;
                tail->next = nullptr;
            }
            else
                remove_item->next->prev = remove_item->prev;
            
            if (clear_data)
                delete remove_item->data;
            delete remove_item;
            --size;
        }
        return true;
    }
    return false;
}

template<typename T>
typename Linked_list<T>::List_item * const Linked_list<T>::find_insertby_item(const T & _data, comparator_func comp)
{
    List_item * insertby_item = nullptr;
    List_item * temp = head;
    for (int i{}; i < size; ++i)
    {
        if (insertby_item != nullptr)
        {
            if (comp(temp->data, _data) && comp(insertby_item->data, temp->data))
                insertby_item = temp;
        }
        else
            if (comp(temp->data, _data))
                insertby_item = temp;
        temp = temp->next;
    }
    return insertby_item;
}

template<typename T>
void Linked_list<T>::add_in_order(const T & _data, comparator_func comp)
{
    List_item * insertby_item = find_insertby_item(_data, comp);
    if (size == 0)
        add_first(_data);
    else
    {
        List_item * insert_item = new List_item(_data);
        if (insertby_item == nullptr)
        {
            insert_item->next = head;
            head->prev = insert_item;
            head = insert_item;
        }
        else
        {
            if (insertby_item == tail)
                tail = insert_item;
            else
                insertby_item->next->prev = insert_item;
            
            insert_item->next = insertby_item->next;
            insert_item->prev = insertby_item;
            insertby_item->next = insert_item;
        }
        ++size;
    }
}

template<typename T>
void Linked_list<T>::clear(bool clear_data)
{
    List_item * remove_item;
    while (size)
    {
        remove_item = head;
        head = head->next; 
        
        if (clear_data)
            delete remove_item->data;
        delete remove_item;
        
        if (size - 1)
            head->prev = nullptr;
        
        --size;
    }
    tail = nullptr;
}

template<typename T>
std::string Linked_list<T>::to_stirng(int amount, to_cout_func func) const
{
    std::stringstream ss;
    List_item * temp = head;
    ss << "Size: " << size << std::endl
    << "Elements:\n";
    
    if (amount > size)
        amount = size;
    for (int i{}; i < amount; ++i)
    {
        ss << i << ". "  << func(temp->data) << ";\n";
        temp = temp->next;
    }
    
    return ss.str();
}

template<typename T>
Linked_list<T>::~Linked_list()
{
    clear();
}
#endif
