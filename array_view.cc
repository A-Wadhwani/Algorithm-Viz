#include "array_view.hh"

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

    // Commands display
    pending_operations = vector<Commands>();

    // Setting up mutex
    pthread_mutex_init(&mutex, NULL);
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

    pthread_mutex_lock(&mutex);
    if (!pending_operations.empty())
    {
        Commands command = pending_operations[0];
        int pos1 = command.index1;
        int pos2 = command.index2;
        switch (command.command)
        {
        case SWAP_START:
        {
            drawElement(pos1, array[pos1], olc::Pixel(254, 185, 96), olc::OUTER_COL);
            drawElement(pos2, array[pos2], olc::Pixel(254, 185, 96), olc::OUTER_COL);
            if (!g_fastmode)
                break;
        }
        case SWAP_SWITCH:
        {
            drawElement(pos1, array[pos1], olc::Pixel(255, 165, 152), olc::Pixel(255, 165, 152));
            drawElement(pos2, array[pos2], olc::Pixel(255, 165, 152), olc::Pixel(255, 165, 152));
            int temp = array[pos1];
            array[pos1] = array[pos2];
            array[pos2] = temp;
            drawElement(pos1, array[pos1], olc::Pixel(254, 249, 0), olc::OUTER_COL);
            drawElement(pos2, array[pos2], olc::Pixel(254, 249, 0), olc::OUTER_COL);
            if (!g_fastmode)
                break;
        }
        case SWAP_END:
        {
            createArrayView();
            break;
        }
        case COMPARE_START:
        {
            drawElement(pos1, array[pos1], olc::Pixel(31, 254, 244), olc::OUTER_COL);
            drawElement(pos2, array[pos2], olc::Pixel(31, 254, 244), olc::OUTER_COL);
            if (!g_fastmode)
                break;
        }
        case COMPARE_MARK:
        {

            if (array[pos1] > array[pos2])
            {
                drawElement(pos1, array[pos1], olc::Pixel(28, 176, 22), olc::OUTER_COL);
                drawElement(pos2, array[pos2], olc::Pixel(176, 46, 26), olc::OUTER_COL);
            }
            else if (array[pos1] < array[pos2])
            {
                drawElement(pos2, array[pos2], olc::Pixel(28, 176, 22), olc::OUTER_COL);
                drawElement(pos1, array[pos1], olc::Pixel(176, 46, 26), olc::OUTER_COL);
            }
            else
            {
                drawElement(pos1, array[pos1], olc::Pixel(175, 255, 96), olc::OUTER_COL);
                drawElement(pos2, array[pos2], olc::Pixel(175, 255, 96), olc::OUTER_COL);
            }
            if (!g_fastmode)
                break;
        }
        case COMPARE_END:
        {
            createArrayView();
            break;
        }
        case GET_START:
        {
            drawElement(pos1, array[pos1], olc::Pixel(175, 255, 96), olc::OUTER_COL);
            if (!g_fastmode)
                break;
        }
        case GET_END:
        {
            createArrayView();
            break;
        }
        default:
            break;
        }
        pending_operations.erase(pending_operations.begin());
    }
    pthread_mutex_unlock(&mutex);
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
    pthread_mutex_lock(&mutex);
    pending_operations.push_back(Commands(SWAP_START, pos1, pos2));
    if (!g_fastmode)
    {
        pending_operations.push_back(Commands(SWAP_SWITCH, pos1, pos2));
        pending_operations.push_back(Commands(SWAP_END, pos1, pos2));
    }
    pthread_mutex_unlock(&mutex);
}

int ArrayView::compareElements(int pos1, int pos2)
{
    pthread_mutex_lock(&mutex);
    pending_operations.push_back(Commands(COMPARE_START, pos1, pos2));
    if (!g_fastmode)
    {
        pending_operations.push_back(Commands(COMPARE_MARK, pos1, pos2));
        pending_operations.push_back(Commands(COMPARE_END, pos1, pos2));
    }
    pthread_mutex_unlock(&mutex);
    while (!pending_operations.empty())
    {
        this_thread::yield();
    }
    if (array[pos1] > array[pos2])
    {
        return 1;
    }
    else if (array[pos1] < array[pos2])
    {
        return -1;
    }
    return 0;
}

int ArrayView::getElement(int position)
{
    pending_operations.push_back(Commands(GET_START, position));
    if (!g_fastmode)
    {
        pending_operations.push_back(Commands(GET_END, position));
    }
    while (!pending_operations.empty())
    {
        this_thread::yield();
    }
    return array[position];
}