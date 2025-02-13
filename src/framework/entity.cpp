#include "entity.h"

Entity::Entity() {
	mesh = nullptr;
	modelMatrix.SetIdentity();
	mode = eRenderMode::POINTCLOUD;
}

Entity::Entity(Mesh* mesh_ptr, Matrix44 transform, eRenderMode emode, Image* tex)
{
	mesh = mesh_ptr; 
	modelMatrix = transform;
	mode = emode;
    texture = tex;
}

void Entity::Render(Image* framebuffer, FloatImage* zbuffer, Camera* camera, const Color& c, bool occlusions, bool usemeshtext, bool interpolated)
{
	bool negZ = false;
	std::vector<Vector3> vertices = mesh->GetVertices();
    std::vector<Vector2> uvs = mesh->GetUVs();
	for (int i=0; i<vertices.size(); i+=3)
	{
		Vector3 clipPos1 = camera->ProjectVector((modelMatrix * vertices[i]), negZ);
		Vector3 clipPos2 = camera->ProjectVector((modelMatrix * vertices[i + 1]), negZ);
		Vector3 clipPos3 = camera->ProjectVector((modelMatrix * vertices[i + 2]), negZ);
        Vector2 uv1 = uvs[i];
        Vector2 uv2 = uvs[i + 1];
        Vector2 uv3 = uvs[i + 2];

		if (abs(clipPos1.x) <= 1 && abs(clipPos1.y) <= 1 && abs(clipPos1.z) <= 1 &&
			abs(clipPos2.x) <= 1 && abs(clipPos2.y) <= 1 && abs(clipPos2.z) <= 1 &&
			abs(clipPos3.x) <= 1 && abs(clipPos3.y) <= 1 && abs(clipPos3.z) <= 1)
		{
			Vector3 screenPos1 = Vector3((clipPos1.x + 1) / 2 * framebuffer->width, (clipPos1.y + 1) / 2 * framebuffer->height, (clipPos1.z + 1)/2);
			Vector3 screenPos2 = Vector3((clipPos2.x + 1) / 2 * framebuffer->width, (clipPos2.y + 1) / 2 * framebuffer->height,(clipPos2.z + 1)/2 );
			Vector3 screenPos3 = Vector3((clipPos3.x + 1) / 2 * framebuffer->width, (clipPos3.y + 1) / 2 * framebuffer->height, (clipPos3.z + 1)/2);
			
            TriangleInfo triangle = {screenPos1, screenPos2, screenPos3, uv1, uv2, uv3, Color::RED, Color::BLUE, Color::GREEN};
            
			if (mode == eRenderMode::POINTCLOUD)
			{
                if(zbuffer->GetPixel(screenPos1.x, screenPos1.y) > screenPos1.z){
                    framebuffer->SetPixel(screenPos1.x, screenPos1.y, c);
                    zbuffer->SetPixel(screenPos1.x, screenPos1.y, screenPos1.z);
        
                }
                if(zbuffer->GetPixel(screenPos2.x, screenPos2.y) > screenPos2.z){
                    framebuffer->SetPixel(screenPos2.x, screenPos2.y, c);
                    zbuffer->SetPixel(screenPos2.x, screenPos2.y, screenPos2.z);
        
                }
                if(zbuffer->GetPixel(screenPos3.x, screenPos3.y) > screenPos3.z){
                    framebuffer->SetPixel(screenPos3.x, screenPos3.y, c);
                    zbuffer->SetPixel(screenPos3.x, screenPos3.y, screenPos3.z);
        
                }
            
			}
			else if (mode == eRenderMode::WIREFRAME)
			{
				framebuffer->DrawTriangle(triangle, false, c, zbuffer, occlusions);
			}
			else if (mode == eRenderMode::TRIANGLES)
			{
				framebuffer->DrawTriangle(triangle, true, c, zbuffer, occlusions);
			}
			else if (mode == eRenderMode::TRIANGLES_INTERPOLATED)
			{
				if (!usemeshtext) framebuffer->DrawTriangleInterpolated(triangle, zbuffer, nullptr, occlusions);
				else if (usemeshtext) framebuffer->DrawTriangleInterpolated(triangle, zbuffer, texture, occlusions);
				
			}

		}
	}
}
