#include "material.h"

void Material::Enable(const sUniformData& uniformData) {
	shader->Enable();

	// Upload light properties
	shader->SetVector3("u_light_position", uniformData.light.position);
	shader->SetVector3("u_light_intensity", uniformData.light.intensity);
	shader->SetFloat("u_shininess", shininess);
}

void Material::Disable(const sUniformData& uniformData) {
	shader->Disable();
}