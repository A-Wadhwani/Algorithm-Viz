#include "array_main.hh"
#include <unistd.h>

using namespace AlgorithmViz;

int main()
{
	ArrayView view = ArrayView(20);
	if (view.Construct(720, 720, 4, 4, false, false))
		view.Start(); // Program doesn't return until GUI closes.
		
	return 0;
}