#include "entity.h"

Entity::Entity() {
	mesh = nullptr;
	modelMatrix.SetIdentity();
}

Entity::Entity(Mesh* mesh_ptr, Matrix44 transform)
{
	mesh = mesh_ptr; 
	modelMatrix = transform;
}

void Entity::Render(Image* framebuffer, Camera* camera, const Color& c)
{
	bool negZ = false;
	std::vector<Vector3> vertices = mesh->GetVertices();

	for (int i=0; i<vertices.size(); i+=3)
	{
		Vector3 clipPos1 = camera->ProjectVector((modelMatrix * vertices[i]), negZ);
		Vector3 clipPos2 = camera->ProjectVector((modelMatrix * vertices[i + 1]), negZ);
		Vector3 clipPos3 = camera->ProjectVector((modelMatrix * vertices[i + 2]), negZ);


		if (abs(clipPos1.x) <= 1 && abs(clipPos1.y) <= 1 && abs(clipPos1.z) <= 1 &&
			abs(clipPos2.x) <= 1 && abs(clipPos2.y) <= 1 && abs(clipPos2.z) <= 1 &&
			abs(clipPos3.x) <= 1 && abs(clipPos3.y) <= 1 && abs(clipPos3.z) <= 1)
		{
			Vector2 screenPos1 = Vector2((clipPos1.x + 1) / 2 * framebuffer->width, (clipPos1.y + 1) / 2 * framebuffer->height);
			Vector2 screenPos2 = Vector2((clipPos2.x + 1) / 2 * framebuffer->width, (clipPos2.y + 1) / 2 * framebuffer->height);
			Vector2 screenPos3 = Vector2((clipPos3.x + 1) / 2 * framebuffer->width, (clipPos3.y + 1) / 2 * framebuffer->height);
				
			framebuffer->DrawTriangle(screenPos1, screenPos2, screenPos3, c, false, c);

		}
	}
}
