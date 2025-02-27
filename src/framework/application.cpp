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

	this->lights = (sLight*)calloc(3, sizeof(sLight));
}

Application::~Application()
{
}

void Application::Init(void)
{
	// Initialize Quad for Lab4
	shader = Shader::Get("shaders/redtoblue.vs", "shaders/redtoblue.fs");
	texture = Texture::Get("images/fruits.png");
	mesh = new Mesh();
	mesh->CreateQuad();
	
	// Initialize flags
	current_scene = eScene::LAB4;
	current_task = eTask::FORMULAS;
	current_subtask = 1;
	active_lights = 1;

	// Initialize camera
	camera = Camera();
	camera.LookAt(Vector3(0, 1, 3), Vector3(0, 0, 0), Vector3(0, 1, 0));
	camera.SetPerspective(3.14 / 2, 1.6, 0.1f, 10.0f);

	// Initialize lights
	lights[0].intensity = Vector3(1.0, 1.0, 1.0); lights[0].position = Vector3(-1.0, 0.0, 0.0);
	lights[1].intensity = Vector3(1.0, 1.0, 1.0); lights[1].position = Vector3(1.0, 0.0, 0.0);
	lights[2].intensity = Vector3(1.0, 1.0, 1.0); lights[2].position = Vector3(0.0, 1.0, 0.0);

	// Initialize entity
	entity.mesh->LoadOBJ("../res/meshes/anna.obj");
	entity.modelMatrix.Set(1.0, 0.0, 0.0, 0.0,
							0.0, 1.0, 0.0, 0.0,
							0.0, 0.0, 1.0, 0.0,
							0.0, 0.0, 0.0, 1.0);
	
	Material anna_material;
	anna_material.Ka = Vector3(0.5, 0.5, 0.5);
	anna_material.Kd = Vector3(0.5, 0.5, 0.5);
	anna_material.Ks = Vector3(0.5, 0.5, 0.5);

	anna_material.shader = Shader::Get("shaders/simple.vs", "shaders/simple.fs");
	anna_material.texture = Texture::Get("textures/anna_specular.tga");
	anna_material.shininess = 0.5;

	entity.material = anna_material;

	// uniformData initialization
	uniformData.ambient_light = Vector3(1.0, 1.0, 1.0);
	uniformData.aspect_ratio = (float)window_width / (float)window_height;
	uniformData.light = lights[0];
	uniformData.model = entity.modelMatrix;
	uniformData.time = time;
	uniformData.view_projection_matrix = camera.GetViewProjectionMatrix();
}

// Render one frame
void Application::Render(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);// Clear the screen
	glEnable(GL_DEPTH_TEST);
	
	if (current_scene == eScene::LAB5)
	{
		this->uniformData.aspect_ratio = (float)window_width / (float)window_height;
		this->uniformData.time = time;

		this->uniformData.light = lights[0];

		glDepthFunc(GL_LEQUAL);
		glDisable(GL_BLEND);

		entity.Render(uniformData);

		for (int i = 1; i < active_lights; i++)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE);

			this->uniformData.light = lights[i];

			entity.Render(uniformData);
		}
	}
	else
	{
		shader->Enable();

		shader->SetUniform1("u_aspect_ratio", (float)window_width / (float)window_height);
		shader->SetUniform1("u_time", time);

		shader->SetTexture("u_texture", texture);

		mesh->Render();

		shader->Disable();
	}
}
// Called after render
void Application::Update(float seconds_elapsed)
{
	if (current_scene == eScene::LAB5)
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

		case SDLK_1:
			if (current_scene == eScene::LAB4) {
				current_task = eTask::FORMULAS;
				current_subtask = 1;

				shader = Shader::Get("shaders/formula.vs", "shaders/redtoblue.fs");
				texture = Texture::Get("images/fruits.png");
			}
			else active_lights = 1;

			break;

		case SDLK_2:
			if (current_scene == eScene::LAB4) {
				current_task = eTask::FILTERS;
				current_subtask = 1;

				shader = Shader::Get("shaders/filter.vs", "shaders/bnw.fs");
				texture = Texture::Get("images/fruits.png");
			}
			else active_lights = 2;

			break;

		case SDLK_3:

			if (current_scene == eScene::LAB4) {
				current_task = eTask::TRANSFORMATIONS;
				current_subtask = 1;

				shader = Shader::Get("shaders/filter.vs", "shaders/pixelate.fs");
				texture = Texture::Get("images/fruits.png");
			}
			else active_lights = 3;

			break;	

		case SDLK_l:
			if (current_scene == eScene::LAB4) {
				current_scene = eScene::LAB5;
			} else current_scene = eScene::LAB4;
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
			if (current_scene == eScene::LAB4) {
				if (current_task == eTask::FORMULAS) {
					shader = Shader::Get("shaders/formula.vs", "shaders/pointfade.fs");
				}
				else if (current_task == eTask::FILTERS) {
					shader = Shader::Get("shaders/filter.vs", "shaders/inverse.fs");
				}
				else if (current_task == eTask::TRANSFORMATIONS) {
					shader = Shader::Get("shaders/filter.vs", "shaders/rotate.fs");
				}
			}
			else {
				// Toggle between specular or not
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

		case SDLK_r:
			camera = Camera();
			camera.LookAt(Vector3(0, 1, 3), Vector3(0, 0, 0), Vector3(0, 1, 0));
			camera.SetPerspective(PI / 2, 1.6, 0.1f, 10.0f);  // Adjust near/far planes
			camera.UpdateViewMatrix();
			break;

		case SDLK_n:
			// Toggle between using normal or not
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
