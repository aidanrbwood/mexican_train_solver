#pragma once
#ifndef DOMINO_SOLVER_H
#define DOMINO_SOLVER_H

#include <vector>
#include <string>
#include "DominoTile.h"


using namespace std;

class DominoSolver {
public:
	DominoSolver(short starting_domino, short domino_range, vector<DominoTile> dominos);
	vector<vector<DominoTile*>> solve_train();
	void print_solved_train();
	std::string print_solved_train_string();
private:	
	short tile_range;
	short starting_tile;
	vector<DominoTile> tiles;
	vector<vector<DominoTile*>> tile_numbers;
	void build_tile_numbers();
};

#endif // DOMINO_SOLVER_H
