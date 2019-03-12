#include "C016193GTank.h"
#include "../../TankManager.h"
#include "../../Commons.h"
#include "../../C2DMatrix.h"
#include <queue>

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
	SearchBFS(pNodeA);


}

void C016193GTank::SearchBFS(GraphNode * rootNode)
{
	queue<GraphNode*> queue;
	queue.push(rootNode);
	GraphNode* current;
	
	queue.front()->SetVisited(true);

	while (!queue.empty())
	{
		current = queue.front();
		cout << current->GetID();
		queue.pop();

		std::vector<GraphNode*> children = current->GetChildNodes();
		for (int i = 0; i < children.size(); i++)
		{
			if (!children[i]->GetVisited())
			{
				children[i]->SetVisited(true);
				queue.push(children[i]);
			}
		}
		
	}
}

void C016193GTank::SearchDFS(GraphNode * rootNode)
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