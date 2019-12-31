#include "DominoSolver.h"
#include "DominoInput.h"
#include "iostream"

int main() {
	DominoInput in;
	
	try {
		in.input_starting_domino();
		in.input_domino_range();
		in.input_dominos();
		DominoSolver solve(in.starting_domino, in.domino_range, in.dominos);

		solve.print_solved_train();
	} catch (const char* e) {
		cerr << e << endl;
	}
	
	return 0;
}
