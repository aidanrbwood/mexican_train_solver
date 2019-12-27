#pragma once
#ifndef DOMINO_SOLVER_H
#define DOMINO_SOLVER_H

#include <vector>
#include "DominoTile.h"

using namespace std;

class DominoSolver {
public:
	void take_starting_tile_input();
	void take_tile_input();

	void input_starting_tile (short t);
	void input_tiles(vector<short> t);
	vector<vector<DominoTile*>> solve_train();
	void print_solved_train();
private:	
	short starting_tile;
	vector<DominoTile> tiles;
	vector<vector<DominoTile*>> tile_numbers;

	void build_tile_numbers();
};

#endif // DOMINO_SOLVER_H
