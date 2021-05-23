#include "array_view.hh"

using namespace AlgorithmViz;

ArrayView::ArrayView()
{
    g_TimeSinceFrame = 0;
    sAppName = "Algorithm Visualization: Array View";
    // olc_ConfigureSystem();
}

bool ArrayView::OnUserCreate()
{
    return true;
}

bool ArrayView::OnUserUpdate(float fElapsedTime)
{
    if (g_TimeSinceFrame < 1.00 / g_FrameRate){
        g_TimeSinceFrame += fElapsedTime;
        return true;
    } 
    g_TimeSinceFrame = 0;
    uint8_t r = rand() % 255;
    uint8_t g = rand() % 255;
    uint8_t b = rand() % 255;
    for (int x = 0; x < ScreenWidth(); x++)
    {
        for (int y = 0; y < ScreenHeight(); y++)
            Draw(x, y, olc::Pixel(r, g, b));
    }
    return true;
}
