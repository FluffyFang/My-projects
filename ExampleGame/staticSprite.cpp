#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "staticSprite.h"
#include <SDL.h>

StaticSprite::StaticSprite(std::string filename, SDL_Renderer *renderer) :
	SpriteTemplate (filename, renderer)
{
	background = false;
	interactable = false;
}

void StaticSprite::setXY(float _x, float _y)
{
	x = _x;
	y = _y;
}

void StaticSprite::updateLocation(float dt, int cameraX, int cameraY) 
{
    animation[currentFrame] -> dest.x = (int)(x*gridToPixels + gridToPixels/2 - animation[currentFrame] -> dest.w/2 - cameraX);
    animation[currentFrame] -> dest.y = (int)(y*gridToPixels + gridToPixels - animation[currentFrame] -> dest.h - cameraY);
}