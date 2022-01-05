#include "graph_view.hh"

using namespace AlgorithmViz;

GraphView::GraphView(int V)
{
    this->num_v = V;
    this->num_e = 0;
    this->is_directed = false;
    this->adj_list = vector<vector<Edge>>(V);
    pthread_mutex_init(&mutex, NULL);
}

GraphView::GraphView(int V, bool is_directed)
{
    this->num_v = V;
    this->num_e = 0;
    this->is_directed = is_directed;
    this->adj_list = vector<vector<Edge>>(V);
    pthread_mutex_init(&mutex, NULL);
}

GraphView::GraphView(string filename)
{
    ifstream file(filename);
    string line;
    getline(file, line);
    num_v = stoi(line);
    num_e = 0;
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
    pthread_mutex_init(&mutex, NULL);
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
        double x = radius + (x_range - y_range) / 2 + radius * cos(angle);
        double y = (min_y + y_range / 2) + radius * sin(angle);
        vertex_locations.push_back(pair<double, double>(x, y));
    }

    // Calculate radius of nodes:
    double circumference = 2 * M_PI * radius;
    vertex_radius = min(ceil(circumference / (num_v * 3)), min_y);

    // Display Settings
    g_FrameRate = 5;
    g_TimeSinceFrame = 0;
    sAppName = "Algorithm Visualization: Graph View";

    // Setting up mutex
    createGraphView();
    draw_header();
    return true;
}

bool GraphView::OnUserUpdate(float fElapsedTime)
{
    if (GetKey(olc::Key::Q).bReleased)
    {
        return false; // Quit viz if 'Q' is pressed
    }
    if (GetKey(olc::Key::RIGHT).bReleased)
    {
        g_FrameRate += 5;
    }
    if (GetKey(olc::Key::LEFT).bReleased)
    {
        g_FrameRate -= 5;
        if (g_FrameRate < 0)
        {
            g_FrameRate = 0;
        }
    }
    if (g_TimeSinceFrame < 1.00 / g_FrameRate)
    {
        g_TimeSinceFrame += fElapsedTime;
        return true;
    }
    g_TimeSinceFrame = 0;
    pthread_mutex_lock(&mutex);
    if (!pending_operations.empty())
    {
        Command command = pending_operations[0];
        if (command.is_edge())
        {
            createGraphView();
            if (command.color_1 != olc::BLANK)
            {
                draw_edge(command.u, command.v, command.w, command.color_1, command.color_2);
                draw_vertex(command.u);
                draw_vertex(command.v);
            }
        }
        else
        {
            draw_vertex(command.u, command.color_1, command.color_2);
        }
        pending_operations.erase(pending_operations.begin());
    }
    else
    {
        createGraphView();
    }
    draw_header();
    pthread_mutex_unlock(&mutex);
    return true;
}

bool GraphView::OnUserDestroy()
{
    exit(EXIT_SUCCESS);
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

void GraphView::draw_header()
{
    DrawStringProp(15, 15, "Graph Viz", TEXT_COL, 3);
    ostringstream fps;
    fps.precision(0);
    fps << fixed << g_FrameRate;
    DrawStringProp(15, 40, "FPS: " + fps.str(), TEXT_COL, 2);
}

void GraphView::draw_vertex(int v)
{
    draw_vertex(v, NODE_COL, NODE_BORDER);
}

void GraphView::draw_vertex(int v, olc::Pixel col, olc::Pixel border)
{
    pair<double, double> vertex = vertex_locations[v];
    FillCircle(vertex.first, vertex.second, vertex_radius, col);
    DrawCircle(vertex.first, vertex.second, vertex_radius, border);
}

void GraphView::draw_edge(int u, int v, int w)
{
    draw_edge(u, v, w, EDGE_COL, EDGE_WEIGHT);
}

void GraphView::draw_edge(int u, int v, int w, olc::Pixel edge, olc::Pixel weight)
{
    pair<double, double> u_loc = vertex_locations[u];
    pair<double, double> v_loc = vertex_locations[v];
    // Draw Line
    DrawLine(u_loc.first, u_loc.second,
             v_loc.first, v_loc.second, edge);
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
                     p3.second, p4.first, p4.second, edge);
    }
    if (num_e < 100)
    {
        // Drawing Weight
        if (is_directed || u < v)
        {
            pair<double, double> p = pair<double, double>(
                v_loc.first - 4 * vertex_radius * cos(angle),
                v_loc.second - 4 * vertex_radius * sin(angle));
            DrawString(p.first, p.second, to_string(w), weight, 2);
        }
    }
}

