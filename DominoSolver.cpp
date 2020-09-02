#include "DominoSolver.h"
#include <stack>
#include <unordered_set>
#include <iostream>
#include "HashDominoTilePtr.h"
#include "DominoNode.h"
#include <thread>
#include <functional>

using domino_set = unordered_set<DominoTile*, HashDominoTilePtr>;

DominoSolver::DominoSolver(short starting_domino, short domino_range, vector<DominoTile> dominos) : 
tile_range(domino_range), 
starting_tile(starting_domino), 
tiles(dominos) 
{
	build_tile_numbers();
}

std::string DominoSolver::print_solved_train_string() {
	const char* vert_pipe = "|";
	const char* horz_pipe = "-";
	const char* thin_pipe = "|";
	const char* up_left = "*";
	const char* up_right = "*";
	const char* down_left = "*";
	const char* down_right = "*";
	const char* down_split = "*";
	const char* up_split = "*";
	string res;	
	vector<vector<DominoTile*>> t = solve_train();
	
	for (vector<DominoTile*> vec : t) {
		res+=horz_pipe;
		res+=horz_pipe;
		res+=up_right;
		res+=" ";
		for (int x = 0; x < t[0].size(); x++){
			res+=up_left;
			res+=horz_pipe;
			res+=horz_pipe;
			res+=up_split;
			res+=horz_pipe;
			res+=horz_pipe;
			res+=up_right;
			res+=" ";
		}
		res+="\n";
		if (starting_tile < 10)
			res+=" ";
		res+=to_string(starting_tile);
		res+=vert_pipe;
		res+=" ";
		short current_num = starting_tile;

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
				//cout << "ERROR: domino tiles arent matching";
			}
			current_num = second_num;
			res+=vert_pipe;
			if (first_num < 10)
				res+=" ";
			res+=to_string(first_num);
			res+=thin_pipe; 
			if (second_num < 10)
				res+=" ";
			res+=to_string(second_num);
			res+=vert_pipe;
			res+=" ";
		}

		res+="\n";
		res+=horz_pipe;
		res+=horz_pipe;
		res+=down_right;
		res+=" ";
		for (int x = 0; x < t[0].size(); x++){
			res+=down_left;
			res+=horz_pipe;
			res+=horz_pipe;
			res+=down_split;
			res+=horz_pipe;
			res+=horz_pipe;
			res+=down_right;
			res+=" ";
		}
		res+="\n";
	}
	res+="length: ";
	res+=to_string(t[0].size());
	return res;
}
void DominoSolver::print_solved_train() {
	const char* vert_pipe = u8"\u2503";
	const char* horz_pipe = u8"\u2501";
	const char* thin_pipe = u8"\u2502";
	const char* up_left = u8"\u250f";
	const char* up_right = u8"\u2513";
	const char* down_left = u8"\u2517";
	const char* down_right = u8"\u251b";
	const char* down_split = u8"\u2537";
	const char* up_split = u8"\u252f";
	cout << "SOLUTION(S):" << endl;
		
	vector<vector<DominoTile*>> t = solve_train();
	
	for (vector<DominoTile*> vec : t) {
		cout << horz_pipe << horz_pipe;
		cout << up_right;
		for (int x = 0; x < t[0].size(); x++){
			cout << up_left << horz_pipe << horz_pipe << up_split << horz_pipe << horz_pipe << up_right;
		}
		cout << endl;
		if (starting_tile < 10)
			cout << " ";
		cout << starting_tile << vert_pipe;
		short current_num = starting_tile;

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
				//cout << "ERROR: domino tiles arent matching";
			}
			current_num = second_num;
			cout << vert_pipe;
			if (first_num < 10)
				cout << " ";
			cout << first_num << thin_pipe; 
			if (second_num < 10)
				cout << " ";
			cout << second_num << vert_pipe;
		}

		cout << endl << horz_pipe << horz_pipe;
		cout << down_right;
		for (int x = 0; x < t[0].size(); x++){
			cout << down_left << horz_pipe << horz_pipe << down_split << horz_pipe << horz_pipe << down_right;
		}
		cout << endl;
	}
	cout << "length: " << t[0].size();
	cout << endl << "=======================" << endl;
}

void DominoSolver::build_tile_numbers() {
	for (int x = 0; x < tile_range; x++) {
		tile_numbers.push_back(vector<DominoTile*>());	
	}
	DominoTile* tile = nullptr;
	for (int x = 0; x < tiles.size(); x++){
		tile = &tiles[x];
		tile_numbers[tile->first].push_back(tile);
        // Don't double insert double tiles
		if (tile->first != tile->second) {
			tile_numbers[tile->second].push_back(tile);
		}
	}
}

