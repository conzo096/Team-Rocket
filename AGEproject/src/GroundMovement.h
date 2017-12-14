#pragma once
#include <glm/gtx/vector_angle.hpp>
#include <deque>
#include "Movement.h"

using namespace glm;
using namespace std;

//const int xSize = 60; // horizontal size of the map
//const int zSize = 60; // vertical size size of the map
//static int nodeMap[xSize][zSize];
//static int closedNodes[xSize][zSize]; // map of closed (tried-out) nodes
//static int openNodes[xSize][zSize]; // map of open (not-yet-tried) nodes
//static int directions[xSize][zSize]; // map of directions
//const int dir = 8;
//static int dx[dir] = { 1, 1, 0, -1, -1, -1, 0, 1 }; //The x coordinate of the posible directions
//static int dz[dir] = { 0, 1, 1, 1, 0, -1, -1, -1 }; //The z coordinate of the posible directions

class Node
{
	// current position
	int xPos;
	int zPos;
	// total distance already travelled to reach the node
	int distanceTraveled;
	// priority=level+remaining distance estimate
	int priority;  // smaller: higher priority

public:
	Node(int xp, int zp, int d, int p)
	{
		xPos = xp; zPos = zp; distanceTraveled = d; priority = p;
	}

	int GetxPos() const { return xPos; }
	int GetzPos() const { return zPos; }
	int GetDistance() const { return distanceTraveled; }
	int GetPriority() const { return priority; }

	void UpdatePriority(const int & xDestination, const int & zDestination)
	{
		priority = distanceTraveled + Estimate(xDestination, zDestination) * 10; //A*
	}

	// give better priority to going straight instead of diagonally
	void NextDistance(const int & i) // i: direction
	{
		distanceTraveled += (i % 2 == 0 ? 10 : 14);
	}

	// Estimation function for the remaining distance to the goal.
	const int & Estimate(const int & xDest, const int & zDest) const
	{
		static int xd, zd, d;
		xd = xDest - xPos;
		zd = zDest - zPos;

		// Euclidian Distance
		d = static_cast<int>(sqrt(xd*xd + zd*zd));

		// Manhattan distance
		//d=abs(xd)+abs(yd);

		// Chebyshev distance
		//d=max(abs(xd), abs(yd));

		return(d);
	}
};

inline bool operator<(const Node & a, const Node & b)
{
	return a.GetPriority() > b.GetPriority();
}

class GroundMovement : public Movement
{
private:
	int xSize = 300; // horizontal size of the map
	int zSize = 300; // vertical size size of the map
	int **nodeMap;
	int **closedNodes; // map of closed (tried-out) nodes
	int **openNodes; // map of open (not-yet-tried) nodes
	int **directions; // map of directions
	int dir = 8;
	int dx[9] = { 1, 1, 0, -1, -1, -1, 0, 1, 0 }; //The x coordinate of the posible directions
	int dz[9] = { 0, 1, 1, 1, 0, -1, -1, -1, 0 }; //The z coordinate of the posible directions
	dvec3 **terrainGrid; //map of terrain plane coordinates
	deque<ivec2> waypoints; //The series of waypoins to be followed

protected:

	void from_json(const nlohmann::json &j);
public:
	GroundMovement();
	~GroundMovement();

	void SetGoal(glm::dvec3 goal) override;
	void SetGrid(int xSize, int zSize, int **nodeMap) { this->xSize = xSize; this->zSize = zSize, this->nodeMap = nodeMap; };
	bool LineOfSight();
	bool Pathfind(const int & xStart, const int & zStart, const int & xFinish, const int & zFinish);
	void MoveTo(double delta);
	void TurnTo(double delta);
	void Update(double delta) override;

};