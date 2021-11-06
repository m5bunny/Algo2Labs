#ifndef RB_TREE_H_
#define RB_TREE_H_
template<typename T>
class RB_tree
{
    
    enum class Color { BLACK, RED };
    
    struct Tree_element
    {
        int ID;
        Color color;
        T data;
        Tree_element * parent;
        Tree_element * left;
        Tree_element * right;
        Tree_element(const T & _data, int _ID, const Color & _color = Color::RED) : ID(_ID), color(_color), data(_data), parent(nullptr), left(nullptr), right(nullptr) {}
    };
    
    enum class Trav_mode { PREORDER, INORDER, POSTORDER };
    
    using comparator_func = int(*)(const T &, const T &);
    using do_with_data_func = void (const T &);
    using obj_to_str_func = std::string(*)(const T &);
    using delete_data_func = void (T & param);
    
    int id_counter;
    Tree_element * root;
    int size;
    int height;
    
    static int def_comp_func (const T & lhs, const T & rhs) { return lhs - rhs; }
    static std::string def_obj_to_str_func(const T & rhs) { return std::to_string(rhs); }
    
    Tree_element * doFind(const T & _data, comparator_func = def_comp_func);
    void doTraversal(Tree_element * el, do_with_data_func do_with_el, Trav_mode mode);
    void doClear(Tree_element * el, delete_data_func del_dat);
    std::string doTo_string(Tree_element * el, int height, obj_to_str_func obj_to_str, int max_height);
    Tree_element * min(Tree_element * _root);
    Tree_element * max(Tree_element * _root);
    void correct_parent(Tree_element * remove_element, Tree_element * remove_element_child, comparator_func comp);
    void update_height();
    void doUpdate_height(Tree_element * el, int level_height, int & max_height);
    void rotate_right(Tree_element * parent, Tree_element * child, comparator_func comp);
    void rotate_left(Tree_element * parent, Tree_element * child, comparator_func comp);
    void fix_structure(Tree_element * el, comparator_func comp);
    
public:
    RB_tree();
    explicit RB_tree(const T & _root);
    void add(const T & _data, comparator_func comp = def_comp_func, delete_data_func del_dat = nullptr);
    const T * find(const T & _data, comparator_func comp = def_comp_func);
    void preorder(do_with_data_func do_with_data) { doTraversal(root, do_with_data, Trav_mode::PREORDER); };
    void inorder(do_with_data_func do_with_data) { doTraversal(root, do_with_data, Trav_mode::INORDER); };
    void postorder(do_with_data_func do_with_data) { doTraversal(root, do_with_data, Trav_mode::POSTORDER); };
    virtual void clear(delete_data_func del_dat = nullptr);
    int get_height();
    int get_size() { return size; }
    std::string to_string(obj_to_str_func obj_to_str = def_obj_to_str_func, int max_height = -1);
};

template<typename T>
RB_tree<T>::RB_tree() : id_counter(0), root(nullptr), size(0), height(0) {}
template<typename T>
RB_tree<T>::RB_tree(const T & _root) : id_counter(0), root(new Tree_element(_root, id_counter)), size(1), height(1)
{
    root->color = Color::BLACK;
    ++id_counter;
}

template<typename T>
void RB_tree<T>::add(const T & _data, comparator_func comp, delete_data_func del_dat)
{
    Tree_element * add_element = new Tree_element(_data, id_counter);
    
    if (size)
    {
        Tree_element * temp{ root };
        while(true)
        {
            if (comp(temp->data, add_element->data))
            {
                if (comp(temp->data, add_element->data) < 0)
                {
                    if (temp->right == nullptr)
                    {
                        add_element->parent = temp;
                        temp->right = add_element;
                        ++size;
                        fix_structure(add_element, comp);
                        break;
                    }
                    temp = temp->right;
                }
                else
                {
                    if (temp->left == nullptr)
                    {
                        add_element->parent = temp;
                        temp->left = add_element;
                        ++size;
                        fix_structure(add_element, comp);
                        break;
                    }
                    temp = temp->left;
                }
            }
            else
            {
                if(del_dat != nullptr)
                    del_dat(temp->data);
                temp->data = add_element->data;
                delete add_element;
                break;
            }
        }
    }
    else
    {
        root = add_element;
        size = height = 1;
        root->color = Color::BLACK;
    }
    ++id_counter;
}

template<typename T>
void RB_tree<T>::doTraversal(Tree_element *el, do_with_data_func do_with_el, Trav_mode mode)
{
    if (el != nullptr)
    {
        if(mode == Trav_mode::PREORDER)
            do_with_el(el->data);
        doTraversal(el->left, do_with_el, mode);
        if (mode == Trav_mode::INORDER)
            do_with_el(el->data);
        doTraversal(el->right, do_with_el, mode);
        if (mode == Trav_mode::POSTORDER)
            do_with_el(el->data);
    }
}

template<typename T>
typename RB_tree<T>::Tree_element * RB_tree<T>::doFind(const T &_data, comparator_func comp)
{
    Tree_element * find_el = root;
    while (find_el != nullptr)
    {
        if (!comp(find_el->data, _data))
            return find_el;
        else if(comp(find_el->data, _data) < 0)
            find_el = find_el->right;
        else
            find_el = find_el->left;
    }
    return find_el;
}

template<typename T>
void RB_tree<T>::clear(delete_data_func del_data)
{
    doClear(root, del_data);
    root = nullptr;
    size = height = id_counter = 0;
}

