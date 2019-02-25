#define SDL_MAIN_HANDLED

#include <SDL.h>
#include <SDL_mixer.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <time.h> 
#include "staticSprite.h"
#include "movingSprite.h"

struct params {
	std::string fileLocation;
	int blocking;
	int starting;
	int ending;
};

struct coordinate {
	int x;
	int y;
};

bool test = false;

//constants
const int SCREEN_WIDTH = 700;
const int SCREEN_HEIGHT = 500;

const int maxTilesX = 50;
const int maxTilesY = 50;
const int maxObjects = 29;
const int playerSpeed = 5;

const int OPEN_SPACES_POTS = 16;
const int CHOSEN_SPACES_POTS = 8;

const std::string brim = "Player.txt";
const std::string arrow = "arrow_0.txt";
const std::string temp = "temp.txt";

void loadLevel(int dungeon, int floor, int &levelWidth, int &levelHeight, StaticSprite *&background, int floorCode[maxTilesX][maxTilesY], std::vector<StaticSprite *> &sprites, std::vector<MovingSprite *> &aniSprites, SDL_Renderer *renderer, coordinate levelChange[3]);
void calc_random_positions(int open[], int chosen[]);
int fadeTheMusic(void* data);
void changeLevel(int dungeon, int floor, int levelWidth, int levelHeight, int x, int y, char replaceWith);
void saveCurrentLevel(int dungeon, int floor, int levelWidth, int levelHeight, int floorCode[maxTilesX][maxTilesY]);

std::string song = "";
bool baton = true;
Mix_Music *gMusic = NULL;

enum keyCode {NONE, UP, RIGHT, DOWN, LEFT};

