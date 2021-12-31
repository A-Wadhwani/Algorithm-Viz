#include "graph_view.hh"

using namespace AlgorithmViz;

GraphView::GraphView(int V)
{
    this->num_v = V;
    this->num_e = 0;
    this->is_directed = false;
    this->adj_list = vector<vector<Edge>>(V);
}

GraphView::GraphView(int V, bool is_directed)
{
    this->num_v = V;
    this->num_e = 0;
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
    // Calculate Locations of Vertices
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

    // Calculate radius of nodes:
    double circumference = 2 * M_PI * radius;
    vertex_radius = min(ceil(circumference / (num_v * 3)), min_y);

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
    randomized(0.01 * (rand() % 100), 100);
    createGraphView();
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
    FillCircle(vertex.first, vertex.second, vertex_radius, NODE_COL);
    DrawCircle(vertex.first, vertex.second, vertex_radius, NODE_BORDER);
}

void GraphView::draw_edge(int u, int v, int w)
{
    pair<double, double> u_loc = vertex_locations[u];
    pair<double, double> v_loc = vertex_locations[v];
    // Draw Line
    DrawLine(u_loc.first, u_loc.second,
             v_loc.first, v_loc.second, EDGE_COL);
    // Angle of slope
    double angle = atan2(v_loc.second - u_loc.second,
                         v_loc.first - u_loc.first);
    // Drawing Directed Arrow
    if (is_directed)
    {
        double triangle_side = vertex_radius / 2;
        // Intersection of line and circle
        pair<double, double> p1 = pair<double, double>(
            v_loc.first - vertex_radius * cos(angle),
            v_loc.second - vertex_radius * sin(angle));
        // To find other two points of triangle
        pair<double, double> p3 = pair<double, double>(
            p1.first - triangle_side * cos(angle + M_PI / 4),
            p1.second - triangle_side * sin(angle + M_PI / 4));
        pair<double, double> p4 = pair<double, double>(
            p1.first - triangle_side * cos(angle - M_PI / 4),
            p1.second - triangle_side * sin(angle - M_PI / 4));
        FillTriangle(p1.first, p1.second, p3.first,
                     p3.second, p4.first, p4.second, EDGE_COL);
    }
    if (num_e < 40)
    {
        // Drawing Weight
        pair<double, double> p = pair<double, double>(
            v_loc.first - 4 * vertex_radius * cos(angle),
            v_loc.second - 4 * vertex_radius * sin(angle));
        DrawString(p.first, p.second, to_string(w), NODE_BORDER, 2);
    }
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
    num_e++;
    if (!is_directed)
    {
        adj_list[v].push_back(Edge(v, u, w));
    }
    num_e++;
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

void GraphView::randomized(double density, int max_weight)
{
    // Clear graph
    num_e = 0;
    adj_list.clear();
    adj_list.resize(num_v);
    srand(time(NULL));
    // Generate random edges
    for (int i = 0; i < num_v; i++)
    {
        for (int j = 0; j < num_v; j++)
        {
            if (i != j && rand() % 100 < density * 100)
            {
                add_edge(i, j, rand() % max_weight + 1);
            }
        }
    }
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