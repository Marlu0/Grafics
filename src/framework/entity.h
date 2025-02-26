#pragma once

#include "framework.h"
#include "mesh.h"
#include "image.h"
#include "shader.h"

enum class eRenderMode {
	POINTCLOUD,
	WIREFRAME,
	TRIANGLES,
	TRIANGLES_INTERPOLATED
};

class Entity
{
public:
	Mesh* mesh;
	Matrix44 modelMatrix;
	Texture* texture;
	Shader* shader;

	Entity(void);
	Entity(Mesh* mesh_ptr, Matrix44 transform, Texture* tex, Shader* sha);
	void Render(Camera* camera, Shader* shader);
};