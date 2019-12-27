#include "DominoSolver.h"
#include <stack>
#include <unordered_set>
#include <fstream>
#include <iostream>
#include "HashDominoTilePtr.h"

using domino_set = unordered_set<DominoTile*, HashDominoTilePtr>;

void DominoSolver::take_starting_tile_input() {
	ifstream inputFile;
	inputFile.open("./starting_tile");
	short t;
	inputFile >> t;
	input_starting_tile(t);
	inputFile.close();
}

void DominoSolver::take_tile_input() {
	ifstream inputFile;
	inputFile.open("./tiles");
	short t;
	vector<short> tile_list;
	while(inputFile >> t) {
		tile_list.push_back(t);
	}

	input_tiles(tile_list);
	inputFile.close();
}

void DominoSolver::input_starting_tile(short t) {
	starting_tile = t;
}

void DominoSolver::input_tiles(vector<short> t) {
	int index = 0;
	while (index < t.size()) {
		tiles.push_back(DominoTile(t[index], t[index+1]));
		index += 2;
	}
}

void DominoSolver::print_solved_train() {
	cout << starting_tile << " ";
	vector<vector<DominoTile*>> t = solve_train();
	short current_num = starting_tile;
	for (vector<DominoTile*> vec : t) {
		for (DominoTile* tile : vec) {
			short first_num = 0;
			short second_num = 0;
			if (current_num == tile->first) {
				first_num = tile->first;
				second_num = tile->second;
			} else if (current_num == tile->second) {
				first_num = tile->second;
				second_num = tile->first;
			} else {
				cout << "ERROR: domino tiles arent matching";
			}
			current_num = second_num;
			cout << first_num << " " << second_num << " ";
		}
	}
}

void DominoSolver::build_tile_numbers() {
	for (DominoTile tile : tiles){
		tile_numbers[tile.first].push_back(&tile);
		tile_numbers[tile.second].push_back(&tile);
	}
}

vector<vector<DominoTile*>> DominoSolver::solve_train(){
	vector<vector<DominoTile*>> best_trains;
	vector<DominoTile*> current_train;
	
	stack<domino_set> tried_tiles;
	tried_tiles.push(domino_set());

	domino_set best_used;
	domino_set current_used;


	short current_number = starting_tile;
	while (true) {
		bool added = false;
		for (DominoTile* tile : tile_numbers[current_number]) {
			if (current_used.count(tile) == 0 && tried_tiles.top.count(tile) == 0) {
				current_train.push_back(tile);
				current_used.emplace(tile);
				current_number = tile->get_other_num(current_number);
				tried_tiles.push(domino_set());
				added = true;
				break;
			}
		}
		if (!added) {
			// If we reach here then we have gone as far as this train can go
			if (current_train.size() > best_trains[0].size()) {
				best_trains.clear();
				best_trains.push_back(current_train);
			} else if (current_train.size() == best_trains[0].size()) {
				best_trains.push_back(current_train);
			}

			DominoTile last_tile* = (current_train.rbegin())*;
			short last_num = last_tile->get_other_num(current_number);
		
			current_train.pop_back();
			current_best.erase(last_tile);
			current_number = last_num;

			tried_tiles.top.emplace(last_tile);

			if (tried_tile.top.size() == tile_numbers[current_number]) {
				if (current_train.size() == 0) {
					return best_trains;
				}
				last_tile = (current_train.rbegin())*;
				last_num = last_tile->get_other_num(current_number);

				current_train.pop_back();
				current_best.erase(last_tile);
				current_number = last_num;

				tried_tiles.pop();
			}
		}
	}
}
