#pragma once
#include "raylib.h"


class MazePlayer
{
public:
	void UpdatePlayer(float dt);
	void DrawPlayer();

	Vector2 pos = { 14.0f, 23.0f };

	Rectangle shape = { 0.0f, 0, 14, 14 };

	float speed = 150;
	Vector2 vel = { 0.0f, 0.0f };
	Vector2 defaultSpeed = { 50, 50 };

private:
};

