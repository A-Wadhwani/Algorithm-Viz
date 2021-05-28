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
        float g_TimeSinceFrame = 0;
        float g_FrameRate = 60;
        vector<int> array;
        int array_size;

    public:
        ArrayView(int size);
        void createArrayView();

    public:
        bool OnUserCreate() override;
        bool OnUserUpdate(float fElapsedTime) override;
    };
};

#endif
