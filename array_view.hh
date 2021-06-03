#ifndef array_view_hh
#define array_view_hh

#include "olcPixelGameEngine.hh"
#include <pthread.h>
#include <vector>
#include <string>
#include <random>
#include <chrono>
#include <algorithm>

using namespace std;

namespace AlgorithmViz
{
    enum CommandOptions
    {
        SWAP_START, // 254, 185, 96
        SWAP_SWITCH, // 254, 249, 0
        SWAP_END,
        COMPARE_START, // 31, 254, 244
        COMPARE_MARK, // Greater: 28, 176, 22 | Lesser: 176, 46, 26 | Equal: 175, 255, 96
        COMPARE_END,
        GET_START, // 31, 254, 96
        GET_END
    };

    class Commands
    {
    public:
        CommandOptions command;
        int index1;
        int index2; //If needed

        Commands(CommandOptions command, int index1, int index2)
        {
            this->command = command;
            this->index1 = index1;
            this->index2 = index2;
        }

        Commands(CommandOptions command, int index1)
        {
            this->command = command;
            this->index1 = index1;
            this->index2 = -1;
        }
    };

    class ArrayView : public olc::PixelGameEngine
    {
    private:
        double g_TimeSinceFrame = 0;
        double g_FrameRate = 10;
        bool g_fastmode = false;
        vector<int> array;
        int array_size;

        double start_w;
        double end_w;
        double box_size;

        double start_h;
        double end_h;
        double height;
        double box_increment;

        vector<Commands> pending_operations;
        pthread_mutex_t mutex;

    public:
        ArrayView(int size);
        void createArrayView();
        void drawElement(int position, int value, olc::Pixel inner_col, olc::Pixel outer_col);
        bool OnUserCreate() override;
        bool OnUserUpdate(float fElapsedTime) override;
        void swapElements(int pos1, int pos2);
        int compareElements(int pos1, int pos2);
        int getElement(int position);

        void setFrameRate(double new_framerate)
        {
            g_FrameRate = new_framerate;
            if (g_FrameRate > 40){
                g_fastmode = true;
            }
        }
        int getSize()
        {
            return array_size;
        }
        void setFrameMode(bool mode){
            g_fastmode = mode;
        }
    };
};

#endif
