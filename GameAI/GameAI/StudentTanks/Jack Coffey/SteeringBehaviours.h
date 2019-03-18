#ifndef STEERINGBEHAVIOURS
#define STEERINGBEHAVIOURS

#include <vector>
#include "../../Commons.h"
#include <string>

class C016193GTank;

struct behaviours {
	bool seek = false;
	bool flee = false;
};

class SteeringBehaviours
{
public:
	enum calculateType { weighted, prioritzed };
	void SetCalculateType(calculateType type) { m_CalculateType = type; }

	Vector2D Calculate();

	void SetTarget(Vector2D target) { m_vTarget = target; }
	Vector2D GetTarget() { return m_vTarget; }

	void SeekOn() { m_bCurrentBehaviours->seek = true; }
	void SeekOff() { m_bCurrentBehaviours->seek = false; }

	void FleeOn() { m_bCurrentBehaviours->flee = true; }
	void FleeOff() { m_bCurrentBehaviours->flee = false; }

	SteeringBehaviours(C016193GTank* tank) {m_Tank = tank; }
	~SteeringBehaviours() {}
private:
	Vector2D m_vSteeringForce;
	Vector2D m_vTarget;

	C016193GTank* m_Tank;

	behaviours* m_bCurrentBehaviours;
	calculateType m_CalculateType;

	bool AccumulateForce(Vector2D runningTotal, Vector2D forceToAdd);
	
	void CalculateWeightedSum();
	void CalculatePrioritized();

	Vector2D Seek(Vector2D targetPos);
	Vector2D Flee(Vector2D targetPos);
	Vector2D Pursuit();
};

#endif


