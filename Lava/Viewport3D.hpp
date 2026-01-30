#pragma once

#include "raylib.h"

struct viewport3D {
	// camera view
	Camera3D view;

	RenderTexture2D framebuffer;
};

// TODO: TEMP!
struct model_view {
	Model* model = nullptr;

	Vector3 offset = { 0 };
	float scale = 1.f;
	Color color = WHITE;
};
