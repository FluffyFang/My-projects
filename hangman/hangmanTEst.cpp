#include <iostream>
#include <string>

using namespace std;

int main ()
{
	string headAlive = "   _______ \n  /       \\ \n /  |   |  \\ \n |         | \n \\  \\___/  / \n  \\_______/" ;
	
	string bodyArms = "\n    /| |\\ \n   //| |\\\\ \n  // | | \\\\ \n //  | |  \\\\ \n     | | \n     | | \n     |_|";
	
	string legs = "\n    // \\\\ \n   //   \\\\ \n  //     \\\\";
	
	string bodyNoArms = "\n     | | \n     | | \n     | | \n     | | \n     | | \n     | | \n     |_|";
	
	string deadMan = "   _______       /\n  /       \\     / \n /  X   X  \\   / \n |         |  /\n \\   ___   / / \n  \\_______/ / \n   ==|=|===\n  // | | \\\\ \n  || | | || \n  || | | || \n     | | \n     | | \n     |_| \n    // \\\\ \n   ||   || \n   ||   || \n \n   He  Dead \n \n";
	
	cout << deadMan << endl;

	return 0;


}