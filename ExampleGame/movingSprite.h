#include <SDL.h>
#include <string>
#include "spriteTemplate.h"
class MovingSprite : public SpriteTemplate
{
public:
	MovingSprite(std::string filename, SDL_Renderer *renderer);
	bool interactable;
	int snapX, snapY;
	float actionTimer;
	float vx, vy;
	float movespeed;
	void setSpeed (float _s);
	void move (int direction);
	void updateLocation(float dt, int cameraX, int cameraY);
	void setXY(float _x, float _y);
};