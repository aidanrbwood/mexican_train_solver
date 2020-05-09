#include <fstream>
#include <cctype>
#include <iostream>
#include "DominoInput.h"


void DominoInput::input_starting_domino() {
	ifstream inputFile;
	inputFile.open("./starting_domino");
	if (inputFile.fail()) {
		throw "Error: failure to open file starting_domino";
	}
	inputFile >> starting_domino;
	if (inputFile.fail()) {
		throw "Error: starting_domino file contains non-integer";
	}
	inputFile.close();
}

void DominoInput::input_domino_range() {
	ifstream inputFile;
	inputFile.open("./domino_range");
	if (inputFile.fail()) {
		throw "Error: failure to open file domino_range";
	}
	inputFile >> domino_range;
	if (inputFile.fail()) {
		throw "Error: domino_range file contains non-integer";
	}
	inputFile.close();
}

void DominoInput::input_dominos() {
	ifstream inputFile;
	inputFile.open("./dominos");
	if (inputFile.fail()) {
		throw "Error: failure to open file dominos";
	}
	short t;
	vector<short> tile_list;
	while(inputFile >> t) {
		if (inputFile.fail()) {
			throw "Error: dominos file contains invalid non-integer";
		}
		tile_list.push_back(t);
	}
	inputFile.close();
	construct_dominos(tile_list);
}


void DominoInput::construct_dominos(vector<short> t) {
	int index = 0;
	if (t.size() % 2) {
		throw "Error: odd number of integers in dominos file";
	}
	while (index < t.size()) {
		dominos.push_back(DominoTile(t[index], t[index+1]));
		index += 2;
	}
}

bool DominoInput::input_dominos_char(char* arr, int size)
{
	string str(arr);
	std::cout << "str length: " << str.length() << std::endl;
	dominos.clear();
	short num1 = -1;
	short num2 = -1;
	int index = 0;
	enum State{FIRSTNUM, SECONDNUM, NEWLINE};
	State s = FIRSTNUM;

	while (index < str.length())
	{
		char c = str[index];
		// Remove Spaces
		if (c == ' ')
		{
			index++;
		}
		else
		{
			switch (s)
			{
			case FIRSTNUM:
				if (isdigit(c) && index+1 < str.length() && (str[index+1] == ' ' || isdigit(str[index+1])))
				{
					short d1 = c - '0';
					index++;
					c = str[index];
					if (c == ' ')
					{
						num1 = d1;
					}
					else if (isdigit(c))
					{
						short d2 = c - '0';
						num1 = d1*10 + d2;
						index++;
					}
					else
					{
						std::cout << "inner firstNum failed" << std::endl;
						dominos.clear();
						return false;
					}
					
				}
				else
				{
					std::cout << "outer firstNum failed" << std::endl;
					dominos.clear();
					return false;
				}
				s = SECONDNUM;
				break;
			case SECONDNUM:
				std::cout << "index: " << index << std::endl;
				if (isdigit(c) && index+1 < str.length() && (str[index+1] == '\n' || isdigit(str[index+1])))
				{
					short d1 = c - '0';
					index++;
					c = str[index];
					if (c == '\n')
					{
						num2 = d1;
					}
					else if (isdigit(c))
					{
						short d2 = c - '0';
						num2 = d1*10 + d2;
						index++;
					}
					else
					{
						std::cout << "inner secondNum failed" << std::endl;
						dominos.clear();
						return false;
					}
				}
				else if (index+1 == str.length())
				{
					num2 = c - '0';
					index++;
				}
				else
				{
					std::cout << "outer secondNum failed" << std::endl;
					dominos.clear();
					return false;
				}
				dominos.push_back(DominoTile(num1, num2));
				num1 = -1;
				num2 = -1;
				s = NEWLINE;
				break;
			case NEWLINE:
				if (str[index] != '\n')
				{
					std::cout << "newlined failed" << std::endl;
					dominos.clear();
					return false;
				}
				index++;
				s = FIRSTNUM;
				break;
			default:
				break;
			}
		}
	}
	return true;	
}

bool DominoInput::input_domino_range_char(char* c, int size)
{
	string str(c);
	try 
	{
		domino_range = stoi(str);
	}
	catch (std::invalid_argument const &e)
	{
		return false;
	}
	return true;
}

bool DominoInput::input_starting_domino_char(char* c, int size)
{
	string str(c);
	try
	{
		starting_domino = stoi(str);
	}
	catch(std::invalid_argument const &e)
	{
		return false;
	}
	return true;
	
}

void DominoInput::print_dominos()
{
	for (int x = 0; x < dominos.size(); x++)
	{
		std::cout << dominos[x].first << " " << dominos[x].second << std::endl;
	}
}





