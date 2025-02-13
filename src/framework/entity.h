#pragma once

#include "framework.h"
#include "mesh.h"
#include "image.h"

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
	eRenderMode mode;
    Image* texture;

	Entity(void);
	Entity(Mesh* mesh_ptr, Matrix44 transform, eRenderMode emode, Image* tex);
	void Render(Image* framebuffer, FloatImage* zbuffer, Camera* camera, const Color& c, bool occlusion);
};
