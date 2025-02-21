#include "entity.h"

Entity::Entity() {
	mesh = nullptr;
	modelMatrix.SetIdentity();

}

Entity::Entity(Mesh* mesh_ptr, Matrix44 transform, Texture* tex, Shader* sha)
{
	mesh = mesh_ptr; 
	modelMatrix = transform;
    texture = tex;
    shader = sha;
}

void Entity::Render(Camera* camera)
{
   
    
}
