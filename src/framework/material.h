#pragma once
#pragma once

#include "framework.h"
#include "shader.h"
#include "texture.h"
#include "camera.h"


typedef struct sLight {
    Vector3 position;
    Vector3 intensity;
} Light;

typedef struct sUniformData {
	Matrix44 model;
	//Matrix44 view_projection_matrix;
	Vector3 ambient_light;
	Light light;
	float aspect_ratio;
	float time;
    //Vector3 camera_position;
    Camera* camera;
    int first_light_rendered;
    int is_normal_activated;
	int is_colortext_activated;
	int is_specular_activated;
    
} sUniformData;


class Material
{
public:
	Shader* shader;
	Texture* texture;
    Texture* normals;
	Vector3 Ka;
	Vector3 Kd;
	Vector3 Ks;
	float shininess;
	
	void Enable(const sUniformData uniformData);
	void Disable(const sUniformData uniformData);
};
