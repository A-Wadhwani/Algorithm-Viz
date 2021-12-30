#include "graph_main.hh"

using namespace AlgorithmViz;

int main()
{
    GraphView view(7, GRAPH_UNDIRECTED);
    view.add_edge(0, 1, 1);
    view.add_edge(0, 2, 1);
    view.add_edge(1, 2, 1);
	if (view.Construct(800, 600, 1, 1, false, false))
	{
		// Make thread to do sorting.
		view.Start(); // Program doesn't return until GUI closes.
	}
}