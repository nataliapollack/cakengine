#include "MazeBuilder.h"
#include "Coordinator.hpp"
#include "Core.h" 
#include "ScreenManager.h"
#include <random>

extern Coordinator gCoordinator;


void MazeBuilder::BuildMaze()
{
	// Get random starting cell in maze
	std::random_device rd;
	std::mt19937 eng(rd());
	std::uniform_int_distribution<> distr(0, MazeSize - 1);

	int randomRow = distr(eng);
	int randomCol = distr(eng);

	MazeNode* InitialNode = &MazeNodes[randomRow][randomCol];


	InitialNode->Visited = true;
	VisitedNodes.push(InitialNode);

	while (!VisitedNodes.empty())
	{
		MazeNode* currNode = VisitedNodes.top();
		VisitedNodes.pop();

		std::vector<MazeNode*> neighbors = GetValidNeighbors(currNode);
		if (!neighbors.empty())
		{
			VisitedNodes.push(currNode);

			std::random_device rd;
			std::mt19937 eng(rd());
			std::uniform_int_distribution<> distr(0, neighbors.size() - 1);
			int randomIndex = distr(eng);

			//TODO: Place wall or whatever, on chosen node's location


			neighbors[randomIndex]->Visited = true;	
			VisitedNodes.push(neighbors[randomIndex]);

		}
		// else means we hit a dead end so maybe do something here if we want
	}

}

std::vector<MazeBuilder::MazeNode*> MazeBuilder::GetValidNeighbors(MazeNode* node)
{
	std::vector<MazeNode*> ValidNeighbors;
	int NeighborCol;
	int NeighborRow;

	// Check North
	NeighborRow = node->row + 1;
	NeighborCol = node->col;
	if (NeighborRow < MazeSize)
	{
		MazeNode* North = &MazeNodes[NeighborRow][NeighborCol];
		if (North->Visited == false)
			ValidNeighbors.push_back(North);
	}
	// Check East
	NeighborRow = node->row;
	NeighborCol = node->col + 1;
	if (NeighborCol < MazeSize)
	{
		MazeNode* East = &MazeNodes[NeighborRow][NeighborCol];
		if (East->Visited == false)
			ValidNeighbors.push_back(East);
	}
	// Check South
	NeighborRow = node->row - 1;
	NeighborCol = node->col;
	if (NeighborRow >= 0)
	{
		MazeNode* South = &MazeNodes[NeighborRow][NeighborCol];
		if (South->Visited == false)
			ValidNeighbors.push_back(South);
	}
	// Check West
	NeighborRow = node->row;
	NeighborCol = node->col - 1;
	if (NeighborCol >= 0)
	{
		MazeNode* West = &MazeNodes[NeighborRow][NeighborCol];
		if (West->Visited == false)
			ValidNeighbors.push_back(West);
	}
	return ValidNeighbors;
}

void MazeBuilder::PlaceWall(Vector2 pos)
{
	Entity en = gCoordinator.CreateEntity();
	gCoordinator.AddComponent(
		en,
		render_box{ Vector2{10.0f, 10.0f }, BLACK }
	);

	gCoordinator.AddComponent(
		en,
		transform2D{ pos }
	);

	gCoordinator.AddComponent(
		en,
		status{ true, true, MAZE, WALL }

	);


}
