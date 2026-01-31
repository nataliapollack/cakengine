#pragma once
#include <stack>
#include <vector>
#include "raylib.h"
// Ok uhh, this will build the maze and place whatever items in it, 
// but won't really be doing much actively 



class MazeBuilder
{
public:
	struct MazeNode
	{
		bool Visited = false;
		int row;
		int col;
	};

private:
	static const int MazeSize = 40;

	MazeNode MazeNodes[MazeSize][MazeSize];

	std::stack<MazeNode*> VisitedNodes;

	void BuildMaze();

	std::vector<MazeNode*> GetValidNeighbors(MazeNode* node);

	void PlaceWall(Vector2 pos);

};

