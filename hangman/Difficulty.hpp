#include "Dictionary.hpp"
#include "Hangman.hpp"
#include <string>

class Difficulty : public Dictionary
{	
public:
	int lives;
	
	Difficulty() : Dictionary() {
		std::string temp = "";
		while (temp == ""){
			temp = chooseDifficulty();
		}
		loadDictionary(temp);
	}

	std::string chooseDifficulty () {
		std::string success;
		std::string input;
		std::cout << "Please select a difficulty:" << std::endl << "(e)asy - 7 misses" << std::endl << "(m)edium - 5 misses" << std::endl << "(h)ard - 4 misses" << std::endl;
		std::cin >> input;
		if (input == "e" || input == "easy" || input == "E" || input == "Easy") {
			lives = 7;
			success = "easy.txt";
		}
		else if (input == "m" || input == "medium" || input == "M" || input == "Medium") {
			lives = 5;
			success = "medium.txt";
		}
		else if (input == "h" || input == "hard" || input == "H" || input == "Hard") {
			lives = 4;
			success = "hard.txt";
		}
		else {
			std::cout << "Input not recognized." << std::endl;
			success = "";
		}
		return success;
	}
};