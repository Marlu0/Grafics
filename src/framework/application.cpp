#include "application.h"
#include "mesh.h"
#include "shader.h"
#include "utils.h" 
#include "GL/glew.h"
#include <fstream>

Application::Application(const char* caption, int width, int height)
{
	this->window = createWindow(caption, width, height);

	int w,h;
	SDL_GetWindowSize(window,&w,&h);

	this->mouse_state = 0;
	this->time = 0.f;
	this->window_width = w;
	this->window_height = h;
	this->keystate = SDL_GetKeyboardState(nullptr);

	this->is_mouse_pressed_left = false;
    this->is_mouse_pressed_right = false;
	this->entity = (Entity**)malloc(sizeof(Entity*) * NUMENTITIES);
    this->zbuffer = FloatImage(width, height);
	this->current_property = eProperty::FOV;
	this->current_scene = eScene::STATIC;
	this->occlusions = true;
	this->usemeshtext = true;

	
}

Application::~Application()
{
}

void Application::Init(void)
{
	shader = new Shader();

	mesh = new Mesh();

	shader = Shader::Get("../res/shaders/quad.vs", "../res/shaders/quad.fs");

	mesh->CreateQuad();

}

// Render one frame
void Application::Render(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);// Clear the screen

	shader->Enable();
	mesh->Render();
	shader->Disable();
}
// Called after render
void Application::Update(float seconds_elapsed)
{
	
}

//keyboard press event 
void Application::OnKeyPressed(SDL_KeyboardEvent event)
{
	// KEY CODES: https://wiki.libsdl.org/SDL2/SDL_Keycode
	switch (event.keysym.sym) {
		case SDLK_ESCAPE: exit(0); break; // ESC key, kill the app

		case SDLK_r:
			camera = Camera();
			camera.LookAt(Vector3(0, 1, 3), Vector3(0, 0, 0), Vector3(0, 1, 0));
			camera.SetPerspective(PI / 2, 1.6, 0.1f, 10.0f);  // Adjust near/far planes
			camera.UpdateViewMatrix();
			break;

		case SDLK_1:
			current_scene = eScene::STATIC;
			break;

		case SDLK_2:
			current_scene = eScene::ANIMATION;
			break;

		case SDLK_MINUS:
		case SDLK_KP_MINUS:
			if (current_property == eProperty::NEAR_PLANE) {
				camera.near_plane = std::max(0.1f, static_cast<float>(camera.near_plane - 0.1));
				camera.UpdateProjectionMatrix();
			}
			else if (current_property == eProperty::FAR_PLANE) {
				camera.far_plane = std::max(static_cast<float>(camera.near_plane), static_cast<float>(camera.far_plane - 0.1));
				camera.UpdateProjectionMatrix();
			}
			else if (current_property == eProperty::FOV) {
				camera.fov = std::min(PI, camera.fov + (PI / 12));
				camera.UpdateProjectionMatrix();
			}
			break;

		case SDLK_PLUS:
		case SDLK_KP_PLUS:
			if (current_property == eProperty::NEAR_PLANE) {
				camera.near_plane = std::min(static_cast<float>(camera.far_plane), static_cast<float>(camera.near_plane + 0.1));
				camera.UpdateProjectionMatrix();
			}
			else if (current_property == eProperty::FAR_PLANE) {
				camera.far_plane = std::min(200.0f, static_cast<float>(camera.far_plane + 0.1));
				camera.UpdateProjectionMatrix();
			}
			else if (current_property == eProperty::FOV) {
				camera.fov = std::max(PI / 12, camera.fov - (PI / 12));
				camera.UpdateProjectionMatrix();
			}
			break;

		case SDLK_v:
			current_property = eProperty::FOV;
			break;

		case SDLK_n:
			current_property = eProperty::NEAR_PLANE;
			break;

		case SDLK_f:
			current_property = eProperty::FAR_PLANE;
			break;

		case SDLK_t:
			if (usemeshtext) usemeshtext = false;
			else usemeshtext = true;
			break;

		case SDLK_z:
			if (occlusions) occlusions = false;
			else occlusions = true;
			break;

		case SDLK_c:
			for (int i = 0; i < NUMENTITIES; i++) {
				if (entity[i]->mode == eRenderMode::TRIANGLES_INTERPOLATED) {
					entity[i]->mode = eRenderMode::TRIANGLES;
					interpolated = false;
				}
				else if (entity[i]->mode == eRenderMode::TRIANGLES) {
					entity[i]->mode = eRenderMode::TRIANGLES_INTERPOLATED;
					interpolated = true;
				}
			}
			break;
	}
}

void Application::OnMouseButtonDown(SDL_MouseButtonEvent event)
{
	if (event.button == SDL_BUTTON_LEFT) {
		is_mouse_pressed_left = true;
	} else if (event.button == SDL_BUTTON_RIGHT ){
		is_mouse_pressed_right = true;
	}
	else if (event.button == SDL_BUTTON_MIDDLE) {
		is_mouse_pressed_center = true;
	}
}

void Application::OnMouseButtonUp( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_LEFT) {
		is_mouse_pressed_left = false;
    } else if (event.button == SDL_BUTTON_RIGHT ){
		is_mouse_pressed_right = false;
    } else if (event.button == SDL_BUTTON_MIDDLE) {
		is_mouse_pressed_center = false;
	}
}

void Application::OnMouseMove(SDL_MouseButtonEvent event)
{
	
}

void Application::OnWheel(SDL_MouseWheelEvent event)
{
        Vector3 viewDir = camera.center - camera.eye;
        float currentDistance = viewDir.Length(); // Get current distance from eye to center

        viewDir.Normalize();
        
        float zoomAmount = event.preciseY * 0.1f;
        float minDistance = 0.2f;
                
        float newDistance = currentDistance - zoomAmount;

        
        if (newDistance < minDistance)
            newDistance = minDistance;

       
        camera.eye = camera.center - viewDir * newDistance;
        
        camera.UpdateViewMatrix();
}

void Application::OnFileChanged(const char* filename)
{ 
	Shader::ReloadSingleShader(filename);
}

void Application::moveHarmonic(Entity* entity, float time, float speed, Vector3 axis)
{
	entity->modelMatrix.Translate(axis.x * 0.02 * sin(speed * time * 2 * PI), axis.y * 0.02 * sin(speed * time * 2 * PI), axis.z * 0.02 * sin(speed * time * 2 * PI));
}

void Application::rotateEntity(Entity* entity, float seconds_elapsed, float speed, Vector3 axis) 
{
	entity->modelMatrix.RotateLocal(seconds_elapsed * speed, axis);
}

void Application::scaleEntity(Entity* entity, float time, float speed, Vector3 axis)
{
	entity->modelMatrix.M[0][0] = abs(axis.x*cos(time * speed));
	entity->modelMatrix.M[1][1] = abs(axis.y*cos(time * speed));
	entity->modelMatrix.M[2][2] = abs(axis.z*cos(time * speed));
}
