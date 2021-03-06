#pragma once
#include "Graphsicks\Renderer.h"
#include "VString.h"

template <typename T, typename ...Args>
void RenderQuad(Renderer& renderer, const b2Vec2& pos, const b2Vec2& size, 
	bool flip_sprite_on_x = false, bool flip_sprite_on_y = false, Args&&... args)
{
	const float
		min_u = flip_sprite_on_x ? 1.0f : 0.0f,
		min_v = flip_sprite_on_y ? 1.0f : 0.0f,
		max_u = flip_sprite_on_x ? 0.0f : 1.0f,
		max_v = flip_sprite_on_y ? 0.0f : 1.0f;

	float
		min_x = pos.x,
		min_y = pos.y,
		max_x = pos.x + size.x,
		max_y = pos.y + size.y;

	// first triangle   x      y      u      v
	typename T::PrimData vertices[3];
	vertices[0] = { min_x, min_y, min_u, min_v };
	vertices[1] = { max_x, min_y, max_u, min_v };
	vertices[2] = { min_x, max_y, min_u, max_v };

	T shader = T(
		vertices[0], vertices[1], vertices[2], std::forward<Args>(args)...);

	renderer.AddDrawCommand(shader);

	// second triangle
	vertices[0] = { max_x, min_y, max_u, min_v };
	vertices[1] = { max_x, max_y, max_u, max_v };
	vertices[2] = { min_x, max_y, min_u, max_v };

	shader = T(
		vertices[0], vertices[1], vertices[2], std::forward<Args>(args)...);

	renderer.AddDrawCommand(shader);
}

void RenderText(Renderer& renderer, StringRef text, Font& font, 
	int point_size, const b2Vec2& pos, const Color& color = Colors::White);