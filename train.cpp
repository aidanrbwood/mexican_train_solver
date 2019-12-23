#include "train.h"
#include <stack>
#include <unordered_set>
#include <fstream>
#include <iostream>

void Domino_Solver::take_starting_tile_input() {
	ifstream inputFile;
	inputFile.open("./starting_tile");
	short t;
	inputFile >> t;
	input_starting_tile(t);
	inputFile.close();
}

void Domino_Solver::take_tile_input() {
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

void Domino_Solver::input_starting_tile(short t) {
	starting_tile = t;
}

void Domino_Solver::input_tiles(vector<short> t) {
	int index = 0;
	while (index < t.size()) {
		tiles.push_back(Domino_Tile(t[index], t[index+1]));
		index += 2;
	}
}

void Domino_Solver::print_solved_train() {
	cout << starting_tile << " ";
	vector<vector<Domino_Tile*>> t = solve_train();
	short current_num = starting_tile;
	for (vector<Domino_Tile*> vec : t) {
		for (Domino_Tile* tile : vec) {
			short first_num = 0;
			short second_num = 0;
			if (current_num == tile->num1) {
				first_num = tile->num1;
				second_num = tile->num2;
			} else if (current_num == tile->num2) {
				first_num = tile->num2;
				second_num = tile->num1;
			} else {
				cout << "ERROR: domino tiles arent matching";
			}
			current_num = second_num;
			cout << first_num << " " << second_num << " ";
		}
	}
}

void Domino_Solver::build_tile_numbers() {
	for (Domino_Tile tile : tiles){
		tile_numbers[tile.num1].push_back(&tile);
		tile_numbers[tile.num2].push_back(&tile);
	}
}

vector<vector<Domino_Solver::Domino_Tile*>> Domino_Solver::solve_train(){
	vector<vector<Domino_Tile*>> best_trains;
	vector<Domino_Tile*> current_train;
	
	stack<unordered_set<Domino_Tile*>> tried_tiles;
	tried_tiles.push(unordered_set<Domino_Tile>());

	unordered_set<Domino_Tile*> best_used;
	unordered_set<Domino_Tile*>	current_used;


	short current_number = starting_tile;
	while (true) {
		bool added = false;
		for (Domino_Tile* tile : tile_numbers[current_number]) {
			if (current_used.count(tile) == 0 && tried_tiles.top.count(tile) == 0) {
				current_train.push_back(tile);
				current_used.emplace(tile);
				current_number = tile->get_other_num(current_number);
				tried_tiles.push(unordered_set<Domino_Tile>());
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

			Domino_Tile last_tile* = (current_train.rbegin())*;
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
