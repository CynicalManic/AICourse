#include "C016193GTank.h"
#include "../../TankManager.h"
#include "../../Commons.h"
#include "../../C2DMatrix.h"
#include "SteeringBehaviours.h"
#include "../../ObstacleManager.h"
#include "MyPathFinder_DRS.h"


//--------------------------------------------------------------------------------------------------

C016193GTank::C016193GTank(SDL_Renderer* renderer, TankSetupDetails details) : BaseTank(renderer, details)
{

	//Set up tree structure from lecture.
	GraphNode* pNodeA = new GraphNode('A');
	GraphNode* pNodeB = new GraphNode('B');
	GraphNode* pNodeC = new GraphNode('C');
	GraphNode* pNodeD = new GraphNode('D');
	pNodeA->AddAChildNode(pNodeB);
	pNodeA->AddAChildNode(pNodeC);
	pNodeA->AddAChildNode(pNodeD);

	GraphNode* pNodeE = new GraphNode('E');
	GraphNode* pNodeF = new GraphNode('F');
	pNodeB->AddAChildNode(pNodeE);
	pNodeB->AddAChildNode(pNodeF);

	GraphNode* pNodeG = new GraphNode('G');
	pNodeC->AddAChildNode(pNodeG);

	GraphNode* pNodeH = new GraphNode('H');
	GraphNode* pNodeI = new GraphNode('I');
	pNodeD->AddAChildNode(pNodeH);
	pNodeD->AddAChildNode(pNodeI);
	//-----------------------------------------------------------------------------


	//Depth First Search.
	//SearchDFS(pNodeA);

	//Breadth First Search.
	cout << endl;
	SearchBFS(pNodeA, pNodeI);
	cout << endl;
	GetReturnStack(pNodeI);
	cout << endl;

	m_steeringBehaviours = new SteeringBehaviours(this);
	m_vTarget = GetCentralPosition();
	m_steeringBehaviours->SetTarget(m_vTarget);
	m_dMaxSpeed = GetMaxSpeed();

	m_pathfinder = new MyPathFinder_DRS(renderer, mCollisionMap);
}


bool C016193GTank::RotateHeadingToFacePosition(Vector2D target, float deltaTime)
{
	Vector2D toTarget = Vec2DNormalize(target - GetCentralPosition());

	//Determine the angle between the heading vector and the target.
	double angle = acos(mHeading.Dot(toTarget));

	

	//Ensure angle does not become NaN and cause the tank to disappear.
	if (angle != angle)
		angle = 0.0f;

	//Return true if the player is facing the target.
	if (angle < 0.01f)
		return true;

	RotateHeadingByRadian(angle, mHeading.Sign(toTarget), deltaTime);

	return false;
}

void C016193GTank::RotateHeadingByRadian(double radian, int sign, float deltaTime)
{
	//Incorporate delta time.
	radian *= deltaTime * m_dTurnSpeed;

	//Clamp the amount to turn to the max turn rate.
	if (radian > mMaxTurnRate)
		radian = mMaxTurnRate;
	else if (radian < -mMaxTurnRate)
		radian = -mMaxTurnRate;

	//IncrementTankRotationAngle(RadsToDegs(radian));
	mRotationAngle += RadsToDegs(radian)*sign;

	//Usee a rotation matrix to rotate the player's heading
	C2DMatrix RotationMatrix;

	//Calculate the direction of rotation.
	RotationMatrix.Rotate(radian * sign);
	//Get the new heading.
	RotationMatrix.TransformVector2Ds(mHeading);

	//Side vector must always be perpendicular to the heading.
	mSide = mHeading.Perp();
}

void C016193GTank::SearchBFS(GraphNode * rootNode, GraphNode* endNode)
{
	if (rootNode == endNode)
	{
		return;
	}

	queue<GraphNode*> queue;
	queue.push(rootNode);
	GraphNode* current;
	
	queue.front()->SetVisited(true);

	while (!queue.empty())
	{
		current = queue.front();
		cout << current->GetID();
		queue.pop();

		if (current == endNode)		
			break;		

		std::vector<GraphNode*> children = current->GetChildNodes();
		for (int i = 0; i < children.size(); i++)
		{
			if (!children[i]->GetVisited())
			{
				children[i]->SetReturnNode(current);
				children[i]->SetVisited(true);
				queue.push(children[i]);
			}
		}		
	}
}

