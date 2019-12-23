#include "train.h"

int main() {
	Domino_Solver solver;
	solver.take_starting_tile_input();
	solver.take_tile_input();
	solver.print_solved_train();

	return 0;
}
