#pragma once
#pragma once

#include "framework.h"
#include "shader.h"
#include "texture.h"

typedef struct sUniformData {
	Matrix44 model;
	Matrix44 view_projection_matrix;
	Vector3 ambient_light;
	Light light;
	float aspect_ratio;
	float time;
};

typedef struct sLight {
	Vector3 position;
	Vector3 intensity;
} Light;

class Material
{
public:
	Shader* shader;
	Texture* texture;
	Vector3 Ka;
	Vector3 Kd;
	Vector3 Ks;
	float shininess;
	
	void Material::Enable(const sUniformData& uniformData);
	void Material::Disable(const sUniformData& uniformData);
};
