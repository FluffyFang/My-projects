#include <iostream>
#include <string>
#include "movingSprite.h"
#include <SDL.h>

MovingSprite::MovingSprite(std::string filename, SDL_Renderer *renderer) :
	SpriteTemplate (filename, renderer)
{
	interactable = false;
	snapX = 0;
	snapY = 0;
	actionTimer = 0;
	vx = 0;
	vy = 0;
	movespeed = 0;
}


void MovingSprite::updateLocation(float dt, int cameraX, int cameraY) 
{
    x += vx*dt;
    y += vy*dt;
    
    if (actionTimer < dt) 
    {
        x = snapX;
        y = snapY;
		vx = 0;
        vy = 0;
		actionTimer = 0;
    }
    else actionTimer -= dt;
    animation[currentFrame] -> dest.x = (int)(x*gridToPixels + gridToPixels/2 - animation[currentFrame] -> dest.w/2 - cameraX);
    animation[currentFrame] -> dest.y = (int)(y*gridToPixels + gridToPixels - animation[currentFrame] -> dest.h - cameraY);
}

void MovingSprite::move(int direction) // 1=up; 2=right; 3=down; 4=left
{
	actionTimer = 1/movespeed;
	switch (direction)
	{
	case 1:
		vy -= movespeed;
		snapY -= 1;
		break;
	case 2:
		vx += movespeed;
		snapX += 1;
		break;
	case 3:
		vy += movespeed;
		snapY += 1;
		break;
	case 4:
		vx -= movespeed;
		snapX -= 1;
		break;
	}
}

void MovingSprite::setSpeed (float _s)
{
	movespeed = _s;
}

void MovingSprite::setXY(float _x, float _y)
{//x, y
	snapX = _x;
	snapY = _y;
	x = _x;
	y = _y;
}