#include <iostream>
#include <string>

class Hangman {
private:
	std::string headDead;
	std::string headAlive;
	std::string bodyArms;
	std::string legs;
	std::string bodyNoArms;
	std::string bodyOneArm;
	std::string leg;
	std::string deadMan;
public:
	Hangman () {
		headDead = "   _______ \n  /       \\ \n /  X   X  \\ \n |         | \n \\   ___   / \n  \\_______/" ;
		headAlive = "   _______ \n  /       \\ \n /  |   |  \\ \n |         | \n \\  \\___/  / \n  \\_______/" ;
		bodyArms = "\n    /| |\\ \n   //| |\\\\ \n  // | | \\\\ \n //  | |  \\\\ \n     | | \n     | | \n     |_|";
		legs = "\n    // \\\\ \n   //   \\\\ \n  //     \\\\";
		bodyNoArms = "\n     | | \n     | | \n     | | \n     | | \n     | | \n     | | \n     |_|";
		bodyOneArm = "\n    /| | \n   //| | \n  // | | \n //  | | \n     | | \n     | | \n     |_|";
		leg = "\n    // \n   // \n  // ";
		deadMan = "   _______       /\n  /       \\     / \n /  X   X  \\   / \n |         |  /\n \\   ___   / / \n  \\_______/ / \n   ==|=|===\n  // | | \\\\ \n  || | | || \n  || | | || \n     | | \n     | | \n     |_| \n    // \\\\ \n   ||   || \n   ||   || \n \n   Fred  Dead \n \n";
	}
	
	void printBody(int lives, int livesRemaining) {
		if (livesRemaining < lives) {
			if (livesRemaining != 0) {
				std::cout << headAlive;
				if (lives == 7) {
					if (livesRemaining == 5) {
						std::cout << bodyNoArms;
					}
					else if (livesRemaining == 4) {
						std::cout << bodyOneArm;
					}
					else if (livesRemaining <= 3) {
						std::cout << bodyArms;
						if (livesRemaining == 2) {
							std::cout << leg;
						}
						else if (livesRemaining <= 1) {
							std::cout << legs;
						}
					}
				}
				else if (lives == 5) {
					if (livesRemaining == 3) {
						std::cout << bodyNoArms;
					}
					else if (livesRemaining <= 2) {
						std::cout << bodyArms;
						if (livesRemaining <= 1) {
							std::cout << legs;
						}
					}
				}
				else {
					if (livesRemaining <= 2) {
						std::cout << bodyArms;
						if (livesRemaining <= 1) {
							std::cout << legs;
						}
					}
				}
			}
			else {
				std::cout << deadMan;
			}
			
			
		}
	}
};