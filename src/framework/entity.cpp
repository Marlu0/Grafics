#include "entity.h"

Entity::Entity() {
	mesh = nullptr;
	modelMatrix.SetIdentity();
	mode = eRenderMode::POINTCLOUD;
}

Entity::Entity(Mesh* mesh_ptr, Matrix44 transform, eRenderMode emode)
{
	mesh = mesh_ptr; 
	modelMatrix = transform;
	mode = emode;
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
			Vector3 screenPos1 = Vector3((clipPos1.x + 1) / 2 * framebuffer->width, (clipPos1.y + 1) / 2 * framebuffer->height, clipPos1.z);
			Vector3 screenPos2 = Vector3((clipPos2.x + 1) / 2 * framebuffer->width, (clipPos2.y + 1) / 2 * framebuffer->height, clipPos2.z);
			Vector3 screenPos3 = Vector3((clipPos3.x + 1) / 2 * framebuffer->width, (clipPos3.y + 1) / 2 * framebuffer->height, clipPos3.z);
			
            TriangleInfo triangle = {screenPos1, screenPos2, screenPos3, Vector3(0,0,0), Vector3(0,0,0), Vector3(0,0,0), Color::RED, Color::BLUE, Color::GREEN};
            
            
			if (mode == eRenderMode::POINTCLOUD)
			{
				framebuffer->SetPixel(screenPos1.x, screenPos1.y, c);
				framebuffer->SetPixel(screenPos2.x, screenPos2.y, c);
				framebuffer->SetPixel(screenPos3.x, screenPos3.y, c);
			}
			else if (mode == eRenderMode::WIREFRAME)
			{
				framebuffer->DrawTriangle(screenPos1, screenPos2, screenPos3, c, false, c);
			}
			else if (mode == eRenderMode::TRIANGLES)
			{
				framebuffer->DrawTriangle(screenPos1, screenPos2, screenPos3, c, true, c);
			}
				

		}
	}
}
