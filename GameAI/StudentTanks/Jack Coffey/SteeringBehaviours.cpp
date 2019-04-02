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
		m_vSteeringForce += Seek(GetTarget()) * SeekWeighting;
	}
	if (m_bCurrentBehaviours->arrive)
	{
		m_vSteeringForce += Arrive(GetTarget()) * ArriveWeighting;
	}
	if (m_bCurrentBehaviours->flee)
	{
		m_vSteeringForce += Flee(GetTarget());
	}
	if (m_bCurrentBehaviours->wallAvoidance)
	{
		m_vSteeringForce += WallAvoidance() * wallAvoidanceWeighting;
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

void SteeringBehaviours::SetupFeelers()
{
	double tankSpeed = m_Tank->GetCurrentSpeed();
	Vector2D currentPosition = m_Tank->GetCentralPosition();
	Vector2D currentHeading = m_Tank->GetHeading();

	m_vFeelers.clear();

	// Feeler Pointing Forwards
	Vector2D forwardFeeler = currentPosition + feelerLength * currentHeading;
	m_vFeelers.push_back(forwardFeeler);
	//cout << "Forward: " << m_vFeelers[0].x << " "<< m_vFeelers[0].y << " ";

	// Feeler Pointing Left
	Vector2D temp;
	temp.x = currentHeading.y;
	temp.y = -currentHeading.x;
	Vector2D leftFeeler = currentPosition + feelerLength / 2.0f * temp;
	m_vFeelers.push_back(leftFeeler);
	//cout << "Left: " << m_vFeelers[1].x << " " << m_vFeelers[1].y << " ";

	// Feeler Pointing Right
	temp.x = -currentHeading.y;
	temp.y = currentHeading.x;
	Vector2D rightFeeler = currentPosition + feelerLength / 2.0f * temp;
	m_vFeelers.push_back(rightFeeler);
	//cout << "Right: " << m_vFeelers[2].x << " " << m_vFeelers[2].y << endl;
}

bool SteeringBehaviours::CheckPointCollision(vector<Vector2D> object, Vector2D point)
{
	if (TriangleCollision(object[1], object[2], object[3], point) || TriangleCollision(object[0], object[1], object[3], point))
	{
		return true;
	}
	return false;
}

bool SteeringBehaviours::CheckLineIntersection(Vector2D tankPos, Vector2D feelerPos, Vector2D wall1, Vector2D wall2, double& distance, Vector2D& point)
{
	//Dot Product calculation between tank and wall
	double rTop = (tankPos.y - wall1.y)*(wall2.x - wall1.x) - (tankPos.x - wall1.x)*(wall2.y - wall1.y);
	//Dot Product calculation between feeler and wall
	double rBot = (feelerPos.x - tankPos.x)*(wall2.y - wall1.y) - (feelerPos.y - tankPos.y)*(wall2.x - wall1.x);

	//Dot Product calculation between tank, feeler and wall pos 1
	double sTop = (tankPos.y - wall1.y)*(feelerPos.x - tankPos.x) - (tankPos.x - wall1.x)*(feelerPos.y - tankPos.y);
	//Dot Product calculation between tank, feeler and wall pos 2
	double sBot = (feelerPos.x - tankPos.x)*(wall2.y - wall1.y) - (feelerPos.y - tankPos.y)*(wall2.x - wall1.x);

	if ((rBot == 0) || (sBot == 0))
	{
		//lines are parallel
		return false;
	}
	double r = rTop / rBot;
	double s = sTop / sBot;

	if ((r > 0) && (r < 1) && (s > 0) && (s < 1))
	{
		distance = Vec2DDistance(tankPos, feelerPos) * r;

		point = tankPos + r * (feelerPos - tankPos);

		return true;
	}
	else
	{
		distance = 0;

		return false;
	}
}

Vector2D SteeringBehaviours::GetPos()
{
	Vector2D position;
	position = m_Tank->GetPosition();
	return position;
}

Vector2D SteeringBehaviours::Seek(Vector2D targetPos)
{
	Vector2D velocity;
	velocity = targetPos - GetPos();
	velocity.Normalize();
	return velocity;
}

Vector2D SteeringBehaviours::Flee(Vector2D targetPos)
{
	Vector2D velocity;
	velocity = GetPos() - targetPos;
	velocity.Normalize();
	return velocity;
}

Vector2D SteeringBehaviours::Arrive(Vector2D targetPos)
{
	Vector2D distanceVec = targetPos - GetPos();
	double distance = hypot(distanceVec.x, distanceVec.y);

	if (distance == 0)
	{
		return(Vector2D(0, 0));
	}

	Vector2D velocity;
	if (distance > m_fArriveDistance)
	{
		velocity = targetPos - GetPos();
	}
	else
	{
		velocity = targetPos - GetPos();
		Vector2D slowVector = GetPos() - targetPos;
		float slowMultiplyer = (m_fArriveDistance - distance) / m_fArriveDistance;
		slowVector *= slowMultiplyer;
		cout << slowMultiplyer << endl;

		if (slowMultiplyer > 0.99)
			return Vector2D(0, 0);

		velocity - slowVector;
	}

	return velocity;
}

Vector2D SteeringBehaviours::WallAvoidance()
{
	SetupFeelers();
	vector<GameObject*> obstacles = m_Tank->GetObstacles();
	Vector2D velocity = Vector2D(0,0);

	if (obstacles.capacity() == 0)
	{
		return (Vector2D(0,0));
	}
	for (int i = 0; i < obstacles.capacity(); i++)
	{
		// Obstacle in front
		if (CheckPointCollision(obstacles[i]->GetAdjustedBoundingBox(), m_vFeelers[0]))
		{
			velocity += GetPos() - m_vFeelers[0] * m_Tank->GetCurrentSpeed();
			//cout << "Obstacle Ahead" << endl;
		}
		// Obstacle to the Left
		if (CheckPointCollision(obstacles[i]->GetAdjustedBoundingBox(), m_vFeelers[1]))
		{
			velocity += GetPos() - m_vFeelers[1] * m_Tank->GetCurrentSpeed();
			//cout << "Obstacle to the Left" << endl;
		}
		// Obstacle to the right
		if (CheckPointCollision(obstacles[i]->GetAdjustedBoundingBox(), m_vFeelers[2]))
		{
			velocity += GetPos() - m_vFeelers[2] * m_Tank->GetCurrentSpeed();
			//cout << "Obstacle to the Right" << endl;
		}
	}
	//cout << velocity.x << " " << velocity.y << endl;
	return velocity;
}

Vector2D SteeringBehaviours::Pursuit()
{
	return Vector2D();
}

bool SteeringBehaviours::AccumulateForce(Vector2D runningTotal, Vector2D forceToAdd)
{
	return false;
}
