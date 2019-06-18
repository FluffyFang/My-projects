#include <string>
#include <stdlib.h>
#include <fstream>
#include <time.h>
#include <vector>

using namespace std;

class Dictionary {
private:
	std::vector<std::string> listOfWords;
	std::vector<std::string> listOfUsedWords;
public:
	Dictionary () {
	}
	
	void loadDictionary (std::string filename) {
		std::string parse;
		std::ifstream in (filename.c_str());


		while (!in.eof()) 
		{
			in >> parse;
			//cout << "read in " << parse << endl;
			listOfWords.push_back(parse);
		}
	}
	
	std::string randomWord () {
		std::string temp;
		srand(time(NULL));
		bool lookingForWord = true;
		while (lookingForWord) {
			lookingForWord = false;
			int randomInt = rand() % (listOfWords.size());
			temp = listOfWords[randomInt];
			for (int i = 0; i < listOfUsedWords.size(); i++) {
				if (temp == listOfUsedWords[i]) {
					lookingForWord = true;
				}
			}
		}
		listOfUsedWords.push_back(temp);
		return temp;
	}
	
	void reset() {
		listOfUsedWords.clear();
	}
};