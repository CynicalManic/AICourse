#ifndef C016193GTank_H
#define C016193GTank_H

#include <SDL.h>
#include "../../Commons.h"
#include "../../BaseTank.h"
#include "../../StudentTanks/Jack Coffey/GraphNode.h"
#include <queue>
#include <stack>

class BaseTank;
class SteeringBehaviours;
class MyPathFinder_DRS;


//---------------------------------------------------------------

class C016193GTank : public BaseTank
{
	//---------------------------------------------------------------
public:
	C016193GTank(SDL_Renderer* renderer, TankSetupDetails details);
	~C016193GTank();

	vector<GameObject*> GetObstacles();

	void ChangeState(BASE_TANK_STATE newState);

	void Update(float deltaTime, SDL_Event e);

	void RotateHeadingByRadian(double radian, int sign);

	void SetMaxSpeed(double maxSpeed);

	//---------------------------------------------------------------
protected:
	void MoveInHeadingDirection(float deltaTime);
	void SearchBFS(GraphNode* rootNode, GraphNode* endNode);
	void SearchDFS(GraphNode* rootNode);
	void UpdateBehaviours(float deltaTime);
	void UpdateObstacles();
	bool RotateHeadingToFacePosition(Vector2D target, float deltaTime);
	void RotateHeadingByRadian(double radian, int sign, float deltaTime);

	std::stack<GraphNode*> GetReturnStack(GraphNode* endNode);

	Vector2D m_vPos;
	Vector2D m_vHeading;
	Vector2D m_vTarget;	
	Vector2D m_vVelocity;

	double m_dTurnSpeed = 10;
	double m_dMaxSpeed;
	
	vector<GameObject*> m_obstacles;
	vector<BaseTank*> m_visibleTanks;
	vector<BaseTank*> m_audibleTanks;

	SteeringBehaviours* m_steeringBehaviours;

	MyPathFinder_DRS* m_pathfinder;

private:

};

//---------------------------------------------------------------

#endif //C016193GTank_H