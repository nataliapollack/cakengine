#include "MazePlayer.h"
#include "Events.hpp"
#include "Coordinator.hpp"
#include "Core.h" 
#include "ScreenManager.h"
#include <algorithm>


extern Coordinator gCoordinator;
void MazePlayer::UpdatePlayer(float dt)
{
	bool keyPress = false;
	if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
	{
		keyPress = true;
		pos.x -= (speed * dt);
		vel.x -= (speed * dt);
	}
	if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
	{
		keyPress = true;
		pos.x += (speed * dt);
		vel.x += (speed * dt);
	}
	if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))
	{
		keyPress = true;
		pos.y += (speed * dt);
		vel.y += (speed * dt);
	}
	if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))
	{
		keyPress = true;
		pos.y -= (speed * dt);
		vel.y -= (speed * dt);
	}
	if (!keyPress)
	{
		vel.x = 0;
		vel.y = 0;
	}
	else
	{
		vel.x = std::clamp(vel.x, -speed, speed);
		vel.y = std::clamp(vel.y, -speed, speed);
	}
	

	//pos.x += vel.x;
	//pos.y += vel.y;
	


	shape.x = pos.x;
	shape.y = pos.y;

	if (vel.x < 0)
	{
		vel.x += dt;
	}
	else if (vel.x > 0)
	{
		vel.x -= dt;
	}

	if (vel.y < 0)
	{
		vel.y += dt;
	}
	else if (vel.y > 0)
	{
		vel.y -= dt;
	}


	if (IsKeyPressed(KEY_BACKSPACE))
	{
		Event screen(Events::Game::SCREEN_CHANGE);
		screen.SetParam(Events::Game::SCREEN_ID, OUTSIDE);

		gCoordinator.SendEvent(screen);
	}
}

void MazePlayer::DrawPlayer()
{
	DrawRectangleRec(shape, RED);
}