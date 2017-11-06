#include "GroundMovement.h"
#include <queue>

void GroundMovement::from_json(const nlohmann::json & j)
{
}

GroundMovement::GroundMovement() : Movement("GroundMovement")
{
}

GroundMovement::~GroundMovement()
{
}

bool GroundMovement::LineOfSight()
{
	return false;
}

bool GroundMovement::Pathfind(const int & xStart, const int & zStart, const int & xFinish, const int & zFinish)
{
	static priority_queue<Node> untriedNodes[2]; // list of open (not-yet-tried) nodes
	static int nodeIndex = 0;
	static Node* x0;
	static Node* z0;
	static int x, z,
		xdx, //The Current x plus a direction
		zdz; //The Current z plus a direction

	// reset the node maps
	for (z = 0; z < zSize; z++)
	{
		for (x = 0; x < xSize; x++)
		{
			closedNodes[x][z] = 0;
			openNodes[x][z] = 0;
		}
	}

	// create the start node and push into list of open nodes
	x0 = new Node(xStart, zStart, 0, 0);
	x0->UpdatePriority(xFinish, zFinish);
	untriedNodes[nodeIndex].push(*x0);
	openNodes[x][z] = x0->GetPriority(); // mark it on the open nodes map

											  // A* search
	while (!untriedNodes[nodeIndex].empty())
	{
		// get the current node w/ the highest priority
		// from the list of open nodes
		x0 = new Node(untriedNodes[nodeIndex].top().GetxPos(), untriedNodes[nodeIndex].top().GetzPos(),
			untriedNodes[nodeIndex].top().GetDistance(), untriedNodes[nodeIndex].top().GetPriority());

		x = x0->GetxPos(); z = x0->GetzPos();

		untriedNodes[nodeIndex].pop(); // remove the node from the open list
		openNodes[x][z] = 0;
		// mark it on the closed nodes map
		closedNodes[x][z] = 1;

		// quit searching when the goal state is reached
		if (x == xFinish && z == zFinish)
		{
			// generate the path from finish to start
			// by following the directions
			while (!(x == xStart && z == zStart))
			{
				int j = directions[x][z];
				x += dx[j];
				z += dz[j];
			}

			delete x0;
			// empty the leftover nodes
			while (!untriedNodes[nodeIndex].empty())
				untriedNodes[nodeIndex].pop();
			return true;
		}

		// generate moves (child nodes) in all possible directions
		for (int i = 0; i < dir; i++)
		{
			xdx = x + dx[i];
			zdz = z + dz[i];

			if (!(xdx<0 || xdx>xSize - 1 || zdz<0 || zdz>zSize - 1 || nodeMap[xdx][zdz] == 1
				|| closedNodes[xdx][zdz] == 1))
			{
				// generate a child node
				z0 = new Node(xdx, zdz, x0->GetDistance(),
					x0->GetPriority());
				z0->NextDistance(i);
				z0->UpdatePriority(xFinish, zFinish);

				// if it is not in the open list then add into that
				if (openNodes[xdx][zdz] == 0)
				{
					openNodes[xdx][zdz] = z0->GetPriority();
					untriedNodes[nodeIndex].push(*z0);
					// mark its parent node direction
					directions[xdx][zdz] = (i + dir / 2) % dir;
				}
				else if (openNodes[xdx][zdz] > z0->GetPriority())
				{
					// update the priority info
					openNodes[xdx][zdz] = z0->GetPriority();
					// update the parent direction info
					directions[xdx][zdz] = (i + dir / 2) % dir;

					// replace the node
					// by emptying one node to the other one
					// except the node to be replaced will be ignored
					// and the new node will be pushed in instead
					while (!(untriedNodes[nodeIndex].top().GetxPos() == xdx &&
						untriedNodes[nodeIndex].top().GetzPos() == zdz))
					{
						untriedNodes[1 - nodeIndex].push(untriedNodes[nodeIndex].top());
						untriedNodes[nodeIndex].pop();
					}
					untriedNodes[nodeIndex].pop(); // remove the wanted node

								   // empty the larger size node to the smaller one
					if (untriedNodes[nodeIndex].size() > untriedNodes[1 - nodeIndex].size()) nodeIndex = 1 - nodeIndex;
					while (!untriedNodes[nodeIndex].empty())
					{
						untriedNodes[1 - nodeIndex].push(untriedNodes[nodeIndex].top());
						untriedNodes[nodeIndex].pop();
					}
					nodeIndex = 1 - nodeIndex;
					untriedNodes[nodeIndex].push(*z0); // add the better node instead
				}
				else delete z0;
			}
		}
		delete x0;
	}
	return false; // no route found
}

