#include "array_main.hh"

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
	case MERGE_SORT:
		merge_sort(view, 0, view->getSize() - 1);
		break;
	case QUICK_SORT:
		quick_sort(view, 0, view->getSize() - 1);
		break;
	case HEAP_SORT:
		heap_sort(view);
		break;
	case SHELL_SORT:
		shell_sort(view);
		break;
	}
	bool verify = view->verifyArray();
	auto end = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = end - start;
	if (!verify)
	{
		cout << "Sort failed" << endl;
	}
	else
	{
		cout << "Sort completed in " << elapsed_seconds.count() << " seconds" << endl;
	}
}

void insertion_sort(ArrayView *view)
{
	int length = view->getSize();
	for (int i = 1; i < length; ++i)
	{
		int j = i;
		while (j > 0 && view->compareElements(j, j - 1) < 0)
		{
			view->swapElements(j, j - 1);
			j--;
		}
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

void merge_sort(ArrayView *view, int l, int r)
{
	if (l >= r)
	{
		return;
	}
	int m = l + (r - l) / 2;
	merge_sort(view, l, m);
	merge_sort(view, m + 1, r);
	merge(view, l, m, r);
}

void quick_sort(ArrayView *view, int l, int r)
{
	if (l >= r)
	{
		return;
	}
	int m = partition(view, l, r);
	quick_sort(view, l, m - 1);
	quick_sort(view, m + 1, r);
}

void heap_sort(ArrayView *view)
{
	int length = view->getSize();
	for (int i = length / 2 - 1; i >= 0; i--)
	{
		heapify(view, length, i);
	}
	for (int i = length - 1; i >= 0; i--)
	{
		view->swapElements(0, i);
		heapify(view, i, 0);
	}
}

void shell_sort(ArrayView *view)
{
	int length = view->getSize();
	vector<int> intervals = vector<int>{1, 4, 10, 23, 57, 132, 301, 701, 1750};
	for (int k = intervals.size() - 1; k >= 0; k--)
	{
		int interval = intervals[k];
		for (int i = interval; i < length; i += 1)
		{
			int temp = view->getElement(i);
			int j;
			for (j = i; j >= interval && view->getElement(j - interval) > temp; j -= interval)
			{
				view->putElement(j, view->getElement(j - interval));
			}
			view->putElement(j, temp);
		}
	}
}

void merge(ArrayView *view, int l, int m, int r)
{
	int i = l;
	int j = m + 1;
	while (i <= m && j <= r)
	{
		if (view->compareElements(i, j) == -1)
		{
			i++;
		}
		else
		{
			int value = view->getElement(j);
			int index = j;
			while (index != i)
			{
				view->putElement(index, view->getElement(index - 1));
				index--;
			}
			view->putElement(i, value);
			i++;
			m++;
			j++;
		}
	}
}

int partition(ArrayView *view, int l, int r)
{
	// random partition
	int i = l + rand() % (r - l + 1);
	view->swapElements(l, i);
	int m = l;
	for (int j = l + 1; j <= r; j++)
	{
		if (view->compareElements(j, l) == -1)
		{
			m++;
			view->swapElements(m, j);
		}
	}
	view->swapElements(l, m);
	return m;
}

void heapify(ArrayView *view, int length, int i)
{
	int largest = i;
	int l = 2 * i + 1;
	int r = 2 * i + 2;
	if (l < length && view->compareElements(l, largest) == 1)
	{
		largest = l;
	}
	if (r < length && view->compareElements(r, largest) == 1)
	{
		largest = r;
	}
	if (largest != i)
	{
		view->swapElements(i, largest);
		heapify(view, length, largest);
	}
}

// Add usage for this program using getopt
int main(int argc, char **argv)
{
	SortOptions sort_option = QUICK_SORT;
	string sort_name = "Quick Sort";
	int num_elements = 20;
	int frame_rate = 10;
	bool fast_mode = false;

	int opt;
	while ((opt = getopt(argc, argv, "n:s:h:f:m")) != -1)
	{
		switch (opt)
		{
		case 's':
			if (strcmp(optarg, "bubble") == 0)
			{
				sort_option = BUBBLE_SORT;
				sort_name = "Bubble Sort";
			}
			else if (strcmp(optarg, "insertion") == 0)
			{
				sort_option = INSERTION_SORT;
				sort_name = "Insertion Sort";
			}
			else if (strcmp(optarg, "merge") == 0)
			{
				sort_option = MERGE_SORT;
				sort_name = "Merge Sort";
			}
			else if (strcmp(optarg, "quick") == 0)
			{
				sort_option = QUICK_SORT;
				sort_name = "Quick Sort";
			}
			else if (strcmp(optarg, "heap") == 0)
			{
				sort_option = HEAP_SORT;
				sort_name = "Heap Sort";
			}
			else if (strcmp(optarg, "shell") == 0)
			{
				sort_option = SHELL_SORT;
				sort_name = "Shell Sort";
			}
			else
			{
				cout << "Unrecognized sort option.\n";
				return -1;
			}
			break;
		case 'n':
			num_elements = atoi(optarg);
			break;
		case 'f':
			frame_rate = atoi(optarg);
			break;
		case 'm':
			fast_mode = true;
			break;
		case 'h':
		default:
			cout << "Usage: " << argv[0] << " [-s sort_type] [-n num_elements] [-f]\n";
			cout << "  -s sort_type: The sort type to use.\n";
			cout << "  -n num_elements: The number of elements to sort.\n";
			cout << "  -f frame_rate: The frame rate to use.\n";
			cout << "  -m: Use fast mode.\n";
			return 0;
			break;
		}
	}
	ArrayView view = ArrayView(num_elements, sort_name);
	view.setFrameRate(frame_rate);
	view.setFastMode(fast_mode);

	if (view.Construct(800, 400, 1, 1, false, false))
	{
		// Make thread to do sorting.
		thread sort = thread(sort_view, &view, sort_option);

		view.Start(); // Program doesn't return until GUI closes.
		sort.join();
	}
	return 0;
}