int main(int argc, char *argv[])
{
	std::ifstream readFile ("Save.txt");
	
	//initializing stuff
	int floorCode[maxTilesX][maxTilesY];
	std::vector<StaticSprite *> sprites;
	std::vector<MovingSprite *> aniSprites;
	SDL_Window *win = NULL;
	SDL_Renderer *renderer = NULL;
	int levelWidth, levelHeight, floor, dungeon;
	int posX = 100; //Window position
	int posY = 100; //Window position
	readFile >> dungeon >> floor; //Read in character position
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	win = (SDL_CreateWindow("Hello world", posX, posY, SCREEN_WIDTH, SCREEN_HEIGHT, 0));
	renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	SDL_Thread* audio;
	int last = SDL_GetTicks();
	int keyPressed = 0;
	SDL_Rect camera = {0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
	srand (time(NULL));
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	StaticSprite *background;
	coordinate levelChange[3];
	
	//Load player
	MovingSprite *player = new MovingSprite(brim, renderer);
	readFile >> posX >> posY;
	player -> setXY(posX, posY);
	player -> setSpeed(playerSpeed);
	player -> currentFrame = 3;
	readFile.close();
	
std::cout << "Loading level" << std::endl;
	loadLevel(dungeon,floor,levelWidth,levelHeight,background,floorCode,sprites,aniSprites,renderer,levelChange);
std::cout << "Finished loading level" << std::endl;
std::cout << "Loading audio" << std::endl;
	audio = SDL_CreateThread (fadeTheMusic, "Music Stuff", (void *)NULL);
std::cout << "Finished loading audio" << std::endl;
	test = true;
	while (1)
	{
		SDL_Event e;
		if (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT) break;
		}
		else if (e.type == SDL_KEYDOWN) //Records button presses
		{
			switch( e.key.keysym.sym )
			{
			case SDLK_UP:
				keyPressed = UP;
				break;
				
			case SDLK_RIGHT:
				keyPressed = RIGHT;
				break;

			case SDLK_DOWN:
				keyPressed = DOWN;
				break;

			case SDLK_LEFT:
				keyPressed = LEFT;
				break;

			default:
				break;
			}
		}
		else if (e.type == SDL_KEYUP) //Records button releases
		{
			switch( e.key.keysym.sym )
			{
			case SDLK_UP:
				if (keyPressed == UP)
				{
					keyPressed = NONE;
				}
				break;
				
			case SDLK_RIGHT:
				if (keyPressed == RIGHT)
				{
					keyPressed = NONE;
				}
				break;

			case SDLK_DOWN:
				if (keyPressed == DOWN)
				{
					keyPressed = NONE;
				}
				break;

			case SDLK_LEFT:
				if (keyPressed == LEFT)
				{
					keyPressed = NONE;
				}
				break;

			default:
				break;
			}
		}
		if (player -> actionTimer == 0 && keyPressed) //Does things based on button registered
		{
			bool found = false;
			switch(keyPressed)
			{
			case UP:
				player -> currentFrame = 0;
				if (floorCode[player -> snapX][player -> snapY-1] == 1) // Space is open
				{
					player -> move(UP);
				}
				else if (floorCode[player -> snapX][player -> snapY-1] == 2) // Space has moveable object
				{
					if (floorCode[player -> snapX][player -> snapY-2] == 1) // Space past that is empty
					{
						for (int i = 0; i < aniSprites.size() && !found; i++)
						{
							if (aniSprites[i] -> x == player -> snapX && aniSprites[i] -> y == player -> snapY - 1)
							{
								aniSprites[i] -> move(UP);
								found = true;
							}
						}
						floorCode[player -> snapX][player -> snapY-1] = 1;
						floorCode[player -> snapX][player -> snapY-2] = 2;
						player -> move(UP);
					}
				}
				break;
			case RIGHT:
				player -> currentFrame = 1;
				if (floorCode[player -> snapX+1][player -> snapY] == 1) 
				{
					player -> move(RIGHT);
				}
				else if (floorCode[player -> snapX+1][player -> snapY] == 2) 
				{
					if (floorCode[player -> snapX+2][player -> snapY] == 1)
					{
						for (int i = 0; i < aniSprites.size(); i++)
						{
							if (aniSprites[i] -> x == player -> snapX + 1 && aniSprites[i] -> y == player -> snapY)
							{
								aniSprites[i] -> move(RIGHT);
							}
						}
						floorCode[player -> snapX+1][player -> snapY] = 1;
						floorCode[player -> snapX+2][player -> snapY] = 2;
						player -> move(RIGHT);
					}
				}
				break;
			case DOWN:
				player -> currentFrame = 2;
				if (floorCode[player -> snapX][player -> snapY+1] == 1) 
				{
					player -> move(DOWN);
				}
				else if (floorCode[player -> snapX][player -> snapY+1] == 2) 
				{
					if (floorCode[player -> snapX][player -> snapY+2] == 1)
					{
						for (int i = 0; i < aniSprites.size(); i++)
						{
							if (aniSprites[i] -> x == player -> snapX && aniSprites[i] -> y == player -> snapY + 1)
							{
								aniSprites[i] -> move(DOWN);
							}
						}
						floorCode[player -> snapX][player -> snapY+1] = 1;
						floorCode[player -> snapX][player -> snapY+2] = 2;
						player -> move(DOWN);
					}
				}
				break;
			case LEFT:
				player -> currentFrame = 3;
				if (floorCode[player -> snapX-1][player -> snapY] == 1) 
				{
					player -> move(LEFT);
				}
				else if (floorCode[player -> snapX-1][player -> snapY] == 2) 
				{
					if (floorCode[player -> snapX-2][player -> snapY] == 1)
					{
						for (int i = 0; i < aniSprites.size(); i++)
						{
							if (aniSprites[i] -> x == player -> snapX - 1 && aniSprites[i] -> y == player -> snapY)
							{
								aniSprites[i] -> move(LEFT);
							}
						}
						floorCode[player -> snapX-1][player -> snapY] = 1;
						floorCode[player -> snapX-2][player -> snapY] = 2;
						player -> move(LEFT);
					}
				}
				break;
			default:
				break;
			}
			for (int i = 0; i < aniSprites.size(); i++)
			{
//std::cout << floorCode[aniSprites[i] -> snapX][aniSprites[i] -> snapY] << " : " << aniSprites[i] -> loopFrameStart << "-" << aniSprites[i] -> loopFrameEnd << std::endl;
			}
			
			//std::cout << player -> snapX << " : " << player -> snapY << std::endl << "   " << floorCode[player -> snapX][player -> snapY-1] << std::endl << floorCode[player -> snapX-1][player -> snapY] << "	  " << floorCode[player -> snapX+1][player -> snapY] << std::endl << "   " << floorCode[player -> snapX][player -> snapY+1] << std::endl << std::endl;
		}
		float dt = float(SDL_GetTicks()-last)/1000.0; //Millisecond timer
		
		//set camera position
		camera.x = player -> x*50 - camera.w/2;
		camera.y = player -> y*50 - camera.h/2;
		if (camera.x < 0) camera.x = 0;
		else if (camera.x+camera.w > levelWidth) camera.x = levelWidth - camera.w;
		if (camera.y < 0) camera.y = 0;
		else if (camera.y+camera.h > levelHeight) camera.y = levelHeight - camera.h;
		
		background -> updateLocation(dt, camera.x, camera.y);
		for (unsigned i=0; i<sprites.size();i++)
		{
			sprites[i] -> loop(dt, sprites[i] -> loopFrameStart, sprites[i] -> loopFrameEnd);
			sprites[i] -> updateLocation(dt, camera.x, camera.y);
		}
		for (unsigned i=0; i<aniSprites.size();i++)
		{
			aniSprites[i] -> loop(dt, aniSprites[i] -> loopFrameStart, aniSprites[i] -> loopFrameEnd);
			aniSprites[i] -> updateLocation(dt, camera.x, camera.y);
		}
		player -> updateLocation(dt, camera.x, camera.y);
		
		//Check for scripted events based on player location
		if (dungeon == 1)
		{
			if (floor == 1)
			{
				if (player -> x == levelChange[1].x && player -> y == levelChange[1].y) //Go outside
				{
					floor += 1;
					player -> setXY(5,1);
					loadLevel(dungeon,floor,levelWidth,levelHeight,background,floorCode,sprites,aniSprites,renderer,levelChange);
					audio = SDL_CreateThread (fadeTheMusic, "Music Stuff", (void *)NULL);
					std::ofstream writeFile ("Save.txt");
					writeFile << dungeon << " " << floor << " " << player -> snapX << " " << player -> snapY;
					writeFile.close();
				}
			}
			else if (floor == 2)
			{
				if (player -> x == levelChange[0].x && player -> y == levelChange[0].y) //Go inside house
				{
					floor -= 1;
					player -> setXY(10, 8);
					loadLevel(dungeon,floor,levelWidth,levelHeight,background,floorCode,sprites,aniSprites,renderer,levelChange);
					audio = SDL_CreateThread (fadeTheMusic, "Music Stuff", (void *)NULL);
					std::ofstream writeFile ("Save.txt");
					writeFile << dungeon << " " << floor << " " << player -> snapX << " " << player -> snapY;
					writeFile.close();
				}
			}
		}
		last = SDL_GetTicks();
		SDL_RenderClear(renderer);
		
		background -> draw();
		for (unsigned i=0; i<sprites.size();i++)
		{
			sprites[i] -> draw();
		}
		for (unsigned i=0; i<aniSprites.size();i++)
		{
			aniSprites[i] -> draw();
		} 
		player -> draw();
		SDL_RenderPresent(renderer);
	}
	
	for (unsigned i=0; i<sprites.size();i++)
	{
		sprites[i] -> ~SpriteTemplate();
	}
	//saveCurrentLevel(dungeon, floor, levelWidth, levelHeight, floorCode);
	player -> ~SpriteTemplate();
	//system("pause");
	Mix_FreeMusic(gMusic);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(win);
	Mix_Quit();
	SDL_Quit();
	return 0;
}

