#ifndef graph_view_hh
#define graph_view_hh

#include "olcPixelGameEngine.hh"
#include <pthread.h>
#include <unistd.h>
#include <vector>
#include <string>
#include <random>
#include <chrono>
#include <thread>
#include <algorithm>

using namespace std;

namespace AlgorithmViz
{
    static const olc::Pixel
        NODE_COL(0xEA9172),
        NODE_BORDER(0x772953),
        BACKGROUND_COL(0xAD7E97);

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
    class Graph
    {
    private:
        int num_v;
        bool is_directed;
        vector<vector<Edge>> adj_list;

        bool _update_edge(int u, int v, int w)
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

        bool _remove_edge(int u, int v)
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

    public:
        Graph(int V)
        {
            this->num_v = V;
            this->is_directed = false;
            this->adj_list = vector<vector<Edge>>(V);
        }

        Graph(int V, bool is_directed)
        {
            this->num_v = V;
            this->is_directed = is_directed;
            this->adj_list = vector<vector<Edge>>(V);
        }

        Graph(string filename)
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

        void add_edge(int u, int v, int w)
        {
            adj_list[u].push_back(Edge(u, v, w));
            if (!is_directed)
            {
                adj_list[v].push_back(Edge(v, u, w));
            }
        }

        void add_edge(int u, int v)
        {
            add_edge(u, v, 1);
        }

        bool is_edge(int u, int v)
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

        Edge get_edge(int u, int v)
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

        bool update_edge(int u, int v, int w)
        {
            if (is_directed || _update_edge(v, u, w))
            {
                return _update_edge(u, v, w);
            }
            return false;
        }

        bool remove_edge(int u, int v)
        {
            if (is_directed || _remove_edge(v, u))
            {
                return _remove_edge(u, v);
            }
            return false;
        }

        bool directed()
        {
            return is_directed;
        }

        void print_graph()
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
    };
};

#endif