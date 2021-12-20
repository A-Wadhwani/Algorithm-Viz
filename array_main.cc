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
	case MERGE_SORT:
		merge_sort(view, 0, view->getSize() - 1);
		break;
	case QUICK_SORT:
		quick_sort(view, 0, view->getSize() - 1);
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

void merge(ArrayView *view, int l, int m, int r)
{
	int n1 = m - l + 1;
	int n2 = r - m;
	int L[n1], R[n2];

	for (int i = 0; i < n1; i++)
		L[i] = view->getElement(l + i);
	for (int j = 0; j < n2; j++)
		R[j] = view->getElement(m + 1 + j);

	int i = 0;
	int j = 0;
	int k = l;

	while (i < n1 && j < n2)
	{
		if (L[i] <= R[j])
		{
			view->putElement(k, L[i]);
			i++;
		}
		else
		{
			view->putElement(k, R[j]);
			j++;
		}
		k++;
	}

	while (i < n1)
	{
		view->putElement(k, L[i]);
		i++;
		k++;
	}

	while (j < n2)
	{
		view->putElement(k, R[j]);
		j++;
		k++;
	}
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

	if (argc >= 4)
	{
		view.setFrameRate(atoi(argv[3]));
	}

	if (argc == 5)
	{
		view.setFastMode(argv[4][0] == 'y');
	}

	if (view.Construct(800, 400, 4, 4, false, false))
	{
		// Make thread to do sorting.
		std::thread sort = std::thread(sort_view, &view, sort_option);

		view.Start(); // Program doesn't return until GUI closes.
		sort.join();
	}
	return 0;
}