void loadLevel(int dungeon, int floor, int &levelWidth, int &levelHeight, StaticSprite *&background, int floorCode[maxTilesX][maxTilesY], std::vector<StaticSprite *> &sprites, std::vector<MovingSprite *> &aniSprites, SDL_Renderer *renderer, coordinate levelChange[3])
{
	int numberOfSprites, starting, ending;
	int mapX = 0; int mapY = 0;
	std::string parse;
	std::stringstream s;
	std::stringstream levelFile;
	std::vector<params *> spriteParams;
	std::vector<std::string> levelLayout;
	StaticSprite *spriteRead;
	MovingSprite *spriteRead2;
	
	s << dungeon << "_" << floor << ".txt";
	if (test) background -> ~SpriteTemplate();
	background = new StaticSprite (s.str(), renderer);
	background -> currentFrame = 0;
	background -> setXY((background -> animation[background -> currentFrame] -> dest.w / 100) - 0.5,(background -> animation[background -> currentFrame] -> dest.h / 50) - 1);
	levelWidth = background -> animation[background -> currentFrame] -> dest.w;
	levelHeight = background -> animation[background -> currentFrame] -> dest.h;
	
	levelChange[0].x = -1;
	levelChange[1].x = -1;
	levelChange[2].x = -1;
	levelChange[0].y = -1;
	levelChange[1].y = -1;
	levelChange[2].y = -1;
	
	for (int i = 0; i < sprites.size(); i++)
	{
		sprites[i] -> ~SpriteTemplate();
	}
	sprites.clear();
	for (int i = 0; i < aniSprites.size(); i++)
	{
		aniSprites[i] -> ~SpriteTemplate();
	}
	aniSprites.clear();
	
	levelFile << "Maps\\" << dungeon << "_" << floor << ".txt";
	std::ifstream readFile (levelFile.str().c_str());
	readFile >> song >> numberOfSprites;
	std::getline(readFile, parse);
	
	for (int i = 0; i < numberOfSprites; i++)
	{
		std::stringstream ss;
		params *temp = new params;
		std::getline(readFile, parse);
		ss.str(parse);
		ss >> temp -> fileLocation >> temp -> blocking;
		if (ss >> starting)
		{
			temp -> starting = starting;
		}
		else temp -> starting = 0;
		if (ss >> ending)
		{
			temp -> ending = ending;
		}
		else temp -> ending = 0;
		
		spriteParams.push_back(temp);
	}
	while (std::getline(readFile, parse))
	{
		levelLayout.push_back(parse);
	}
	for (int y = 0; y < levelLayout.size(); y++)
	{
//std::cout << std::endl;
		for (int x = 0; x < levelLayout[y].length(); x++)
		{
//std::cout << levelLayout[y][x];
			if (levelLayout[y][x] == '#')
			{
				floorCode[x][y] = 0;
			} 
			else if (levelLayout[y][x] == '.'){
				floorCode[x][y] = 1;
			}
			else if (levelLayout[y][x] == '<'){
				levelChange[0].x = x;
				levelChange[0].y = y;
				floorCode[x][y] = 1;
			}
			else if (levelLayout[y][x] == '>'){
				levelChange[1].x = x;
				levelChange[1].y = y;
				floorCode[x][y] = 1;
			}
			else if (levelLayout[y][x] == '?'){
				levelChange[2].x = x;
				levelChange[2].y = y;
				floorCode[x][y] = 1;
			}
			else if (levelLayout[y][x] - '0' <= numberOfSprites) {
				floorCode[x][y] = spriteParams[(levelLayout[y][x] - '0') - 1] -> blocking;
				if (floorCode[x][y] == 2)
				{
					spriteRead2 = new MovingSprite(spriteParams[(levelLayout[y][x] - '0') - 1] -> fileLocation, renderer);
					spriteRead2 -> setXY(x, y);
					spriteRead2 -> setSpeed(playerSpeed);
					spriteRead2 -> display = true;
					if (spriteParams[(levelLayout[y][x] - '0') - 1] -> starting)
					{
						spriteRead2 -> loopFrameStart = spriteParams[(levelLayout[y][x] - '0') - 1] -> starting;
						spriteRead2 -> currentFrame = spriteRead2 -> loopFrameStart;
					}
					if (spriteParams[(levelLayout[y][x] - '0') - 1] -> ending)
					{
						spriteRead2 -> loopFrameEnd = spriteParams[(levelLayout[y][x] - '0') - 1] -> ending;
					}
//std::cout << floorCode[x][y] << ": " << spriteRead2 -> loopFrameStart << "-" << spriteRead -> loopFrameEnd << std::endl;
					aniSprites.push_back(spriteRead2);
				}
				else {
					spriteRead = new StaticSprite(spriteParams[(levelLayout[y][x] - '0') - 1] -> fileLocation, renderer);
					spriteRead -> setXY(x, y);
					spriteRead -> display = true;
					if (spriteParams[(levelLayout[y][x] - '0') - 1] -> starting)
					{
						spriteRead -> loopFrameStart = spriteParams[(levelLayout[y][x] - '0') - 1] -> starting;
						spriteRead -> currentFrame = spriteRead -> loopFrameStart;
					}
					if (spriteParams[(levelLayout[y][x] - '0') - 1] -> ending)
					{
						spriteRead -> loopFrameEnd = spriteParams[(levelLayout[y][x] - '0') - 1] -> ending;
					}
//std::cout << floorCode[x][y] << ": " << spriteRead -> loopFrameStart << "-" << spriteRead -> loopFrameEnd << std::endl;
					sprites.push_back(spriteRead);
				}
			}	
		}
	}
}

