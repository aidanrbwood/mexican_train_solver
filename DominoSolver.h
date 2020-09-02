#pragma once
#ifndef DOMINO_SOLVER_H
#define DOMINO_SOLVER_H

#include <vector>
#include <string>
#include <unordered_set>
#include "DominoTile.h"


using namespace std;

class DominoSolver {
public:
	DominoSolver(short starting_domino, short domino_range, vector<DominoTile> dominos);
	vector<vector<DominoTile*>> solve_train();
	void print_solved_train();
	std::string print_solved_train_string(); 
	vector<vector<DominoTile*>> tile_numbers;

private:	
	short tile_range;
	short starting_tile;
	vector<DominoTile> tiles;
	void build_tile_numbers();
    //void find_train(const int current_num, const std::unordered_set<DominoTile*> unused_dominos, DominoNode* &train);
    //vector<DominoTile*> intersect(const vector<DominoTile*> &possible_next, const std::unordered_set<DominoTile*> &unused_dominos);
};

#endif // DOMINO_SOLVER_H
