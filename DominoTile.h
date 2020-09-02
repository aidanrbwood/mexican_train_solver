#ifndef DOMINO_TILE_H
#define DOMINO_TILE_H

#include <utility>

using namespace std;

class DominoTile : public pair<short,short> {
public:
	DominoTile(short s1, short s2) : pair(s1,s2) {}
	
	bool operator==(const DominoTile& d) {return (first == d.first && second == d.second) || (first == d.second && second == d.first);}
	short get_other_num(short s) {return s == first ? second : first;}
};

#endif //DOMINO_TILE_H
