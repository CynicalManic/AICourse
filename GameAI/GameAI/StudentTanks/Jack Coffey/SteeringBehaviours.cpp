#include "SteeringBehaviours.h"
#include "C016193GTank.h"



Vector2D SteeringBehaviours::Calculate()
{
	// Reset Steering Force
	m_vSteeringForce.Zero();

	// Choose which calculation to use
	switch (m_CalculateType)
	{
	case weighted:
		CalculateWeightedSum(); break;
	case prioritzed:
		CalculatePrioritized(); break;
	default:
		m_vSteeringForce = Vector2D(0, 0); break;
	}


	return m_vSteeringForce;
}

void SteeringBehaviours::CalculateWeightedSum()
{
	if (m_bCurrentBehaviours->seek)
	{
		m_vSteeringForce += Seek(GetTarget());
	}
	if (m_bCurrentBehaviours->flee)
	{

	}
}

void SteeringBehaviours::CalculatePrioritized()
{
	Vector2D force;

	if (m_bCurrentBehaviours->seek)
	{
		force = Seek(GetTarget());
	}
	if (m_bCurrentBehaviours->flee)
	{
		force = Flee(GetTarget());
	}
}

Vector2D SteeringBehaviours::Seek(Vector2D targetPos)
{
	Vector2D velocity;
	velocity = targetPos;
	return velocity;
}

Vector2D SteeringBehaviours::Flee(Vector2D targetPos)
{
	return Vector2D();
}

Vector2D SteeringBehaviours::Pursuit()
{
	return Vector2D();
}

bool SteeringBehaviours::AccumulateForce(Vector2D runningTotal, Vector2D forceToAdd)
{
	return false;
}

