#pragma once

#include "framework.h"
#include "mesh.h"
#include "image.h"

class Entity
{
public:
	Mesh* mesh;
	Matrix44 modelMatrix;

	Entity::Entity(void);
	Entity::Entity(Mesh* mesh_ptr, Matrix44 transform);
	void Entity::Render(Image* framebuffer, Camera* camera, const Color& c);
};