template<typename T>
void RB_tree<T>::doClear(RB_tree::Tree_element * el, delete_data_func del_dat)
{
    if (el != nullptr)
    {
        doClear(el->left, del_dat);
        doClear(el->right, del_dat);
        if (del_dat != nullptr)
            del_dat(el->data);
        el->parent = el->right = el->left = nullptr;
        delete el;
    }
}

template<typename T>
std::string RB_tree<T>::doTo_string(Tree_element * el, int current_height, obj_to_str_func obj_to_str, int max_height)
{
    std::string res_str;
    if (el != nullptr)
    {
        res_str = std::to_string(current_height)  + "\t|" + std::to_string(el->ID) + ":\t"
                  + "[c: " + ((el->color == Color::RED) ? "red" : "black") + ", "
                  + "p: " + (el->parent == nullptr ? "null" : std::to_string(el->parent->ID)) + ", "
                  + "l: " + (el->left == nullptr ? "null" : std::to_string(el->left->ID)) + ", "
                  + "r: " + (el->right == nullptr ? "null" : std::to_string(el->right->ID)) + "]\t"
                  + "data: {" + obj_to_str(el->data) + "}\n";
        if (current_height + 1 <= max_height)
            res_str += doTo_string(el->left, current_height + 1, obj_to_str, max_height);
        if (current_height + 1 <= max_height)
            res_str += doTo_string(el->right, current_height + 1, obj_to_str, max_height);
    }
    return res_str;
}

template<typename T>
std::string RB_tree<T>::to_string(obj_to_str_func obj_to_str, int max_height)
{
    update_height();
    if (max_height == -1 || max_height > height)
        max_height = height;
    return  "Size: " + std::to_string(size) + "\nHeight: " + std::to_string(height) + "\nElements:\n" + doTo_string(root, 1, obj_to_str, max_height);
}

template<typename T>
typename RB_tree<T>::Tree_element * RB_tree<T>::min(Tree_element * _root)
{
    if (_root->left == nullptr)
        return _root;
    return min(_root->left);
}

template<typename T>
typename RB_tree<T>::Tree_element * RB_tree<T>::max(Tree_element * _root)
{
    if (_root->right == nullptr)
        return _root;
    return max(_root->right);
}

template<typename T>
void RB_tree<T>::correct_parent(Tree_element * parent, Tree_element * child, comparator_func comp)
{
    if (parent != root)
    {
        if (comp(parent->parent->data, parent->data) < 0)
            parent->parent->right = child;
        else
            parent->parent->left = child;
    }
    else
        root = child;
}

template<typename T>
void RB_tree<T>::update_height()
{
    int new_height{};
    doUpdate_height(root, 1, new_height);
    height = new_height;
}

template<typename T>
void RB_tree<T>::doUpdate_height(Tree_element * el, int level_height, int & max_height)
{
    if (el != nullptr)
    {
        doUpdate_height(el->left, level_height + 1, max_height);
        max_height = max_height < level_height ? level_height : max_height;
        doUpdate_height(el->right, level_height + 1, max_height);
        max_height = max_height < level_height ? level_height : max_height;
    }
}

template<typename T>
const T * RB_tree<T>::find(const T & _data, comparator_func comp)
{
    Tree_element * find_element = doFind(_data, comp);
    if (find_element != nullptr)
        return &(find_element->data);
    else
        return nullptr;
};

template<typename T>
void RB_tree<T>::rotate_right(Tree_element * parent, Tree_element * child, comparator_func comp)
{
    correct_parent(parent, child, comp);
    child->parent = parent->parent;
    
    parent->left = child->right;
    if (child->right)
        child->right->parent = parent;
    
    child->right = parent;
    parent->parent = child;
}

template<typename T>
void RB_tree<T>::rotate_left(Tree_element * parent, Tree_element * child, comparator_func comp)
{
    correct_parent(parent, child, comp);
    child->parent = parent->parent;
    
    parent->right = child->left;
    if (child->left)
        child->left->parent = parent;
    
    child->left = parent;
    parent->parent = child;
}

template<typename T>
void RB_tree<T>::fix_structure(Tree_element * el, comparator_func comp)
{
    Tree_element * uncle;
    while (el != root && el->parent->color == Color::RED)
    {
        if (el->parent == el->parent->parent->left)
        {
            uncle = el->parent->parent->right;
            if (uncle == nullptr || uncle->color == Color::BLACK)
            {
                if (el->parent->right == el)
                {
                    rotate_left(el->parent, el, comp);
                    el = el->left;
                }
                el->parent->color = Color::BLACK;
                el->parent->parent->color = Color::RED;
                rotate_right(el->parent->parent, el->parent, comp);
            }
            else
            {
                el->parent->color = Color::BLACK;
                uncle->color = Color::BLACK;
                el->parent->parent->color = Color::RED;
                el = el->parent->parent;
            }
        }
        else
        {
            uncle = el->parent->parent->left;
            if (uncle == nullptr || uncle->color == Color::BLACK)
            {
                if (el->parent->left == el)
                {
                    rotate_right(el->parent, el, comp);
                    el = el->right;
                }
                el->parent->color = Color::BLACK;
                el->parent->parent->color = Color::RED;
                rotate_left(el->parent->parent, el->parent, comp);
            }
            else
            {
                el->parent->color = Color::BLACK;
                uncle->color = Color::BLACK;
                el->parent->parent->color = Color::RED;
                el = el->parent->parent;
            }
        }
    }
    //
    root->color = Color::BLACK;
}

template<typename T>
int RB_tree<T>::get_height()
{
    update_height();
    return height;
}
#endif
