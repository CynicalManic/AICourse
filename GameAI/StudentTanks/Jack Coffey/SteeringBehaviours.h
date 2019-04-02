#ifndef STEERINGBEHAVIOURS
#define STEERINGBEHAVIOURS

#include <vector>
#include "../../Commons.h"
#include <string>

class C016193GTank;

struct behaviours {
	bool seek = false;
	bool flee = false;
	bool wallAvoidance = true;
	bool arrive = true;
};

class SteeringBehaviours
{
public:
	enum calculateType { weighted, prioritzed };
	void SetCalculateType(calculateType type) { m_CalculateType = type; }

	Vector2D Calculate();

	void SetTarget(Vector2D target) { m_vTarget = target; }
	Vector2D GetTarget() { return m_vTarget; }

	void ArriveOn() { m_bCurrentBehaviours->arrive = true; }
	void ArriveOff() { m_bCurrentBehaviours->arrive = false; }

	void SeekOn() { m_bCurrentBehaviours->seek = true; }
	void SeekOff() { m_bCurrentBehaviours->seek = false; }

	void FleeOn() { m_bCurrentBehaviours->flee = true; }
	void FleeOff() { m_bCurrentBehaviours->flee = false; }

	void wallAvoidanceOn() { m_bCurrentBehaviours->wallAvoidance = true; }
	void wallAvoidanceOff() { m_bCurrentBehaviours->wallAvoidance = false; }

	SteeringBehaviours(C016193GTank* tank) {m_Tank = tank; }
	~SteeringBehaviours() {}
private:
	Vector2D m_vSteeringForce;
	Vector2D m_vTarget;
	vector<Vector2D> m_vFeelers;
	double feelerLength = 50;

	C016193GTank* m_Tank;

	behaviours* m_bCurrentBehaviours = new behaviours();
	calculateType m_CalculateType = weighted;

	enum Deceleration{slow = 3, normal = 2, fast = 1};
	Deceleration m_Deceleration = normal;

	float m_fArriveDistance = 200;

	bool AccumulateForce(Vector2D runningTotal, Vector2D forceToAdd);
	
	void CalculateWeightedSum();
	void CalculatePrioritized();
	void SetupFeelers();
	bool CheckPointCollision(vector<Vector2D> object, Vector2D point);
	bool CheckLineIntersection(Vector2D tankPos, Vector2D feelerPos, Vector2D wall1, Vector2D wall2, double& distance, Vector2D& point);
	
	Vector2D GetPos();

	Vector2D Seek(Vector2D targetPos);
	Vector2D Flee(Vector2D targetPos);
	Vector2D Arrive(Vector2D targetPos);
	Vector2D WallAvoidance();
	Vector2D Pursuit();

	

	float SeekWeighting = 0.3f;
	float ArriveWeighting = 0.3f;
	float wallAvoidanceWeighting = 0.7f;
};

#endif


