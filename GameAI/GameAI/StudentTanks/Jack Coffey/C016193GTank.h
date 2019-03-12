#ifndef C016193GTank_H
#define C016193GTank_H

#include <SDL.h>
#include "../../Commons.h"
#include "../../BaseTank.h"
#include "../../StudentTanks/Jack Coffey/GraphNode.h"

class BaseTank;

//---------------------------------------------------------------

class C016193GTank : public BaseTank
{
	//---------------------------------------------------------------
public:
	C016193GTank(SDL_Renderer* renderer, TankSetupDetails details);
	~C016193GTank();

	void ChangeState(BASE_TANK_STATE newState);

	void Update(float deltaTime, SDL_Event e);

	void RotateHeadingByRadian(double radian, int sign);

	//---------------------------------------------------------------
protected:
	void	MoveInHeadingDirection(float deltaTime);
	void SearchBFS(GraphNode* rootNode);
	void SearchDFS(GraphNode* rootNode);

private:

};

//---------------------------------------------------------------

#endif //C016193GTank_H