vector<DominoTile*> intersect(const vector<DominoTile*> &possible_next, const std::unordered_set<DominoTile*> &unused_dominos)
{
    vector<DominoTile*> result;
    for (DominoTile* d : possible_next)
    {
        if (unused_dominos.count(d) != 0)
        {
            result.push_back(d);
        }
    }
    return result;
}

void find_train(DominoSolver* s, const int current_num, const std::unordered_set<DominoTile*> unused_dominos, DominoNode* &train) 
{
    if (unused_dominos.size() == 0)
    {
        return;
    }
    const vector<DominoTile*> possible_next(s->tile_numbers.at(current_num));
    const vector<DominoTile*> available_next(intersect(possible_next, unused_dominos));
    
    if (available_next.size() == 0)
    {
        std::cout << "current_num: " << current_num << " no available nodes" << std::endl;
        return;
    }

    vector<DominoNode*> trains(available_next.size(), nullptr);
    vector<std::thread*> threads(available_next.size(), nullptr);

    int x = 0;
    for (DominoTile* d : available_next)
    {
        int new_num = d->get_other_num(current_num);
        std::cout << "current_num: " << current_num << " spawning to: " << new_num << std::endl;
        std::unordered_set<DominoTile*> new_set(unused_dominos);
        new_set.erase(d);
        threads[x] = new std::thread(find_train, s, new_num, new_set, std::ref(trains[x]));
        x++;
    }

    for (std::thread* t : threads)
    {
        t->join();
    }

    x = 0;
    DominoNode* longest_train = nullptr;
    for (DominoTile* d : available_next)
    {
        if (trains[x] == nullptr && longest_train == nullptr)
        {
            longest_train = new DominoNode(d, 1);
        }
        else if ((trains[x] != nullptr) && (longest_train == nullptr || longest_train->length < trains[x]->length))
        {
            // delete current longest
            while (longest_train != nullptr)
            {
                DominoNode* t = longest_train->next;
                delete longest_train;
                longest_train = t;
            }

            longest_train = new DominoNode(d, trains[x]->length + 1);
            longest_train->next = trains[x];
        } 
        else 
        {
            while (trains[x] != nullptr)
            {
                DominoNode* t = trains[x]->next;
                delete trains[x];
                trains[x] = t;
            }
        }
        if (threads[x] != nullptr)
            delete threads[x];
        x++;
    }
    train = longest_train;
    std::cout << "returning from " << current_num << ": " << longest_train->length << std::endl;
}

vector<vector<DominoTile*>> DominoSolver::solve_train(){
    vector<vector<DominoTile*>> best_trains;
    DominoNode* train = nullptr;

    std::unordered_set<DominoTile*> full_set;
    
    for (vector<DominoTile*> v : tile_numbers)
    {
        for (DominoTile* d : v)
        {
            full_set.insert(d);
        }
    }

    find_train(this, starting_tile, full_set, train);

    vector<DominoTile*> best_train;

    while (train != nullptr)
    {
        best_train.push_back(train->tile);
        train = train->next;
    }

    best_trains.push_back(best_train);
    return best_trains;
    /*
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
			if (current_used.count(tile) == 0) {
				if (tried_tiles.top().count(tile) == 0) {
					current_train.push_back(tile);
					current_used.emplace(tile);
					//cout << "first: " << tile->first << " second: " << tile->second << " done" << endl;
					current_number = tile->get_other_num(current_number);
					tried_tiles.push(domino_set());
					added = true;
					break;
				}
			} else {
				tried_tiles.top().emplace(tile);
			}
		}
		if (!added) {
			if (current_train.size()) {
				// If we didn't add then we have reached the max length of this train
				if (current_train.size() > best_trains[0].size()) {
					best_trains.clear();
					best_trains.push_back(current_train);
				} else if (current_train.size() == best_trains[0].size()) {
					best_trains.push_back(current_train);
				}

				// Save last tile and then pop if off of train, also remove its level of the tried stack
				DominoTile* last_tile = *(current_train.rbegin());
				short last_num = last_tile->get_other_num(current_number);
		
				current_train.pop_back();
				current_used.erase(last_tile);
				current_number = last_num;
			
				tried_tiles.pop();
				tried_tiles.top().emplace(last_tile);
			}
			if (current_train.size() == 0 && tried_tiles.top().size() == tile_numbers[starting_tile].size()) {
				return best_trains;
			}
		}
	}
    */

}
