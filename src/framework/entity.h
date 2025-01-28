#pragma once

#include "framework.h"
#include "mesh.h"
#include "image.h"

class Entity
{
public:
	Mesh *mesh;
	Matrix44 modelMatrix;

	Entity::Entity(void);
	Entity::Entity(const char* filename, Matrix44 modelMatrix);
	void Entity::Render(Image* framebuffer, Camera* camera, const Color& c);
};
