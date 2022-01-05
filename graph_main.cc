#include "graph_main.hh"

using namespace AlgorithmViz;

void DFS(GraphView *view, int v, bool visited[])
{
	visited[v] = true;

	for (auto i : view->get_edges(v))
	{
		if (!visited[i.v])
		{
			DFS(view, i.v, visited);
		}
	}
}

bool is_connected(GraphView *view)
{
	bool visited[view->num_v];
	DFS(view, 0, visited);
	for (int i = 0; i < view->num_v; i++)
	{
		if (!visited[i])
			return false;
	}
	return true;
}

void reverse_delete(GraphView *view)
{
	vector<Edge> edges;
	for (int i = 0; i < view->num_v; i++)
	{
		for (auto j : view->get_edges(i))
		{
			edges.push_back(j);
		}
	}
	sort(edges.begin(), edges.end(), [](Edge a, Edge b)
		 { return a.w > b.w; });
	for (auto i : edges)
	{
		view->remove_edge(i.u, i.v);
		if (is_connected(view) == false)
		{
			view->add_edge(i.u, i.v, i.w);
		}
	}
	cout<<"Reverse Delete"<<endl;
	cout<<view->num_e<<endl;
}

void run(GraphView *view)
{
	view->g_FrameRate = 1000;
	reverse_delete(view);
}

int main()
{
	GraphView view(10, GRAPH_UNDIRECTED);
	view.randomized(0.95, 100);
	if (view.Construct(800, 600, 1, 1, false, false))
	{
		// Make thread to do sorting.
		thread t(run, &view);
		view.Start(); // Program doesn't return until GUI closes.
		t.join();
	}
}