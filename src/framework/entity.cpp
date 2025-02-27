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
	material->Enable(uniformData);

	material->shader->SetMatrix44("u_model", modelMatrix);
	material->shader->SetMatrix44("u_viewprojection", uniformData.view_projection_matrix);
	material->shader->SetTexture("u_texture", material->texture);

	// Upload material properties
	material->shader->SetVector3("u_material_Ka", material->Ka);
	material->shader->SetVector3("u_material_Kd", material->Kd);
	material->shader->SetVector3("u_material_Ks", material->Ks);

	// Upload 3DObject properties
	material->shader->SetMatrix44("u_model", uniformData.model);

	// Upload camera properties
	material->shader->SetMatrix44("u_view_projection", uniformData.view_projection_matrix);

	// Upload any other global properties
	material->shader->SetFloat("u_aspect_ratio", uniformData.aspect_ratio);
	material->shader->SetFloat("u_time", uniformData.time);

	mesh->Render();

	material->Disable(uniformData);
}