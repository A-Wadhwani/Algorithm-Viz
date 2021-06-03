#include "array_main.hh"
#include <thread>

using namespace AlgorithmViz;

void sort_view(ArrayView *view)
{
	while (!view->olc_IsRunning())
	{
		std::this_thread::yield();
	}
	bubble_sort(view);
}

void bubble_sort(ArrayView *view)
{
	int length = view->getSize();
	for (int i = 0; i < length - 1; i++)
	{
		for (int j = 0; j < length - i - 1; j++)
		{
			if (view->compareElements(j, j + 1) == 1)
			{
				view->swapElements(j, j + 1);
			}
		}
	}
}

int main(int argc, char **argv)
{
	int num_elements = 20;
	if (argc >= 2)
	{
		num_elements = atoi(argv[1]);
	}
	ArrayView view = ArrayView(num_elements);

	if (argc == 3)
	{
		view.setFrameRate(atoi(argv[2]));
	}

	if (argc == 4){
		view.setFrameMode(strcmp(argv[3], "-f") == 0);
	}
	if (view.Construct(720, 720, 4, 4, false, false))
	{
		// Make thread to do sorting.
		std::thread sort = std::thread(sort_view, &view);

		view.Start(); // Program doesn't return until GUI closes.
		sort.join();
	}
	return 0;
}
