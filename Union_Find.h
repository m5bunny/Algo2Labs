#ifndef UNION_FIND_H_
#define UNION_FIND_H_

class Union_Find
{
private:
    int * parents;
    int * ranks;
public:
    Union_Find(int _n);
    ~Union_Find();
    void unite(int i, int j);
    void unite_by_rank(int i, int j);
    int find(int i);
    int find_with_compression(int i);
    
    int get_find_counter() { return find_counter; }
};

Union_Find::Union_Find(int _n)
{
    find_counter = 0;
    
    parents = new int[_n];
    ranks = new int[_n];
    for (int i{}; i < _n; ++i)
    {
        parents[i] = i;
        ranks[i] = 0;
    }
}
Union_Find::~Union_Find()
{
    delete [] parents;
    delete [] ranks;
}

void Union_Find::unite(int i, int j)
{
    parents[i] = j; 
}

void Union_Find::unite_by_rank(int i, int j)
{
    if (ranks[i] >= ranks[j])
    {
        parents[j] = i;
        if(ranks[i] == ranks[j])
            ++ranks[i];
    }
    else
        parents[i] = j;
}

int Union_Find::find(int i)
{
    while (i != parents[i])
        i = parents[i];
    return i;
}

int Union_Find::find_with_compression(int i)
{
    if (i == parents[i])
        return i;
    
    int root = find_with_compression(parents[i]);
    if (root != parents[i])
        parents[i] = root;
    return root;
}
#endif
