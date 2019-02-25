#pragma once
#include <SDL.h>
#include <vector>
#include <string>
class SpriteTemplate
{
protected:
	static const int gridToPixels = 50;
	SDL_Texture *bitmapTex;
	SDL_Renderer *screen;
	int screenW, screenH, totalFrames;
	float objectTime;
public:
	struct frame
	{
		SDL_Texture *bitmap;
		SDL_Rect dest; //pixel location
		SDL_Rect src;
		int frameDuration;
	};
	std::vector<frame *> animation;
	SpriteTemplate (std::string filename, SDL_Renderer *renderer);
	bool display;
	int loopFrameStart;
	int loopFrameEnd;
	int currentFrame;
	float x, y; //Exact location
	bool loop(float dt, int start, int end);
	virtual void setXY (float _x, float _y) = 0;
	void draw();
	~SpriteTemplate();
};