#include "array_view.hh"
#include <random>
#include <chrono>
#include <algorithm>

using namespace AlgorithmViz;

ArrayView::ArrayView(int size)
{
    array_size = size;
    array = vector<int>();
    for (int i = 0; i < size; i++)
    {
        array.push_back(i + 1);
    }

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    shuffle(array.begin(), array.end(), default_random_engine(seed));

    g_TimeSinceFrame = 0;
    sAppName = "Algorithm Visualization: Array View";
}

bool ArrayView::OnUserCreate()
{
    createArrayView();
    return true;
}

bool ArrayView::OnUserUpdate(float fElapsedTime)
{
    if (g_TimeSinceFrame < 1.00 / g_FrameRate)
    {
        g_TimeSinceFrame += fElapsedTime;
        return true;
    }
    g_TimeSinceFrame = 0;

    return true;
}

void ArrayView::createArrayView()
{
    double start_w = 5;
    double end_w = ScreenWidth() - 5;
    double box_size = (end_w - start_w) / array_size; // Width of each array box

    double start_h = 5;
    double end_h = ScreenHeight() - 5;
    double height = end_h - start_h;
    double box_increment = (end_h - start_h) / array_size; // Height of each element in the

    Clear(olc::Pixel(255, 165, 152));

    double x = start_w;
    double y = start_h;
    for (int element : array)
    {
        FillRect(x, height - (box_increment * element), box_size, (box_increment * element), olc::Pixel(150, 157, 255));
        DrawRect(x, height - (box_increment * element), box_size, (box_increment * element), olc::Pixel(22, 103, 120));

        if (box_size > 2)
        {
            DrawRect(x + 1, height - (box_increment * element) + 1, box_size - 2,
                     (box_increment * element) - 2, olc::Pixel(22, 103, 120));
            // Double thickness
        }
        x += box_size;
    }
}