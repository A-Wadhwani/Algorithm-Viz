#ifndef array_view_hh
#define array_view_hh

#include "olcPixelGameEngine.hh"

namespace AlgorithmViz
{
    class ArrayView : public olc::PixelGameEngine
    {
    private:
        float g_TimeSinceFrame = 0;
        float g_FrameRate = 60;
        
    public:
        ArrayView();

    public:
        bool OnUserCreate() override;
        bool OnUserUpdate(float fElapsedTime) override;
    };
};

#endif
