#include <iostream>
#include <string>
#include <cstring>
#include "Difficulty.hpp"

using namespace std;

class Game {
private:
	int livesRemaining;
	int correctLetters;
	std::string currentWord;
	std::string incorrectGuesses;
	std::string wordDisplay;
	Difficulty *diff;
	Hangman *sprite;

	void displayGame() {
		std::cout << "Lives Remaining: " << livesRemaining << std::endl;
		std::cout << "Incorrect guesses: " << incorrectGuesses << std::endl;
		sprite -> printBody(diff -> lives, livesRemaining);
		std::cout << std::endl << wordDisplay << std::endl;
	}
	bool resolution () {
		std::string input;
		bool playAgain;
		displayGame();
		if (livesRemaining > 0) {
			std::cout << "You Win! ";
		}
		else {
			std::cout << "You Lose! ";
		}
		std::cout << "Play Again? y/n : ";
		std::cin >> input;
		if (input == "y" || input == "yes" || input == "Y" || input == "Yes") {
			newRound();
			playAgain = true;
		}
		else if (input == "n" || input == "no" || input == "N" || input == "No") {
			playAgain = false;
		}
		else {
			playAgain = false;
		}
		return playAgain;
	}
public:
	Game() {
		std::cout << "Welcome to Hangman!" << std::endl;
		diff = new Difficulty();
		sprite = new Hangman();
	}

	/*void hardReset() {
		correctLetters = 0;
		for (int i = 0; i < 7; i++) {
			incorrectGuesses[i] = ' ';
		}
		wordList -> reset();
		while (!chooseDifficulty()) {}
		newRound();
	}*/
	
	void newRound() {
		correctLetters = 0;
		incorrectGuesses = "";
		wordDisplay = "";
		
		for (int i = 0; i < 7; i++) {
			incorrectGuesses[i] = ' ';
		}
		
		livesRemaining = diff -> lives;
		currentWord = diff -> randomWord();

		for (int i = 0; i < currentWord.length(); i++) {
			switch(currentWord[i])
			{
				case '\'':
				case ' ':
				case '.':
				case '!':
				case '\n':
				default:
					wordDisplay += '*';
			}
		}
	}
	
	bool loop() {
		bool playing = true;
		while (playing) {
			std::string input;
			bool letterFound = false;
			std::cout << std::endl << std::endl;
			displayGame();
			std::cout << "Guess a letter: ";
			std::cin >> input;
			for (int i = 0; i < currentWord.length(); i++) {
				if (input[0] == currentWord[i]) {
					wordDisplay[i] = input[0];
					correctLetters++;
					letterFound = true;
				}
			}
			
			if (!letterFound) {
				incorrectGuesses += input[0];
				livesRemaining--;
				if (livesRemaining <= 0) {
					playing = false;
					//std::cout << deadMan << endl;
				}
			}
			else if (correctLetters == currentWord.length()) {
				playing = false;
			}
		}
		return resolution();
	}
};