void C016193GTank::SearchDFS(GraphNode * rootNode)
{
	
}

void C016193GTank::UpdateBehaviours(float deltaTime)
{
}

void C016193GTank::UpdateObstacles()
{
	mTanksICanHear;
	mTanksICanSee;

	if (GetKeyState(VK_LBUTTON) < 0)
	{
		POINT mouse;
		GetCursorPos(&mouse);

		HWND hWnd = GetActiveWindow();
		ScreenToClient(hWnd, &mouse);

		m_vTarget.x = mouse.x;
		m_vTarget.y = mouse.y;
		m_steeringBehaviours->SetTarget(m_vTarget);

	}
	
}

std::stack<GraphNode*> C016193GTank::GetReturnStack(GraphNode * endNode)
{
	std::stack<GraphNode*> stack;
	GraphNode* current = endNode;
	stack.push(current);

	while (current->GetReturnNode() != nullptr)
	{
		current = current->GetReturnNode();
		stack.push(current);
	}

	while (stack.empty() != true)
	{
		current = stack.top();
		cout << current->GetID();
		stack.pop();
	}

	return stack;
}

//--------------------------------------------------------------------------------------------------

C016193GTank::~C016193GTank()
{
}

vector<GameObject*> C016193GTank::GetObstacles()
{
	return ObstacleManager::Instance()->GetObstacles();
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
	UpdateBehaviours(deltaTime);
	UpdateObstacles();
	MoveInHeadingDirection(deltaTime);
	
}

void C016193GTank::SetMaxSpeed(double maxSpeed)
{
	m_dMaxSpeed = maxSpeed;
	if (m_dMaxSpeed > GetMaxSpeed())
		m_dMaxSpeed = GetMaxSpeed();	
}

//--------------------------------------------------------------------------------------------------


void C016193GTank::MoveInHeadingDirection(float deltaTime)
{
	Vector2D steeringForce = m_steeringBehaviours->Calculate();
	m_vHeading = steeringForce;
	m_vHeading.Normalize();
	bool isFacingTarget;
	
	// Rotate tank to face target
	// Check if the tank is currently facing the target
	if (m_vHeading == Vector2D(0, 0))
	{
		isFacingTarget = false;
	}
	else
	{
		Vector2D headingTarget = GetCentralPosition() + m_vHeading;
		isFacingTarget = RotateHeadingToFacePosition(headingTarget, deltaTime);
	}
	
	Vector2D CurrentHeading = GetHeading();
	CurrentHeading.Normalize();
	
	if (isFacingTarget)
	{
		double desiredSpeed = hypot(steeringForce.x, steeringForce.y);
		if (desiredSpeed > mCurrentSpeed)
		{
			mCurrentSpeed += kSpeedIncrement * deltaTime;
			if (mCurrentSpeed > desiredSpeed)
				mCurrentSpeed = desiredSpeed;

			if (mCurrentSpeed > m_dMaxSpeed)
				mCurrentSpeed = m_dMaxSpeed;		
		}
		else
		{
			mCurrentSpeed -= kSpeedIncrement * deltaTime;
			if (mCurrentSpeed < desiredSpeed)
				mCurrentSpeed = desiredSpeed;

			if (mCurrentSpeed < 0)
				mCurrentSpeed = 0;
		}
	}
	else
	{
		mCurrentSpeed -= kSpeedIncrement * deltaTime;
		if (mCurrentSpeed < 0)
			mCurrentSpeed = 0;
	}
	
	Vector2D force = (CurrentHeading * mCurrentSpeed) - mVelocity;
	Vector2D acceleration = force / GetMass();
	mVelocity += acceleration * deltaTime;
	mVelocity.Truncate(GetMaxSpeed());

	Vector2D newPosition = GetPosition();
		newPosition.x += mVelocity.x*deltaTime;
		newPosition.y += (mVelocity.y/**-1.0f*/)*deltaTime;	//Y flipped as adding to Y moves down screen.
	SetPosition(newPosition);
	
		
}
