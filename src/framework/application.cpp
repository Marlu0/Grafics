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

	this->entity = (Entity**)calloc(1, sizeof(Entity**));
	
}

Application::~Application()
{
}

void Application::Init(void)
{

	mesh = new Mesh();

	current_task = eTask::FORMULAS;
	current_subtask = 1;

	shader = Shader::Get("shaders/formula.vs", "shaders/redtoblue.fs");
	texture = Texture::Get("images/fruits.png");

	mesh->CreateQuad();

	Matrix44 M1;
	M1.Set(2, 0, 0, 0,
		0, 2, 0, 0,
		0, 0, 2, 0,
		0, 0, 0, 1);

	Mesh* mesh1 = new Mesh();
	mesh1->LoadOBJ("../res/meshes/anna.obj");

	camera = Camera();
	camera.LookAt(Vector3(0, 1, 3), Vector3(0, 0, 0), Vector3(0, 1, 0));
	camera.SetPerspective(3.14 / 2, 1.6, 0.1f, 10.0f);  // Adjust near/far planes

	Texture* T4 = Texture::Get("../res/textures/anna_color_specular.tga");

	Shader* entity_shader = Shader::Get("shaders/simple.vs", "shaders/simple.fs");
	entity[0] = new Entity(mesh1, M1, T4, entity_shader);

}

// Render one frame
void Application::Render(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);// Clear the screen
	glEnable(GL_DEPTH_TEST);

	shader->Enable();
    
	shader->SetFloat("u_aspect_ratio", (float)window_width/(float)window_height);
	shader->SetFloat("u_time", time);

	shader->SetTexture("u_texture", texture);

	if (current_task == eTask::RENDERMESH)
	{
		entity[0]->Render(uniformData);
		entity[0]->mesh->Render();
	}

	mesh->Render();

	shader->Disable();
}
// Called after render
void Application::Update(float seconds_elapsed)
{
	if (current_task == eTask::RENDERMESH)
	{
		if (is_mouse_pressed_left)
		{
			float angle = PI / 16 * mouse_delta.x;
			//float angle_x = PI / 16 * mouse_delta.y;

			Matrix44 first_translate;
			first_translate.Set(1, 0, 0, -camera.center.x,
				0, 1, 0, -camera.center.y,
				0, 0, 1, -camera.center.z,
				0, 0, 0, 1);

			Matrix44 rotate;
			rotate.Set(cos(angle), 0, sin(angle), 0,
				0, 1, 0, 0,
				-sin(angle), 0, cos(angle), 0,
				0, 0, 0, 1);

			Matrix44 translate;
			translate.Set(1, 0, 0, camera.center.x,
				0, 1, 0, camera.center.y,
				0, 0, 1, camera.center.z,
				0, 0, 0, 1);

			camera.eye = translate * rotate * first_translate * camera.eye;
			camera.UpdateViewMatrix();
		}
		else if (is_mouse_pressed_right)
		{
			camera.center.x -= 0.005 * mouse_delta.x;
			camera.center.y -= 0.005 * mouse_delta.y;
			camera.UpdateViewMatrix();
		}
		else if (is_mouse_pressed_center)
		{
			camera.eye.x -= 0.005 * mouse_delta.x;
			camera.eye.y -= 0.005 * mouse_delta.y;
			camera.UpdateViewMatrix();
		}
	}
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
			current_task = eTask::FORMULAS;
			current_subtask = 1;
			mesh->Clear();
			mesh->CreateQuad();
			shader = Shader::Get("shaders/formula.vs", "shaders/redtoblue.fs");
			texture = Texture::Get("images/fruits.png");
			break;

		case SDLK_2:
			current_task = eTask::FILTERS;
			current_subtask = 1;
			mesh->Clear();
			mesh->CreateQuad();
			shader = Shader::Get("shaders/filter.vs", "shaders/bnw.fs");
			texture = Texture::Get("images/fruits.png");
			break;

		case SDLK_3:
			current_task = eTask::TRANSFORMATIONS;
            current_subtask = 1;
			mesh->Clear();
			mesh->CreateQuad();
            shader = Shader::Get("shaders/filter.vs", "shaders/pixelate.fs");
			texture = Texture::Get("images/fruits.png");
			break;

		case SDLK_4:
			current_task = eTask::RENDERMESH;
			mesh->Clear();
			mesh->LoadOBJ("../res/meshes/anna.obj");
			shader = Shader::Get("shaders/simple.vs", "shaders/simple.fs");
			texture = Texture::Get("../res/textures/anna_color_specular.tga");
			break;	
	
		case SDLK_a:
			if (current_task == eTask::FORMULAS) {
				shader = Shader::Get("shaders/formula.vs", "shaders/redtoblue.fs");
			}
			else if (current_task == eTask::FILTERS) {
				shader = Shader::Get("shaders/filter.vs", "shaders/bnw.fs");
            }else if (current_task == eTask::TRANSFORMATIONS){
                shader = Shader::Get("shaders/filter.vs", "shaders/pixelate.fs");
            }
			break;

		case SDLK_s:
			if (current_task == eTask::FORMULAS) {
				shader = Shader::Get("shaders/formula.vs", "shaders/pointfade.fs");
			}
			else if (current_task == eTask::FILTERS) {
				shader = Shader::Get("shaders/filter.vs", "shaders/inverse.fs");
			}
            else if (current_task == eTask::TRANSFORMATIONS) {
                shader = Shader::Get("shaders/filter.vs", "shaders/rotate.fs");
            }
			break;

		case SDLK_d:
			if (current_task == eTask::FORMULAS) {
				shader = Shader::Get("shaders/formula.vs", "shaders/newyork.fs");
			}
			else if (current_task == eTask::FILTERS) {
				shader = Shader::Get("shaders/filter.vs", "shaders/breakingbad.fs");
			}
			break;

		case SDLK_f:
			if (current_task == eTask::FORMULAS) {
				shader = Shader::Get("shaders/formula.vs", "shaders/redgreengrad.fs");
			}
			else if (current_task == eTask::FILTERS) {
				shader = Shader::Get("shaders/filter.vs", "shaders/contrast.fs");
			}
			break;

		case SDLK_g:
			if (current_task == eTask::FORMULAS) {
				shader = Shader::Get("shaders/formula.vs", "shaders/chess.fs");
			}
			else if (current_task == eTask::FILTERS) {
				shader = Shader::Get("shaders/filter.vs", "shaders/vignette.fs");
			}
			break;

		case SDLK_h:
			if (current_task == eTask::FORMULAS) {
				shader = Shader::Get("shaders/formula.vs", "shaders/sinus.fs");
			}
			else if (current_task == eTask::FILTERS) {
				shader = Shader::Get("shaders/filter.vs", "shaders/blur.fs");
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
        
        float zoomAmount = event.preciseY * 0.05f;
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
