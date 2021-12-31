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
        EDGE_COL(231, 111, 81);

    class Command
    {
    public:
        int u;
        int v; // -1 if just selecting vertex.
        olc::Pixel color;

        // For edge
        Command(int u, int v, olc::Pixel color)
        {
            this->u = u;
            this->v = v;
            this->color = color;
        }

        // For vertex
        Command(int u, olc::Pixel color)
        {
            this->u = u;
            this->v = -1;
            this->color = color;
        }

        bool isEdge()
        {
            return v != -1;
        }
    };

    class Commands
    {
    public:
        vector<Command> commands;

        Commands()
        {
            commands = vector<Command>();
        }

        Commands(int u, olc::Pixel color)
        {
            commands = vector<Command>();
            commands.push_back(Command(u, color));
        }

        Commands(int u, int v, olc::Pixel color)
        {
            commands = vector<Command>();
            commands.push_back(Command(u, v, color));
        }

        void add(int u, int v, olc::Pixel color)
        {
            commands.push_back(Command(u, v, color));
        }

        void add(int u, olc::Pixel color)
        {
            commands.push_back(Command(u, color));
        }

        Commands &operator+=(Command other)
        {
            commands.push_back(other);
            return *this;
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
        /* Graph Info */
        int num_v;
        int num_e;
        bool is_directed;
        vector<vector<Edge>> adj_list;

        /* Runtime Settings */
        double g_TimeSinceFrame = 0;
        double g_FrameRate = 10;

        /* Visualization Settings */
        vector<pair<int, int>> vertex_locations;
        int vertex_radius;

        /* Command Queue */
        vector<Commands> pending_operations;
        pthread_mutex_t mutex;

        bool _update_edge(int u, int v, int w);
        bool _remove_edge(int u, int v);
        void draw_vertex(int v);
        void draw_edge(int u, int v, int w);

    public:
        GraphView(int);
        GraphView(int, bool);
        GraphView(string);
        bool OnUserCreate() override;
        bool OnUserUpdate(float fElapsedTime) override;
        //bool OnUserDestroy() override;
        void createGraphView();
        void randomized(double density, int max_weight);
        void add_edge(int u, int v, int w);
        void add_edge(int u, int v);
        bool is_edge(int u, int v);
        Edge get_edge(int u, int v);
        bool update_edge(int u, int v, int w);
        bool remove_edge(int u, int v);
        bool directed();
        void print_graph();
    };
}

#endif