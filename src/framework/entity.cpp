#include "entity.h"

Entity::Entity(void) {
	mesh->CreateCube(5.0);
	modelMatrix.SetIdentity();
}
Entity::Entity(const char* filename, Matrix44 newmodelMatrix) {
	mesh->LoadOBJ(filename);
	modelMatrix = newmodelMatrix;
}

void Entity::Render(Image* framebuffer, Camera* camera, const Color& c) {
	bool negZ = false;
	std::vector<Vector3> vertices = mesh->GetVertices();
	std::vector<Vector3> screenVertices;
	for (int i = 0; i < vertices.size(); ++i) {
		Vector3 vertex = vertices[i];
		Vector3 viewPos = camera->view_matrix * (modelMatrix * vertex);
		Vector3 clipPos = camera->ProjectVector(viewPos, negZ);
		Vector3 screenPos = { (framebuffer->width - 1) * 0.5 * (clipPos.x + 1), (framebuffer->height - 1) * 0.5 * (clipPos.y + 1), 0.5 * (clipPos.z) };
		screenVertices[i] = screenPos;
		if ((i + 1) % 3) {
			framebuffer->DrawTriangle({screenVertices[i].x, screenVertices[i].y }, { screenVertices[i-1].x, screenVertices[i-1].y }, { screenVertices[i-2].x, screenVertices[i-2].y }, c, false, c);
		}
	}
}