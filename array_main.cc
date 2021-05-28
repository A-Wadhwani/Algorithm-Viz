#include "array_main.hh"

using namespace AlgorithmViz;

int main()
{
	ArrayView view = ArrayView(100);
	if (view.Construct(720, 480, 4, 4, false, false))
		view.Start();

	return 0;
}