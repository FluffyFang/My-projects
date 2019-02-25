#include <SDL.h>
#include <vector>
#include <string>
#include "spriteTemplate.h"
class StaticSprite : public SpriteTemplate
{
public:
	StaticSprite(std::string filename, SDL_Renderer *renderer);
	bool interactable;
	bool background;
	void setXY(float _x, float _y);
	void updateLocation(float dt, int cameraX, int cameraY);
};