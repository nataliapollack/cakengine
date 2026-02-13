#pragma once

#include <vector>

#include "raylib.h"

struct viewport3D {
	// camera positions / modes
	std::vector<Camera3D> views = {};

	RenderTexture2D framebuffer = {};

	int curView = 0;
};

// TODO: TEMP!
struct model_view {
	Model* model = nullptr;

	Vector3 offset = { 0 };
	float scale = 1.f;
	Color color = WHITE;
};
