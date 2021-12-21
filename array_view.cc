#include "array_view.hh"

using namespace AlgorithmViz;

ArrayView::ArrayView(int size, string sort_name)
{
    // Array generation
    array_size = size;
    array = vector<int>();
    for (int i = 0; i < array_size; i++)
    {
        array.push_back(i + 1);
    }

    sort = sort_name;
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
    start_w = 10;
    end_w = ScreenWidth() - 10;
    box_size = (end_w - start_w) / array_size; // Width of each array box

    start_h = 10;
    end_h = ScreenHeight() - 10;
    height = end_h - start_h;
    box_increment = (end_h - start_h) / array_size; // Height of each element in the

    createArrayView();
    return true;
}

bool ArrayView::OnUserUpdate(float fElapsedTime)
{
    if (GetKey(olc::Key::Q).bReleased)
    {
        return false; // Quit game if 'Q' is pressed
    }
    if (GetKey(olc::Key::RIGHT).bReleased)
    {
        g_FrameRate += 5;
    }
    if (GetKey(olc::Key::LEFT).bReleased)
    {
        g_FrameRate -= 5;
        if (g_FrameRate < 0) {
            g_FrameRate = 0;
        }
    }
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
            drawElement(pos1, array[pos1], SWAP_START_COL, OUTER_COL);
            drawElement(pos2, array[pos2], SWAP_START_COL, OUTER_COL);
            if (!g_fastmode)
                break;
        }
        case SWAP_SWITCH:
        {
            drawElement(pos1, array[pos1], BACKGROUND_COL, BACKGROUND_COL);
            drawElement(pos2, array[pos2], BACKGROUND_COL, BACKGROUND_COL);
            int temp = array[pos1];
            array[pos1] = array[pos2];
            array[pos2] = temp;
            drawElement(pos1, array[pos1], SWAP_SWITCH_COL, OUTER_COL);
            drawElement(pos2, array[pos2], SWAP_SWITCH_COL, OUTER_COL);
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
            drawElement(pos1, array[pos1], COMPARE_START_COL, OUTER_COL);
            drawElement(pos2, array[pos2], COMPARE_START_COL, OUTER_COL);
            if (!g_fastmode)
                break;
        }
        case COMPARE_MARK:
        {

            if (array[pos1] > array[pos2])
            {
                drawElement(pos1, array[pos1], COMPARE_GREATER_COL, OUTER_COL);
                drawElement(pos2, array[pos2], COMPARE_LESSER_COL, OUTER_COL);
            }
            else if (array[pos1] < array[pos2])
            {
                drawElement(pos2, array[pos2], COMPARE_GREATER_COL, OUTER_COL);
                drawElement(pos1, array[pos1], COMPARE_LESSER_COL, OUTER_COL);
            }
            else
            {
                drawElement(pos1, array[pos1], COMPARE_EQUAL_COL, OUTER_COL);
                drawElement(pos2, array[pos2], COMPARE_EQUAL_COL, OUTER_COL);
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
            drawElement(pos1, array[pos1], GET_START_COL, OUTER_COL);
            if (!g_fastmode)
                break;
        }
        case GET_END:
        {
            createArrayView();
            break;
        }
        case PUT_START:
        {
            drawElement(pos1, array[pos1], PUT_UPDATE_COL, OUTER_COL);
            if (!g_fastmode)
                break;
        }
        case PUT_UPDATE:
        {
            drawElement(pos1, array[pos1], BACKGROUND_COL, BACKGROUND_COL);
            array[pos1] = pos2;
            drawElement(pos1, array[pos1], PUT_UPDATE_COL, OUTER_COL);
            if (!g_fastmode)
                break;
        }
        case PUT_END:
        {
            createArrayView();
            break;
        }
        case SUCCESS:
        {
            createArrayView(SUCCESS_COL, OUTER_COL);
        }
        default:
            break;
        }
        pending_operations.erase(pending_operations.begin());
    }
    pthread_mutex_unlock(&mutex);
    return true;
}

bool ArrayView::OnUserDestroy()
{
    exit(EXIT_SUCCESS);
}

void ArrayView::createArrayView()
{
    createArrayView(INNER_COL, OUTER_COL);
}

void ArrayView::createArrayView(olc::Pixel inner, olc::Pixel outer)
{
    Clear(BACKGROUND_COL);
    for (int i = 0; i < array_size; i++)
    {
        drawElement(i, array[i], inner, outer);
    }
}

void ArrayView::drawElement(int position, int value, olc::Pixel inner, olc::Pixel outer)
{
    int x = start_w + box_size * (position);
    FillRect(x, start_h + height - (box_increment * value), box_size, (box_increment * value), inner);
    DrawRect(x, start_h + height - (box_increment * value), box_size, (box_increment * value), outer);
    DrawStringProp(15, 15, sort, OUTER_COL, 3);
    ostringstream fps;
    fps.precision(0);
    fps << fixed << g_FrameRate;
    DrawStringProp(15, 40, "FPS: " + fps.str(), OUTER_COL, 2);
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
    pending_operations.push_back(Commands(COMPARE_MARK, pos1, pos2));
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
    if (!g_fastmode)
    {
        pending_operations.push_back(Commands(GET_START, position));
        pending_operations.push_back(Commands(GET_END, position));
    }
    while (!pending_operations.empty())
    {
        this_thread::yield();
    }
    return array[position];
}

int ArrayView::putElement(int position, int value)
{
    pending_operations.push_back(Commands(PUT_START, position, value));
    if (!g_fastmode)
    {
        pending_operations.push_back(Commands(PUT_UPDATE, position, value));
        pending_operations.push_back(Commands(PUT_END, position, value));
    }
    while (!pending_operations.empty())
    {
        this_thread::yield();
    }
    return array[position];
}

bool ArrayView::verifyArray()
{
    for (int i = 0; i < array_size - 1; i++)
    {
        if (compareElements(i, i + 1) > 0)
        {
            return false;
        }
    }
    pending_operations.push_back(Commands(SUCCESS, -1, -1));
    return true;
}