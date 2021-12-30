#include "graph_view.hh"

using namespace AlgorithmViz;

GraphView::GraphView(int V)
{
    this->num_v = V;
    this->is_directed = false;
    this->adj_list = vector<vector<Edge>>(V);
}

GraphView::GraphView(int V, bool is_directed)
{
    this->num_v = V;
    this->is_directed = is_directed;
    this->adj_list = vector<vector<Edge>>(V);
}

GraphView::GraphView(string filename)
{
    ifstream file(filename);
    string line;
    getline(file, line);
    num_v = stoi(line);
    getline(file, line);
    is_directed = stoi(line);
    adj_list = vector<vector<Edge>>(num_v);
    while (getline(file, line))
    {
        stringstream ss(line);
        string token;
        vector<int> tokens;
        while (getline(ss, token, ' '))
        {
            tokens.push_back(stoi(token));
        }
        int u = tokens[0];
        int v = tokens[1];
        int w = tokens[2];
        add_edge(u, v, w);
    }
}

bool GraphView::OnUserCreate()
{

    double min_x = 20;
    double max_x = ScreenWidth() - min_x;
    double min_y = 20;
    double max_y = ScreenHeight() - min_y;
    double x_range = max_x - min_x;
    double y_range = max_y - min_y;
    double radius = y_range / 2;
    for (int i = 0; i < num_v; i++)
    {
        double angle = 2 * M_PI * i / num_v;
        double x = (x_range - y_range) / 2 + (radius) + radius * cos(angle);
        double y = (min_y + y_range / 2) + radius * sin(angle);
        vertex_locations.push_back(pair<double, double>(x, y));
    }

    // Display Settings
    g_FrameRate = 10;
    g_TimeSinceFrame = 0;
    sAppName = "Algorithm Visualization: Graph View";

    // Setting up mutex
    pthread_mutex_init(&mutex, NULL);
    createGraphView();
    return true;
}

bool GraphView::OnUserUpdate(float fElapsedTime)
{
    return true;
}

void GraphView::createGraphView()
{
    Clear(BACKGROUND_COL);
    // Draw edges between nodes
    for (int i = 0; i < num_v; i++)
    {
        for (Edge e : adj_list[i])
        {
            draw_edge(i, e.v, e.w);
        }
    }
    for (int i = 0; i < num_v; i++)
    {
        draw_vertex(i);
    }
}

void GraphView::draw_vertex(int v)
{
    pair<double, double> vertex = vertex_locations[v];
    FillCircle(vertex.first, vertex.second, 10, NODE_COL);
    DrawCircle(vertex.first, vertex.second, 10, NODE_BORDER);
}

void GraphView::draw_edge(int u, int v, int w)
{
    pair<double, double> u_loc = vertex_locations[u];
    pair<double, double> v_loc = vertex_locations[v];
    DrawLine(u_loc.first, u_loc.second, v_loc.first, v_loc.second, EDGE_COL);
}

bool GraphView::_update_edge(int u, int v, int w)
{
    for (Edge e : adj_list[u])
    {
        if (e.v == v)
        {
            e.w = w;
            return true;
        }
    }
    return false;
}

bool GraphView::_remove_edge(int u, int v)
{
    for (int i = 0; i < adj_list[u].size(); i++)
    {
        if (adj_list[u][i].v == v)
        {
            adj_list[u].erase(adj_list[u].begin() + i);
            return true;
        }
    }
    return false;
}

void GraphView::add_edge(int u, int v, int w)
{
    adj_list[u].push_back(Edge(u, v, w));
    if (!is_directed)
    {
        adj_list[v].push_back(Edge(v, u, w));
    }
}

void GraphView::add_edge(int u, int v)
{
    add_edge(u, v, 1);
}

bool GraphView::is_edge(int u, int v)
{
    for (Edge e : adj_list[u])
    {
        if (e.v == v)
        {
            return true;
        }
    }
    return false;
}

Edge GraphView::get_edge(int u, int v)
{
    for (Edge e : adj_list[u])
    {
        if (e.v == v)
        {
            return e;
        }
    }
    return Edge(-1, -1, -1);
}

bool GraphView::update_edge(int u, int v, int w)
{
    if (is_directed || _update_edge(v, u, w))
    {
        return _update_edge(u, v, w);
    }
    return false;
}

bool GraphView::remove_edge(int u, int v)
{
    if (is_directed || _remove_edge(v, u))
    {
        return _remove_edge(u, v);
    }
    return false;
}

bool GraphView::directed()
{
    return is_directed;
}

void GraphView::print_graph()
{
    for (int i = 0; i < num_v; i++)
    {
        cout << i << ": ";
        for (Edge e : adj_list[i])
        {
            cout << "(" << e.v << ", " << e.w << ") ";
        }
        cout << endl;
    }
}