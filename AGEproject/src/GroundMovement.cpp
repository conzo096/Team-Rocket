#include "GroundMovement.h"
#include <random>
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

bool GroundMovement::Pathfind(const int & xStart, const int & yStart, const int & xFinish, const int & yFinish)
{
	static priority_queue<node> pq[2]; // list of open (not-yet-tried) nodes
	static int pqi; // pq index
	static node* n0;
	static node* m0;
	static int i, j, x, y, xdx, ydy;
	static char c;
	pqi = 0;

	// reset the node maps
	for (y = 0; y<m; y++)
	{
		for (x = 0; x<n; x++)
		{
			closed_nodes_map[x][y] = 0;
			open_nodes_map[x][y] = 0;
		}
	}

	// create the start node and push into list of open nodes
	n0 = new node(xStart, yStart, 0, 0);
	n0->updatePriority(xFinish, yFinish);
	pq[pqi].push(*n0);
	open_nodes_map[x][y] = n0->getPriority(); // mark it on the open nodes map

											  // A* search
	while (!pq[pqi].empty())
	{
		// get the current node w/ the highest priority
		// from the list of open nodes
		n0 = new node(pq[pqi].top().getxPos(), pq[pqi].top().getyPos(),
			pq[pqi].top().getLevel(), pq[pqi].top().getPriority());

		x = n0->getxPos(); y = n0->getyPos();

		pq[pqi].pop(); // remove the node from the open list
		open_nodes_map[x][y] = 0;
		// mark it on the closed nodes map
		closed_nodes_map[x][y] = 1;

		// quit searching when the goal state is reached
		//if((*n0).estimate(xFinish, yFinish) == 0)
		if (x == xFinish && y == yFinish)
		{
			// generate the path from finish to start
			// by following the directions
			string path = "";
			while (!(x == xStart && y == yStart))
			{
				j = dir_map[x][y];
				c = '0' + (j + dir / 2) % dir;
				path = c + path;
				x += dx[j];
				y += dy[j];
			}

			// garbage collection
			delete n0;
			// empty the leftover nodes
			while (!pq[pqi].empty()) pq[pqi].pop();
			return true;
		}

		// generate moves (child nodes) in all possible directions
		for (i = 0; i<dir; i++)
		{
			xdx = x + dx[i]; ydy = y + dy[i];

			if (!(xdx<0 || xdx>n - 1 || ydy<0 || ydy>m - 1 || nodeMap[xdx][ydy] == 1
				|| closed_nodes_map[xdx][ydy] == 1))
			{
				// generate a child node
				m0 = new node(xdx, ydy, n0->getLevel(),
					n0->getPriority());
				m0->nextLevel(i);
				m0->updatePriority(xFinish, yFinish);

				// if it is not in the open list then add into that
				if (open_nodes_map[xdx][ydy] == 0)
				{
					open_nodes_map[xdx][ydy] = m0->getPriority();
					pq[pqi].push(*m0);
					// mark its parent node direction
					dir_map[xdx][ydy] = (i + dir / 2) % dir;
				}
				else if (open_nodes_map[xdx][ydy]>m0->getPriority())
				{
					// update the priority info
					open_nodes_map[xdx][ydy] = m0->getPriority();
					// update the parent direction info
					dir_map[xdx][ydy] = (i + dir / 2) % dir;

					// replace the node
					// by emptying one pq to the other one
					// except the node to be replaced will be ignored
					// and the new node will be pushed in instead
					while (!(pq[pqi].top().getxPos() == xdx &&
						pq[pqi].top().getyPos() == ydy))
					{
						pq[1 - pqi].push(pq[pqi].top());
						pq[pqi].pop();
					}
					pq[pqi].pop(); // remove the wanted node

								   // empty the larger size pq to the smaller one
					if (pq[pqi].size()>pq[1 - pqi].size()) pqi = 1 - pqi;
					while (!pq[pqi].empty())
					{
						pq[1 - pqi].push(pq[pqi].top());
						pq[pqi].pop();
					}
					pqi = 1 - pqi;
					pq[pqi].push(*m0); // add the better node instead
				}
				else delete m0; // garbage collection
			}
		}
		delete n0; // garbage collection
	}
	return false; // no route found
}

void GroundMovement::MoveTo(double delta)
{
	if (GetParent()->GetPosition() == destination)
	{

		destination.x = rand() % 200 - 100;
		destination.z = rand() % 200 - 100;

		currentSpeed = 0.0f;

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
	MoveTo(delta);
	TurnTo(delta);
}
