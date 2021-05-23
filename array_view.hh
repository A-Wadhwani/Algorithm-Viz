#ifndef array_view_hh
#define array_view_hh

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.hh"

namespace AlgorithmViz
{
    class ArrayView : public olc::PixelGameEngine
    {
    public:
        ArrayView();

    public:
        bool OnUserCreate() override;
        bool OnUserUpdate(float fElapsedTime) override;
    };
}

#endif