void GroundMovement::MoveTo(double delta)
{
	if (GetParent()->GetPosition() == goal)
	{
		currentSpeed = 0.0f;
		goal = -goal;
	}
	else
	{
		if (currentSpeed < speed)
			currentSpeed += speed*delta / accelerationTime;
		dvec3 moveVec = destination - GetParent()->GetPosition();
		double distance = glm::length(moveVec);
		moveVec = glm::normalize(moveVec);
		moveVec = moveVec *delta * currentSpeed;
		if (distance > length(moveVec))
		{
			GetParent()->Move(moveVec);
		}
		else
		{
			GetParent()->SetPosition(destination);
		}
	}
}

void GroundMovement::TurnTo(double delta)
{
	if (GetParent()->GetPosition() != destination)
	{
		vec3 thisPos = GetParent()->GetPosition();
		vec2 targetVec = vec2(thisPos.x, thisPos.z) - vec2((float)destination.x, (float)destination.z);
		float distance = length(targetVec);
		if (targetVec.x != 0 && targetVec.y != 0)
			targetVec = glm::normalize(targetVec);

		//glm::dvec3 f = glm::rotate(GetParent()->GetRotation(), glm::dvec3(0, 0, -1));

		vec3 currentVec = vec3(-GetParent()->GetTransform()[2][0], 0, -GetParent()->GetTransform()[2][2]);
		if (currentVec.x != 0 && currentVec.z != 0)
			currentVec = normalize(currentVec);


		//currentVec = f;

		float angle = (glm::angle(normalize(vec2(currentVec.x, currentVec.z)), normalize(targetVec)) / pi<float>()*180.0f);

		vec3 distantPoint = thisPos + (distance * 2 * currentVec);

		float determinant = ((destination.x - thisPos.x)*(distantPoint.z - thisPos.z)) - ((destination.z - thisPos.z)*(distantPoint.x - thisPos.x));

		if (determinant != 0 && !glm::isnan(angle))
		{

			if (determinant < 0)
			{
				if ((turnSpeed*delta) < angle)
				{
					GetParent()->Rotate(vec3(0, turnSpeed*delta, 0));
				}
				else
				{
					GetParent()->Rotate(vec3(0, angle, 0));
				}
			}
			else
			{
				if ((turnSpeed*delta) < angle)
				{
					GetParent()->Rotate(vec3(0, -turnSpeed*delta, 0));
				}
				else
				{
					GetParent()->Rotate(vec3(0, -angle, 0));
				}
			}
		}
	}
}

void GroundMovement::Update(double delta)
{
	int xStart = floor(GetParent()->GetPosition().x + 0.5);//for grid of 1 spacing
	int zStart = floor(GetParent()->GetPosition().z + 0.5);

	int xFinish = floor(goal.x + 0.5);//for grid of 1 spacing
	int zFinish = floor(goal.z + 0.5);

	Pathfind(xStart, zStart, xFinish, zFinish);
	int j = directions[xStart][zStart];
	float xDestination;
	float zDestination;
	if ((xStart += dx[j]) == xFinish && (zStart += dz[j]) == zFinish)
	{
		float xDestination = (xStart + dx[j]);//translate to world space
		float zDestination = (zStart + dz[j]);
	}
	else
	{
		xDestination = goal.x;
		zDestination = goal.z;
	}

	destination = vec3(xDestination, 0.0, zDestination);
	MoveTo(delta);
	TurnTo(delta);
}
