#include "graph_main.hh"

using namespace AlgorithmViz;

int main()
{
	GraphView view(10, GRAPH_DIRECTED);
	if (view.Construct(800, 600, 1, 1, false, false))
	{
		// Make thread to do sorting.
		view.Start(); // Program doesn't return until GUI closes.
	}
}