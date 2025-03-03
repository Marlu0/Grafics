#include "entity.h"

Entity::Entity() {
	mesh = nullptr;
	modelMatrix.SetIdentity();
}

Entity::Entity(Matrix44 model, Mesh* mesh_ptr, Material imaterial)
{
	mesh = mesh_ptr;
	modelMatrix = model;
	material = imaterial;
}

void Entity::Render(sUniformData uniformData)
{
	material.Enable(uniformData);

	material.shader->SetMatrix44("u_model", modelMatrix);
	material.shader->SetMatrix44("u_viewprojection", uniformData.camera->GetViewProjectionMatrix());
	material.shader->SetTexture("u_texture", material.texture);

	// Upload material properties
	material.shader->SetVector3("u_material_Ka", material.Ka);
	material.shader->SetVector3("u_material_Kd", material.Kd);
	material.shader->SetVector3("u_material_Ks", material.Ks);

	// Upload 3DObject properties
    material.shader->SetVector3("u_camera_position", uniformData.camera->eye);
	// Upload any other global properties
	material.shader->SetFloat("u_aspect_ratio", uniformData.aspect_ratio);
	material.shader->SetFloat("u_time", uniformData.time);
    
    material.shader->SetUniform1("u_is_first_entity_rendered", uniformData.first_light_rendered);

	mesh->Render();

	material.Disable(uniformData);
}
