#include <vector>

using namespace std;

class Domino_Solver {
public:
	class Domino_Tile {
	public:
		Domino_Tile(short s1, short s2) : num1(s1), num2(s2) {}
		short num1;
		short num2;
		short get_other_num(short s) {
			if (num1 == s)
				return num2;
			return num1;
		}
	};
	void take_starting_tile_input();
	void take_tile_input();

	void input_starting_tile (short t);
	void input_tiles(vector<short> t);
	vector<vector<Domino_Tile*>> solve_train();
	void print_solved_train();
private:	
	short starting_tile;
	vector<Domino_Tile> tiles;
	vector<vector<Domino_Tile*>> tile_numbers;

	void build_tile_numbers();
};
