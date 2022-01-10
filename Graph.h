#ifndef GRAPH_H_
#define GRAPH_H_
#include <fstream>
#include "Dynamic_array.h"
#include "Bucket_sort.h"
#include "Union_Find.h"


class Graph
{
    
private:
    struct Vertex
    {
        float x, y;
        Vertex(float _x, float _y) : x(_x), y(_y) {}
        Vertex() = default;
    };
    struct Edge
    {
        int from, to;
        float cost;
        Edge(int _from, int _to, float _cost) : from(_from), to(_to), cost(_cost) {}
        Edge() = default;
    };
    Dynamic_array<Vertex> vertices;
    Dynamic_array<Edge> edges;
    
    static float get_edge_key(const Edge & e) { return e.cost; }
    static bool more_edge_comp(const Edge & lhs, const Edge & rhs) { return lhs.cost > rhs.cost; }
    
    void sort_edges();
    
public:
    Graph() {}
    Graph(const std::string & _file_name);
    Graph get_mst_kruskal();
    Graph get_mst_kruskal_bad();
    void add(const Vertex & v) { vertices.push_back(v); }
    void add(const Edge & e) { edges.push_back(e); }
    float get_total_cost() const;
    int get_edges_amount() const { return edges.get_size(); }
    int get_vertices_amount() const { return vertices.get_size(); }
};

Graph::Graph(const std::string & _file_name)
{
    std::ifstream input_file(_file_name);
    if (!input_file)
    {
        std::cerr << "Unable to open " + _file_name << std::endl;
        exit(1);
    }
    
    int n, from, to;
    float x, y, cost;
    input_file >> n;
    for (int i{}; i < n; ++i)
    {
        input_file >> x >> y;
        vertices.push_back({x, y});
    }
    input_file >> n;
    for (int i{}; i < n; ++i)
    {
        input_file >> from >> to >> cost;
        edges.push_back({from, to, cost});
    }
}

void Graph::sort_edges()
{
    float max_cost = edges[0].cost;
    for (int i{ 1 }; i < edges.get_size(); ++i)
    {
        if (max_cost < edges[i].cost)
            max_cost = edges[i].cost;
    }
    bucket_sort(edges.get_array_without_remove(), edges.get_size(), max_cost, get_edge_key, more_edge_comp);
}

Graph Graph::get_mst_kruskal()
{
    Graph mst;
    sort_edges();
    Union_Find uf(vertices.get_size());
    mst.vertices = vertices;
    
    int x, y;
    int findc = 0;
    for (int i{}; i < edges.get_size(); ++i)
    {
        findc+=2;
        x = uf.find_with_compression(edges[i].from);
        y = uf.find_with_compression(edges[i].to);
        if (x != y)
        {
            mst.add(edges[i]);
            uf.unite_by_rank(x, y);
        }
    }
    return mst;
}

Graph Graph::get_mst_kruskal_bad()
{
    Graph mst;
    sort_edges();
    Union_Find uf(vertices.get_size());
    mst.vertices = vertices;
    int x, y;
    int findc =0;
    for (int i{}; i < edges.get_size(); ++i)
    {
        findc+=2;
        x = uf.find(edges[i].from);
        y = uf.find(edges[i].to);
        if (x != y)
        {
            mst.add(edges[i]);
            uf.unite(x, y);
        }
    }
    return mst;
}

float Graph::get_total_cost() const
{
    float total_cost{ edges[0].cost };
    for (int i{ 1 }; i < edges.get_size(); ++i)
        total_cost += edges[i].cost;
    return total_cost;
}

#endif