bool GraphView::_update_edge(int u, int v, int w)
{
    pthread_mutex_lock(&mutex);
    for (auto &e : adj_list[u])
    {
        if (e.v == v)
        {
            pending_operations.push_back(Command(u, v, w, EDGE_SELECT, EDGE_SELECT));
            pending_operations.push_back(Command(u, v, w, EDGE_COL, EDGE_WEIGHT));
            e.w = w;
            pthread_mutex_unlock(&mutex);
            while (!pending_operations.empty())
            {
                this_thread::sleep_for(chrono::milliseconds(10));
                this_thread::yield();
            } // Wait for GUI to present it.
            return true;
        }
    }
    pthread_mutex_unlock(&mutex);
    return false;
}

bool GraphView::_remove_edge(int u, int v)
{
    pthread_mutex_lock(&mutex);
    for (int i = 0; i < adj_list[u].size(); i++)
    {
        if (adj_list[u][i].v == v)
        {
            adj_list[u].erase(adj_list[u].begin() + i);
            num_e--;
            pending_operations.push_back(
                Command(u, v, 0, olc::BLANK, olc::BLANK));
            pthread_mutex_unlock(&mutex);
            while (!pending_operations.empty())
            {
                this_thread::sleep_for(chrono::milliseconds(10));
                this_thread::yield();
            } // Wait for GUI to present it.
            return true;
        }
    }
    pthread_mutex_unlock(&mutex);
    return false;
}

void GraphView::add_edge(int u, int v, int w)
{
    if (is_edge(u, v))
    {
        update_edge(u, v, w);
        return;
    }
    pthread_mutex_lock(&mutex);
    adj_list[u].push_back(Edge(u, v, w));
    num_e++;
    if (!is_directed)
    {
        adj_list[v].push_back(Edge(v, u, w));
        num_e++;
    }
    pending_operations.push_back(
        Command(u, v, w, EDGE_SELECT, EDGE_SELECT));
    pending_operations.push_back(Command(u, v, w, EDGE_COL, EDGE_WEIGHT));
    pthread_mutex_unlock(&mutex);
    while (!pending_operations.empty())
    {
        this_thread::sleep_for(chrono::milliseconds(10));
        this_thread::yield();
    } // Wait for GUI to present it.
}

void GraphView::add_edge(int u, int v)
{
    add_edge(u, v, 1);
}

vector<Edge> GraphView::get_edges(int u)
{
    return adj_list[u];
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
    pthread_mutex_lock(&mutex);
    for (Edge e : adj_list[u])
    {
        if (e.v == v)
        {
            pending_operations.push_back(
                Command(u, v, e.w, EDGE_SELECT, EDGE_SELECT));
            pending_operations.push_back(
                Command(u, v, e.w, EDGE_COL, EDGE_WEIGHT));
            pthread_mutex_unlock(&mutex);
            while (!pending_operations.empty())
            {
                this_thread::sleep_for(chrono::milliseconds(10));
                this_thread::yield();
            } // Wait for GUI to present it.
            return e;
        }
    }
    pthread_mutex_unlock(&mutex);
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
                int w = rand() % max_weight + 1;
                adj_list[i].push_back(Edge(i, j, w));
                if (!is_directed)
                {
                    adj_list[j].push_back(Edge(j, i, w));
                    num_e++;
                }
                num_e++;
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

void GraphView::mark_vertex(int v)
{
    pthread_mutex_lock(&mutex);
    pending_operations.push_back(
        Command(v, VERTEX_SELECT, NODE_BORDER));
    pending_operations.push_back(
        Command(v, NODE_COL, NODE_BORDER));
    pthread_mutex_unlock(&mutex);
    while (!pending_operations.empty())
    {
        this_thread::sleep_for(chrono::milliseconds(10));
        this_thread::yield();
    }
}

void GraphView::mark_edge(int u, int v, int w)
{
    pthread_mutex_lock(&mutex);
    pending_operations.push_back(
        Command(u, v, w, EDGE_SELECT, EDGE_SELECT));
    pending_operations.push_back(
        Command(u, v, w, EDGE_COL, TEXT_COL));
    pthread_mutex_unlock(&mutex);
    while (!pending_operations.empty())
    {
        this_thread::sleep_for(chrono::milliseconds(10));
        this_thread::yield();
    }
}
