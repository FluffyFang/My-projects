#include <vector>
class Enemy : public MovingSprite
{
	int detectionRange;
	bool isHostile;
	std::vector<int *> idleActions;
	
};