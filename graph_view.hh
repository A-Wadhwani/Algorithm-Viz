#ifndef graph_view_hh
#define graph_view_hh
#define _USE_MATH_DEFINES

#include "olcPixelGameEngine.hh"
#include <pthread.h>
#include <unistd.h>
#include <vector>
#include <string>
#include <random>
#include <chrono>
#include <thread>
#include <cmath>
#include <algorithm>

using namespace std;

namespace AlgorithmViz
{
    static const olc::Pixel
        NODE_COL(42, 157, 143),
        NODE_BORDER(38, 70, 83),
        BACKGROUND_COL(233, 196, 106),
        EDGE_COL(231, 111, 81),
        EDGE_WEIGHT(38, 70, 83),
        EDGE_SELECT(0, 30, 255),
        VERTEX_SELECT(77, 25, 77),
        TEXT_COL(2, 41, 70);

    class Command
    {
    public:
        int u;
        int v; // -1 if just selecting vertex.
        int w;
        olc::Pixel color_1;
        olc::Pixel color_2;

        // For edge
        Command(int u, int v, int w,
                olc::Pixel edge_col, olc::Pixel weight_col)
        {
            this->u = u;
            this->v = v;
            this->w = w;
            this->color_1 = edge_col;
            this->color_2 = weight_col;
        }

        // For vertex
        Command(int u, olc::Pixel node_col,
                olc::Pixel border_col)
        {
            this->u = u;
            this->v = -1;
            this->color_1 = node_col;
            this->color_2 = border_col;
        }

        bool is_edge()
        {
            return v != -1;
        }
    };

    struct Edge
    {
        int u;
        int v;
        int w;
        Edge(int u, int v, int w)
        {
            this->u = u;
            this->v = v;
            this->w = w;
        }
    };

    // Adjacency list graph
    class GraphView : public olc::PixelGameEngine
    {
    private:
        /* Runtime Settings */
        double g_TimeSinceFrame = 0;

        /* Visualization Settings */
        vector<pair<int, int>> vertex_locations;
        int vertex_radius;

        /* Command Queue */
        vector<Command> pending_operations;
        pthread_mutex_t mutex;

        bool _update_edge(int u, int v, int w);
        bool _remove_edge(int u, int v);
        void createGraphView();
        void draw_header();
        void draw_vertex(int v);
        void draw_vertex(int v, olc::Pixel col, olc::Pixel border);
        void draw_edge(int u, int v, int w);
        void draw_edge(int u, int v, int w, olc::Pixel edge, olc::Pixel weight);

    public:
        /* Graph Info */
        bool is_directed;
        vector<vector<Edge>> adj_list;
        int num_v;
        int num_e;

        /* Constructors */
        GraphView(int);
        GraphView(int, bool);
        GraphView(string);

        /* GUI */
        double g_FrameRate = 10;
        bool OnUserCreate() override;
        bool OnUserUpdate(float fElapsedTime) override;
        bool OnUserDestroy() override;

        /* Graph Methods */
        void randomized(double density, int max_weight);
        void add_edge(int u, int v, int w);
        void add_edge(int u, int v);
        vector<Edge> get_edges(int u);
        bool is_edge(int u, int v);
        Edge get_edge(int u, int v);
        bool update_edge(int u, int v, int w);
        bool remove_edge(int u, int v);
        bool directed();
        void print_graph();

        // Nice tools
        void mark_vertex(int v);
        void mark_edge(int u, int v, int w);
    };
}

#endif