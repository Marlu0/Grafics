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
	
	Vector2 triangle[3];
	int counter = 0;

	for (int i=0; i<vertices.size(); i++)
	{
		Vector3 clipPos = camera->ProjectVector((modelMatrix * vertices[i]), negZ);

		if (abs(clipPos.x) <= 1 && abs(clipPos.y) <= 1 && abs(clipPos.z) <= 1)
		{
			Vector2 screenPos = Vector2((clipPos.x + 1)/2 * framebuffer->width, (clipPos.y + 1)/2 * framebuffer->height);
			
			triangle[counter] = screenPos;
			counter++;
			
			if (counter == 3)
			{
				framebuffer->DrawTriangle(triangle[0], triangle[1], triangle[2], c, false, c);
				counter = 0;
			}
		}
	}
}
