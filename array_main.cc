#include "array_main.hh"
#include <thread>

using namespace AlgorithmViz;

void sort_view(ArrayView *view, SortOptions sort)
{
	while (!view->olc_IsRunning())
	{
		std::this_thread::yield();
	}
	auto start = std::chrono::system_clock::now();
	switch (sort)
	{
	case BUBBLE_SORT:
		bubble_sort(view);
		break;
	case INSERTION_SORT:
		insertion_sort(view);
		break;
	}
	auto end = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = end - start;
	cout << "Elapsed time: " << elapsed_seconds.count() << "s\n";
}

void insertion_sort(ArrayView *view)
{
	int length = view->getSize();
	for (int i = 0; i < length; ++i)
	{
		int key = view->getElement(i);
		int j = i - 1;

		while (j >= 0 && view->getElement(j) > key)
		{
			view->putElement(j + 1, view->getElement(j));
			j--;
		}
		view->putElement(j + 1, key);
	}
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

	SortOptions sort_option = INSERTION_SORT;
	if (argc >= 2)
	{
		switch (argv[1][1])
		{
		case 'i':
			sort_option = INSERTION_SORT;
			break;
		case 'b':
			sort_option = BUBBLE_SORT;
			break;
		case 's':
			sort_option = SELECTION_SORT;
			break;
		case 'm':
			sort_option = MERGE_SORT;
			break;
		case 'q':
			sort_option = QUICK_SORT;
			break;
		case 'h':
			sort_option = SHELL_SORT;
			break;
		}
	}

	if (argc >= 3)
	{
		num_elements = atoi(argv[2]);
	}
	ArrayView view = ArrayView(num_elements);

	if (argc == 4)
	{
		view.setFrameRate(atoi(argv[3]));
	}

	if (view.Construct(720, 720, 4, 4, false, false))
	{
		// Make thread to do sorting.
		std::thread sort = std::thread(sort_view, &view,  sort_option);

		view.Start(); // Program doesn't return until GUI closes.
		sort.join();
	}
	return 0;
}
