#include "graph_main.hh"

using namespace AlgorithmViz;

int main()
{
    Graph g(7, GRAPH_UNDIRECTED);
    g.add_edge(0, 1, 7);
    g.add_edge(1, 2, 8);
    g.add_edge(0, 3, 5);
    g.add_edge(1, 3, 9);
    g.add_edge(1, 4, 7);
    g.add_edge(2, 4, 5);
    g.add_edge(3, 4, 15);
    g.add_edge(3, 5, 6);
    g.add_edge(4, 5, 8);
    g.add_edge(4, 6, 9);
    g.add_edge(5, 6, 11);
    g.print_graph();
}