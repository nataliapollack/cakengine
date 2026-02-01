#pragma once
#include <stack>
#include <vector>
#include "raylib.h"
#include "Events.hpp"


// Ok uhh, this will build the maze and place whatever items in it, 
// but won't really be doing much actively 

class MazeBuilder
{
public:
	// Just for some extra readability 
	enum WallDirs
	{
		NORTH = 0,
		EAST = 1,
		SOUTH = 2,
		WEST = 3,
		NONE = 5
	};

	void StartMaze();
	void UpdateMaze(float dt);
	//void NewDay(Event& event);
	bool GetIsActive();



	void PlaceWall(Vector2 pos);
	void DrawMaze();
	bool mazeBuilt = false;
	void BuildMaze();
	
	struct MazeNode
	{
		bool Visited = false;
		bool OnStack = false; // Just a debugging thing
		int row;
		int col;

		// If parent is 0,0 and this is 1,0
		// This variable would be EAST
		// 
		WallDirs ParentToThis = NONE;
		WallDirs ThisToParent = NONE; 

		// True if the wall still exists
		bool walls[4] = { true, true, true, true };
	};

private:

	// Ok just some stuff to remind me when I inevitably need to change this
	// Width == num of cols == X-axis! Height == num of Rows == Y-axis! 
	// (Stop confusing them aaaa!)
	// 
	// To get these vals I was just trial and error-ing it,
	// place a cell at 0.0, another at x * nodeSize, replace x till we find the corners
	// (I'm sure there's some funny math way to do it with screen size 
	// and desired number of nodes and node size yadda yadda, 
	// but i don't wanna think that much rn
	// 
	// Good numbies with nodeSize 50 are:
	// width = 18, height = 12, thickness = 10 
	static const int MazeWidth = 18;
	static const int MazeHeight = 12;

	

	MazeNode* MazeNodes[MazeWidth][MazeHeight];

	int nodeSize = 50;
	float wallOffset = nodeSize;

	// Just whatever looks good really
	int wallThickness = 10;

	std::stack<MazeNode*> VisitedNodes;


	bool isActive = false;
	bool StepByStepBuild = true;
	bool firstStep = true;

	std::vector<MazeNode*> GetValidNeighbors(MazeNode* node);


};

