#include "DominoSolver.h"

int main() {
	DominoSolver solver;
	solver.take_starting_tile_input();
	solver.take_tile_input();
	solver.build_tile_numbers();
	solver.print_solved_train();

	return 0;
}
