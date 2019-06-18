#include "Game.hpp"

int main(int argc, char *argv[]) {
	Game hang;
	hang.newRound();
	while (hang.loop()){}
	return 0;
}