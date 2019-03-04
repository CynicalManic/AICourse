#include "C016193GTank.h"
#include "../../TankManager.h"
#include "../../Commons.h"
#include "../../C2DMatrix.h"

//--------------------------------------------------------------------------------------------------

C016193GTank::C016193GTank(SDL_Renderer* renderer, TankSetupDetails details) : BaseTank(renderer, details)
{

}

//--------------------------------------------------------------------------------------------------

C016193GTank::~C016193GTank()
{
}

//--------------------------------------------------------------------------------------------------

void C016193GTank::ChangeState(BASE_TANK_STATE newState)
{
	BaseTank::ChangeState(newState);
}

//--------------------------------------------------------------------------------------------------

void C016193GTank::Update(float deltaTime, SDL_Event e)
{
	//Call parent update.
	BaseTank::Update(deltaTime, e);
}

//--------------------------------------------------------------------------------------------------


void C016193GTank::MoveInHeadingDirection(float deltaTime)
{
	
}

//--------------------------------------------------------------------------------------------------

void C016193GTank::RotateHeadingByRadian(double radian, int sign)
{
	
}

//--------------------------------------------------------------------------------------------------