#include "MazeBuilder.h"
#include "Coordinator.hpp"
#include "Core.h" 
#include "ScreenManager.h"
#include <random>

extern Coordinator gCoordinator;


void MazeBuilder::BuildMaze()
{
	// Get random starting cell in maze
	if (firstStep)
	{
		std::random_device rd;
		std::mt19937 eng(rd());
		std::uniform_int_distribution<> distr(0, MazeWidth-1);//MazeSize - 1);
		std::uniform_int_distribution<> distrRow(0, MazeHeight - 1);
		int randomCol = distr(eng);
		int randomRow = distrRow(eng);

		MazeNode* InitialNode = MazeNodes[randomCol][randomRow];


		InitialNode->Visited = true;
		VisitedNodes.push(InitialNode);
		firstStep = false;
	}


	while (!VisitedNodes.empty())
	{
		MazeNode* currNode = VisitedNodes.top();
		VisitedNodes.pop();

		currNode->OnStack = false;

		std::vector<MazeNode*> neighbors = GetValidNeighbors(currNode);
		if (!neighbors.empty())
		{
			VisitedNodes.push(currNode);
			currNode->OnStack = true;

			std::random_device rd;
			std::mt19937 eng(rd());
			std::uniform_int_distribution<> distr(0, neighbors.size() - 1);
			int randomIndex = distr(eng);

			// Deletes walls between currNode and chosen node
			MazeNode* chosenNode = neighbors[randomIndex];
			currNode->walls[chosenNode->ParentToThis] = false;
			chosenNode->walls[chosenNode->ThisToParent] = false;


			neighbors[randomIndex]->Visited = true;	
			VisitedNodes.push(neighbors[randomIndex]);
			neighbors[randomIndex]->OnStack = true;

		}
		// else means we hit a dead end so maybe do something here if we want

		if (StepByStepBuild)
		{
			return;
		}
	}
	mazeBuilt = true;

}

std::vector<MazeBuilder::MazeNode*> MazeBuilder::GetValidNeighbors(MazeNode* node)
{
	std::vector<MazeNode*> ValidNeighbors;
	int NeighborCol;
	int NeighborRow;

	// Check South
	NeighborRow = node->row + 1;
	NeighborCol = node->col;
	if (NeighborRow < MazeHeight)
	{
		MazeNode* South = MazeNodes[NeighborCol][NeighborRow];
		if (South->Visited == false)
		{
			ValidNeighbors.push_back(South);
			South->ParentToThis = SOUTH;
			South->ThisToParent = NORTH;
		}
	}
	// Check East
	NeighborRow = node->row;
	NeighborCol = node->col + 1;
	if (NeighborCol < MazeWidth)
	{
		MazeNode* East = MazeNodes[NeighborCol][NeighborRow];
		if (East->Visited == false)
		{
			ValidNeighbors.push_back(East);
			East->ParentToThis = EAST;
			East->ThisToParent = WEST;
		}
	}
	// Check North
	NeighborRow = node->row - 1;
	NeighborCol = node->col;
	if (NeighborRow >= 0)
	{
		MazeNode* North = MazeNodes[NeighborCol][NeighborRow];
		if (North->Visited == false)
		{
			ValidNeighbors.push_back(North);
			North->ParentToThis = NORTH;
			North->ThisToParent = SOUTH;
		}
	}
	// Check West
	NeighborRow = node->row;
	NeighborCol = node->col - 1;
	if (NeighborCol >= 0)
	{
		MazeNode* West = MazeNodes[NeighborCol][NeighborRow];
		if (West->Visited == false)
		{
			ValidNeighbors.push_back(West);
			West->ParentToThis = WEST;
			West->ThisToParent = EAST;
		}
	}
	return ValidNeighbors;
	//return std::vector<MazeNode*>();
}

void MazeBuilder::StartMaze()
{
	for (int i = 0; i < MazeWidth; ++i)
		for (int j = 0; j < MazeHeight; ++j)
		{
			// TODO: Remember to Delete this shit somewhere! 
			MazeNodes[i][j] = new MazeNode;
			MazeNodes[i][j]->col = i;
			MazeNodes[i][j]->row = j;
		}
	if(!StepByStepBuild)
		BuildMaze();
}

void MazeBuilder::UpdateMaze(float dt)
{

}

bool MazeBuilder::GetIsActive()
{
	return isActive;
}

void MazeBuilder::PlaceWall(Vector2 pos)
{

}

void MazeBuilder::DrawMaze()
{
	//DrawRectangle(0.0f, 0.0f, node.nodeSize, node.nodeSize, WHITE);

	//DrawRectangle(node.nodeSize * 11, 0.0f, node.nodeSize, node.nodeSize, WHITE);
	//DrawRectangle(0.0f, node.nodeSize * 7, node.nodeSize, node.nodeSize, WHITE);


	for (int col = 0; col < MazeWidth; ++col)
	{
		for (int row = 0; row < MazeHeight; ++row)
		{
			MazeNode* node = MazeNodes[col][row];
			if (!node->Visited)
				continue;
			// Draw nodes
			int xPos = col * nodeSize;
			int yPos = row * nodeSize;
			if(node->Visited)
				DrawRectangle(xPos, yPos, nodeSize, nodeSize, WHITE);
			if(node->OnStack)
				DrawRectangle(xPos, yPos, nodeSize, nodeSize, GREEN);

			// Draw walls 
			// North
			if(node->walls[NORTH])
				DrawRectangle(xPos, yPos, nodeSize, wallThickness, PURPLE);
			// East
			if (node->walls[EAST])
				DrawRectangle(xPos + wallOffset, yPos, wallThickness, nodeSize, PURPLE);
			// South
			if (node->walls[SOUTH])
				DrawRectangle(xPos, yPos + wallOffset, nodeSize, wallThickness, PURPLE);
			// West
			if (node->walls[WEST])
				DrawRectangle(xPos, yPos, wallThickness, nodeSize, PURPLE);
		}
	}
}
