#include "entity.h"

Entity::Entity() {
	mesh = nullptr;
	modelMatrix.SetIdentity();
	material = nullptr;
}

Entity::Entity(Matrix44 model, Mesh* mesh_ptr, Material* material_ptr)
{
	mesh = mesh_ptr;
	modelMatrix = model;
	material = material_ptr;
}

void Entity::Render(sUniformData uniformData)
{
	material->shader->SetMatrix44("u_model", modelMatrix);
	material->shader->SetMatrix44("u_viewprojection", uniformData.view_projection_matrix);
}