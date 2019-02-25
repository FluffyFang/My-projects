#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "spriteTemplate.h"
#include <SDL.h>

SpriteTemplate::SpriteTemplate (std::string aniFile, SDL_Renderer *newScreen)
{
	loopFrameStart = 0;
	loopFrameEnd = 0;
	currentFrame = 0;
	screen = newScreen;
	SDL_Surface *bitmapSurface;
	std::ifstream in (aniFile.c_str());
	std::string path;
	std::string filename;
	int time;

	in >> totalFrames >> path;
	loopFrameEnd = totalFrames - 1;
	for (int i = 0; i < totalFrames; i++)
	{
		std::stringstream s;
		in >> filename >> time;
		s << path << "/" << filename;
		objectTime += time;
		bitmapSurface = SDL_LoadBMP(s.str().c_str());
		int WhiteKey=SDL_MapRGB(bitmapSurface->format,255,255,255);
		SDL_SetColorKey(bitmapSurface,SDL_TRUE,WhiteKey);
		
		frame *temp = new frame;
		temp -> bitmap = SDL_CreateTextureFromSurface(screen, bitmapSurface);
		temp -> frameDuration = time;
		temp -> dest.w = bitmapSurface -> w;
		temp -> dest.h = bitmapSurface -> h;
		temp -> dest.x = 0;
		temp -> dest.y = 0;
		animation.push_back(temp);
	}
	x = 0.0; //grid location
	y = 0.0;
	display = true;
	SDL_FreeSurface(bitmapSurface);
}

bool SpriteTemplate::loop(float dt, int start, int end)
{
//std::cout << "test1.1" << " - " << currentFrame << " out of " << totalFrames << std::endl;
    objectTime += dt;
	bool loopFinish = false;
    if (objectTime*1000 >= animation[currentFrame] -> frameDuration)
    {
//std::cout << "test1.2: " << std::endl;
        currentFrame++;
        //totalTime -= animation[currentFrame] -> frameDuration;
        objectTime = 0;
		if (currentFrame == end || currentFrame == totalFrames - 1)//if on last frame
		{
			loopFinish = true;
		}
        else if (currentFrame > end || currentFrame >= totalFrames) //if past last frame
        {
            //reset to beginning of animation
            currentFrame = start;
        }
    }
}

void SpriteTemplate::draw()
{
	if (display)
	{
		SDL_RenderCopy(screen, animation[currentFrame] -> bitmap, NULL, &animation[currentFrame] -> dest);
	}
}

SpriteTemplate::~SpriteTemplate()
{
	for (unsigned i=0; i<animation.size();i++)
	{
		if (animation[i] -> bitmap)
		{
			SDL_DestroyTexture(animation[i] -> bitmap);
		}
	}
}