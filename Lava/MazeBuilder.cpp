#include "MazeBuilder.h"
#include "Coordinator.hpp"
#include "Core.h" 
#include "ScreenManager.h"
#include <random>
#include "Player.h"

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
			currNode->RemainingWalls[chosenNode->ParentToThis] = false;
			chosenNode->RemainingWalls[chosenNode->ThisToParent] = false;


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
	PlaceWall();
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
}

void MazeBuilder::HitWall(Rectangle wall)
{
	float collision_forgiveness = 0.4f;
	float x_correction = 0.0f;

	if (wall.height <= wall.width)
	{
		if (player.vel.y > 0 && wall.y > player.pos.y) // floor
		{
			player.pos.y -= wall.height/5;
			player.vel.y = 0.0f;
		}
		else if (player.vel.y < 0) // roof
		{
			// collision forgiveness 
			if (wall.width < player.shape.width * collision_forgiveness)
			{
				if (wall.x > player.pos.x)
					x_correction = -wall.width/5;
				else
					x_correction = wall.width/5;
			}
			else // or not
			{
				player.pos.y += wall.height/5;
				player.vel.y = 0.0f;
			}
		}
	}
	else
	{
		if (player.vel.x > 0)
		{
			player.pos.x -= wall.width/5;
			player.vel.x = 0.0f;
		}
		else if (player.vel.x < 0)
		{
			player.pos.x += wall.width/5;
			player.vel.x = 0.0f;
		}
	}
}
void MazeBuilder::Init()
{
	isActive = true;
	player.pos = player.defaultPos;
	for (int i = 0; i < MazeWidth; ++i)
		for (int j = 0; j < MazeHeight; ++j)
		{
			MazeNodes[i][j] = new MazeNode;
			MazeNodes[i][j]->col = i;
			MazeNodes[i][j]->row = j;
		}
	if(!StepByStepBuild)
		BuildMaze();

}

void MazeBuilder::UpdateMaze(float dt)
{
	if (!isActive)
		return;

	
	player.UpdatePlayer(dt);

	for (int i = 0; i < Walls.size(); ++i)
	{
		if (CheckCollisionRecs(player.shape, Walls[i]))
		{
			HitWall(Walls[i]);
		}
	}

	for (int col = 0; col < MazeWidth; ++col)
	{
		for (int row = 0; row < MazeHeight; ++row)
		{
			MazeNode* node = MazeNodes[col][row];
			int xPos = col * nodeSize;
			int yPos = row * nodeSize;
			Rectangle rec = { xPos, yPos, nodeSize, nodeSize };
			if (CheckCollisionRecs(player.shape, rec))
			{
				if (col == MazeWidth - 1 && row == MazeHeight - 1)
				{
					DrawRectangle(xPos, yPos, nodeSize, nodeSize, GREEN);
					EndMaze();
					Event screen(Events::Game::SCREEN_CHANGE);
					screen.SetParam(Events::Game::SCREEN_ID, OUTSIDE);

					gCoordinator.SendEvent(screen);
				}

				node->IsFoggy = false;
			}
		}
	}
}

bool MazeBuilder::GetIsActive()
{
	return isActive;
}

void MazeBuilder::EndMaze()
{
	for (int i = 0; i < MazeWidth; ++i)
	{
		for(int j = 0; j < MazeHeight; ++j)
			delete MazeNodes[i][j];
	}
	isActive = false;
}

void MazeBuilder::StartMaze(Event& event)
{
	isActive = true;
	firstStep = true;


	Event screen(Events::Game::SCREEN_CHANGE);
	screen.SetParam(Events::Game::SCREEN_ID, MAZE_SCREEN);

	gCoordinator.SendEvent(screen);
}

void MazeBuilder::StartDay(Event& event)
{
	EndMaze();
	Init();
}

void MazeBuilder::PlaceWall()
{
	for (int col = 0; col < MazeWidth; ++col)
	{
		for (int row = 0; row < MazeHeight; ++row)
		{
			MazeNode* node = MazeNodes[col][row];
			int xPos = col * nodeSize;
			int yPos = row * nodeSize;
			if (node->RemainingWalls[NORTH])
				Walls.push_back(Rectangle(xPos, yPos, nodeSize, wallThickness));
			if (node->RemainingWalls[EAST])
				Walls.push_back(Rectangle(xPos + wallOffset, yPos, wallThickness, nodeSize));
			if (node->RemainingWalls[SOUTH])
				Walls.push_back(Rectangle(xPos, yPos + wallOffset, nodeSize, wallThickness));
			if (node->RemainingWalls[WEST])
				Walls.push_back(Rectangle(xPos, yPos, wallThickness, nodeSize));
		}
	}
}

void MazeBuilder::DrawMaze()
{
	if(!isActive)
		return;

	for (int col = 0; col < MazeWidth; ++col)
	{
		for (int row = 0; row < MazeHeight; ++row)
		{
			MazeNode* node = MazeNodes[col][row];
			int xPos = col * nodeSize;
			int yPos = row * nodeSize;

			// goal
			if (col == MazeWidth - 1 && row == MazeHeight - 1)
			{
				DrawRectangle(xPos, yPos, nodeSize, nodeSize, RED);
			}

			// visited is for maze building/step by step drawing
			// IsFoggy is for fog o war during game 
			if (!node->Visited || node->IsFoggy)
				continue;


			// Draw nodes
			if(node->Visited)
				DrawRectangle(xPos, yPos, nodeSize, nodeSize, WHITE);
			if(node->OnStack)
				DrawRectangle(xPos, yPos, nodeSize, nodeSize, GREEN);
			// Draw walls 
			// North
			if(node->RemainingWalls[NORTH])
				DrawRectangle(xPos, yPos, nodeSize, wallThickness, PURPLE);
			// East
			if (node->RemainingWalls[EAST])
				DrawRectangle(xPos + wallOffset, yPos, wallThickness, nodeSize, PURPLE);
			// South
			if (node->RemainingWalls[SOUTH])
				DrawRectangle(xPos, yPos + wallOffset, nodeSize, wallThickness, PURPLE);
			// West
			if (node->RemainingWalls[WEST])
				DrawRectangle(xPos, yPos, wallThickness, nodeSize, PURPLE);
		}
	}
	player.DrawPlayer();
}
