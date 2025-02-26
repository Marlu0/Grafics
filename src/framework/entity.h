#pragma once

#include "framework.h"
#include "mesh.h"
#include "image.h"
#include "shader.h"
#include "material.h"

enum class eRenderMode {
	POINTCLOUD,
	WIREFRAME,
	TRIANGLES,
	TRIANGLES_INTERPOLATED
};

class Entity
{
public:
	Matrix44 modelMatrix;
	Mesh* mesh;
	Material* material;

	Entity(void);
	Entity(Matrix44 model, Mesh* mesh_ptr, Material* material_ptr);
	void Render(sUniformData uniformData);
};