int fadeTheMusic(void* data)
{
	while (!baton) {}
	baton = false; //Makes it so only one fadeTheMusic can run at a time
	if (Mix_PlayingMusic())
	{
		Mix_FadeOutMusic(1000);
	}
	gMusic = Mix_LoadMUS(song.c_str());
	Mix_PlayMusic(gMusic, -1);
	baton = true;
	return 0;
}

/*void changeLevel(int dungeon, int floor, int levelWidth, int levelHeight, int x, int y, char replaceWith)
{
	std::stringstream s;
	s << "Maps\\" << dungeon << "_" << floor << ".txt";
//std::cout << ((levelWidth/50)*(levelHeight/50 - y) + levelWidth/50 - x) << std::endl;
//std::cout << "Width: " << (levelWidth/50) << std::endl;
//std::cout << "Height: " << (levelHeight/50) << std::endl;
//std::cout << "Left: " << levelWidth/50 - x << std::endl;
//std::cout << "Up: " << (levelWidth/50)*(levelHeight/50 - y)/50 << std::endl << std::endl;

	std::fstream saveFile (s.str().c_str(), std::ios::in | std::ios::out);
	saveFile.seekg(-(((levelWidth/50)+1)*(levelHeight/50 - y) + (((levelWidth/50)+1) - x)), std::ios::end);
	saveFile << replaceWith;
}*/

