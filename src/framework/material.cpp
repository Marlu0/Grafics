#include "material.h"

void Material::Enable(const sUniformData uniformData) {
	shader->Enable();

	// Upload light properties
	shader->SetVector3("u_light_position", uniformData.light.position);
	shader->SetVector3("u_light_intensity", uniformData.light.intensity);
	shader->SetFloat("u_shininess", shininess);
    shader->SetVector3("u_ambient_intensity", uniformData.ambient_light);
}

void Material::Disable(const sUniformData uniformData) {
	shader->Disable();
}
