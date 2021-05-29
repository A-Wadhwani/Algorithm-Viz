#include "array_view.hh"
#include <random>
#include <chrono>
#include <algorithm>

using namespace AlgorithmViz;

ArrayView::ArrayView(int size)
{
    // Array generation
    array_size = size;
    array = vector<int>();
    for (int i = 0; i < array_size; i++)
    {
        array.push_back(i + 1);
    }

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    shuffle(array.begin(), array.end(), default_random_engine(seed));

    // View Information
    g_TimeSinceFrame = 0;
    sAppName = "Algorithm Visualization: Array View";
}

bool ArrayView::OnUserCreate()
{
    // Display Settings
    start_w = 5;
    end_w = ScreenWidth() - 5;
    box_size = (end_w - start_w) / array_size; // Width of each array box

    start_h = 5;
    end_h = ScreenHeight() - 5;
    height = end_h - start_h;
    box_increment = (end_h - start_h) / array_size; // Height of each element in the

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

    Clear(olc::Pixel(255, 165, 152));

    int pos = 0;
    for (int element : array)
    {
        drawElement(pos, element, olc::INNER_COL, olc::OUTER_COL);
        pos++;
    }
}

void ArrayView::drawElement(int position, int value, olc::Pixel inner, olc::Pixel outer)
{
    int x = start_w + box_size * (position);
    FillRect(x, start_h + height - (box_increment * value), box_size, (box_increment * value), inner);
    DrawRect(x, start_h + height - (box_increment * value), box_size, (box_increment * value), outer);
}

void ArrayView::swapElements(int pos1, int pos2)
{
    
}

int ArrayView::compareElements(int pos1, int pos2)
{
    return 0;
}

int ArrayView::getElement(int position)
{
    return 0;
}