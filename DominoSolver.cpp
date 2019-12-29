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
		//cout<< t[index] << " " << t[index+1] << endl;
		tiles.push_back(DominoTile(t[index], t[index+1]));
		index += 2;
	}
}

void DominoSolver::print_solved_train() {
	cout << "SOLUTION(S):" << endl;
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
	cout << endl << "=======================" << endl;
}

void DominoSolver::build_tile_numbers() {
	for (int x = 0; x < 20; x++) {
		tile_numbers.push_back(vector<DominoTile*>());	
	}
	DominoTile* tile = nullptr;
	for (int x = 0; x < tiles.size(); x++){
		tile = &tiles[x];
//		cout << "gfirst: " << tile->first << " second: " << tile->second << " done" << endl;
		tile_numbers[tile->first].push_back(tile);
		tile_numbers[tile->second].push_back(tile);
	}
	for (int x = 0; x < tile_numbers.size(); x++) {
		cout << x << ": ";
		for (int y = 0; y < tile_numbers[x].size(); y++) {
			DominoTile* tmp = tile_numbers[x][y];
			cout << tmp->first << " " << tmp->second << "   ";
		}
		cout << "\n";
	}
}

vector<vector<DominoTile*>> DominoSolver::solve_train(){
	vector<vector<DominoTile*>> best_trains;
	vector<DominoTile*> current_train;
	
	stack<domino_set> tried_tiles;

	domino_set best_used;
	domino_set current_used;

	tried_tiles.push(domino_set());
	best_trains.push_back(vector<DominoTile*>());

	short current_number = starting_tile;
	while (true) {
		bool added = false;
		DominoTile* tile = nullptr;
		for (int index = 0; index < tile_numbers[current_number].size(); index++) {
			tile = tile_numbers[current_number][index];
			if (current_used.count(tile) == 0 && tried_tiles.top().count(tile) == 0) {
				current_train.push_back(tile);
				current_used.emplace(tile);
				//cout << "first: " << tile->first << " second: " << tile->second << " done" << endl;
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

			DominoTile* last_tile = *(current_train.rbegin());
			short last_num = last_tile->get_other_num(current_number);
		
			current_train.pop_back();
			current_used.erase(last_tile);
			current_number = last_num;

			tried_tiles.top().emplace(last_tile);

			if (tried_tiles.top().size() == tile_numbers[current_number].size()) {
				if (current_train.size() == 0) {
					return best_trains;
				}
				last_tile = *(current_train.rbegin());
				last_num = last_tile->get_other_num(current_number);

				current_train.pop_back();
				current_used.erase(last_tile);
				current_number = last_num;

				tried_tiles.pop();
			}
		}
	}
}
