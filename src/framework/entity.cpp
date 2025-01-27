#include "entity.h"

Entity::Entity(void) {
	Mesh mesh;
	modelMatrix = Matrix44();
}

void Entity::Render(Image* framebuffer, Camera* camera, const Color& c) {
	std::vector<Vector3> vertices = mesh.GetVertices();
	for (const Vector3& vertex : vertices) {
		Vector3 worldPos = modelMatrix * vertex;
		camera->ProjectVector(worldPos, );
	}
}