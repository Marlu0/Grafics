#include "material.h"

void Material::Enable(const sUniformData& uniformData) {
	shader->Enable();

	// Upload material properties
	shader->SetVector3("u_material_Ka", this->Ka);
	shader->SetVector3("u_material_Kd", this->Kd);
	shader->SetVector3("u_material_Ks", this->Ks);
	
	// Upload 3DObject properties
	shader->SetMatrix44("u_model", uniformData.model);

	// Upload camera properties
	shader->SetMatrix44("u_view_projection", uniformData.view_projection_matrix);

	// Upload light properties
	shader->SetVector3("u_light_position", uniformData.lights)
		Upload camera properties
		Upload light properties
		Upload any other global properties
			Matrix44 model;
		Matrix44 view_projection_matrix;
		Vector3 ambient_light;
		sLight* lights;
}

void Material::Disable(const sUniformData& uniformData) {
	shader->Disable();
}