/*void saveCurrentLevel(int dungeon, int floor, int levelWidth, int levelHeight, int floorCode[maxTilesX][maxTilesY])
{
	std::stringstream s;
	std::string parse;
	char rewrite;
	int numberOfSprites;
	
	s << "Maps\\" << dungeon << "_" << floor << ".txt";
	std::fstream saveFile (s.str().c_str());
	s.str("");
	saveFile >> parse >> numberOfSprites;
	std::getline(saveFile, parse);
	for (int i = 0; i < numberOfSprites; i++)
	{
		std::getline(saveFile, parse);
	}
	for (int y = 0; y < levelHeight/50; y++)
	{
		std::getline(saveFile, parse);
std::cout << parse << std::endl;
		for (int x = 0; x < levelWidth/50; x++)
		{
std::cout << x << ", " << y << ": " << parse[x] << std::endl;
			if (!(parse[x] == '#' && floorCode[x][y] == 0) || 
				!(parse[x] == '.' && floorCode[x][y] == 1) ||
				!(parse[x] == floorCode[x][y]))
			{
std::cout << parse[x] << " : " << floorCode[x][y] << std::endl;
//std::cout << "XY: " << x << ", " << y << std::endl << "Before: " << rewrite << std::endl << "FloorCode: " << floorCode[x][y] << std::endl << std::endl;

				//saveFile.seekg(-1, std::ios::cur);
				switch (floorCode[x][y])
				{
				case 0: 
					parse[x] = '#';
					break;
				case 1:
					parse[x] = '.';
					break;
				default:
					parse[x] = floorCode[x][y];
					break;
				}
			}
			
		}
	}
}*/