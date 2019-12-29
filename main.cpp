#include "DominoSolver.h"
#include "DominoInput.h"

int main() {
	DominoInput in;

	in.input_starting_domino();
	in.input_domino_range();
	in.input_dominos();
	
	DominoSolver solve(in.starting_domino, in.domino_range, in.dominos);

	solve.print_solved_train();

	return 0;
}
