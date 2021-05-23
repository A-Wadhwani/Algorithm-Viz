#include "main.hh"

using namespace AlgorithmViz;

int main()
{
	ArrayView view = ArrayView();
	if (view.Construct(256, 240, 4, 4, false, false))
		view.Start();

	return 0;
}