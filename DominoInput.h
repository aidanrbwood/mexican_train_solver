#pragma once
#ifndef DOMINO_INPUT_H
#define DOMINO_INPUT_H

#include <vector>
#include "DominoTile.h"

class DominoInput {
public:
	void input_starting_domino();
	void input_domino_range();
	void input_dominos();
	bool input_domino_range_char(char* c, int size);
	bool input_starting_domino_char(char* c, int size);
	bool input_dominos_char(char* c, int size);
	void print_dominos();
	void construct_dominos(vector<short> t);

	short starting_domino;
	short domino_range;
	vector<DominoTile> dominos;
	
};
#endif //DOMINO_INPUT_H
