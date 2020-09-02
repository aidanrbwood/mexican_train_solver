#include <stack>
#include <unordered_set>
#include <iostream>
#include <thread>
#include <functional>
#include <algorithm>

#include "HashDominoTilePtr.h"
#include "DominoSolver.h"
#include "DominoList.h"

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

void find_train(const DominoSolver* const s, const int current_num, const std::unordered_set<DominoTile*> unused_dominos, DominoList** train) 
{
    // If we used all the dominos then finish
    if (unused_dominos.size() == 0)
    {
        return;
    }

    // Find the set of next possible dominos based on the current tile number
    // Intersect the set of next possible dominos with the set of unused dominos
    const vector<DominoTile*> possible_next(s->tile_numbers.at(current_num));
    const vector<DominoTile*> available_next(intersect(possible_next, unused_dominos));
    
    // If we haven't used all dominos, but can't add any more to the train then finish
    if (available_next.size() == 0)
    {
        return;
    }

    // Create vectors to keep track of the spawned threads and their returns
    vector<DominoList**> trains(available_next.size(), nullptr);
    vector<std::thread*> threads(available_next.size(), nullptr);

    for (int x = 0; x < available_next.size(); x++)
    {
        DominoTile* d = available_next.at(x);
        std::unordered_set<DominoTile*> new_set(unused_dominos);
        new_set.erase(d);
        trains[x] = new DominoList*(nullptr);
        threads[x] = new std::thread(find_train, s, d->get_other_num(current_num), new_set, trains[x]);
    }

    for (std::thread* t : threads)
    {
        t->join();
    }

    // Find the longest train spawned by all children
    int index = -1;
    int length = -1;
    int points = -1;
    for (int x = 0; x < available_next.size(); x++)
    {
        int train_length = 0;
        int train_points = available_next.at(x)->first + available_next.at(x)->second;
        if (*trains[x] != nullptr) 
        {
            train_length = (*trains[x])->size();
            train_points += (*trains[x])->m_points;
        }

        if (train_length > length || (train_length == length && train_points > points))
        {
            if (length > 0)
            {
                delete *trains[index];
            }
            length = train_length;
            points = train_points;
            index = x;
        }
        else if (train_length > 0)
        {
            delete *trains[x];
        }
    }
 
    // Append the correct tile to the longest train and then return it
    if (length <= 0)
    {
        *train = new DominoList{available_next[index]};
    }
    else
    {
        *train = *trains.at(index);
        (*train)->push_back(available_next.at(index));
    }
    
    for (int x = 0; x < available_next.size(); x++)
    {
        delete threads[x];
        delete trains[x];
    }
}

vector<vector<DominoTile*>> DominoSolver::solve_train(){
    vector<vector<DominoTile*>> best_trains;
    DominoList** train = new DominoList*(nullptr);

    std::unordered_set<DominoTile*> full_set;
    
    for (vector<DominoTile*> v : tile_numbers)
    {
        for (DominoTile* d : v)
        {
            full_set.insert(d);
        }
    }

    find_train(this, starting_tile, full_set, train);

    if (*train != nullptr)
    {
        std::reverse((*train)->begin(), (*train)->end());
        best_trains.push_back(std::move(**train));
        delete *train;
    }
    
    delete train;

    return best_trains;
}
