#ifndef array_view_hh
#define array_view_hh

#include "olcPixelGameEngine.hh"
#include <vector>
#include <string>

using namespace std;

namespace AlgorithmViz
{
    class ArrayView : public olc::PixelGameEngine
    {
    private:
        double g_TimeSinceFrame = 0;
        double g_FrameRate = 60;
        vector<int> array;
        int array_size;

        double start_w;
        double end_w;
        double box_size;

        double start_h;
        double end_h;
        double height;
        double box_increment;

    public:
        ArrayView(int size);
        void createArrayView();
        void drawElement(int position, int value, olc::Pixel inner_col, olc::Pixel outer_col);
    public:
        bool OnUserCreate() override;
        bool OnUserUpdate(float fElapsedTime) override;
        void swapElements(int pos1, int pos2);
        int compareElements(int pos1, int pos2);
        int getElement(int position);
    };
};

#endif
