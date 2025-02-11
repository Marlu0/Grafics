#include "application.h"
#include "mesh.h"
#include "shader.h"
#include "utils.h" 

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

	this->is_mouse_pressed = false;
	this->framebuffer.Resize(w, h);
	this->entity = (Entity**)malloc(sizeof(Entity*) * 4);
}

Application::~Application()
{
}

void Application::Init(void)
{
	std::cout << "Initiating app..." << std::endl;

	Matrix44 M1;
	M1.Set(	2, 0, 1, 1,
			0, 2, 0, -0.5,
			0, 0, 2, 0,
			0, 0, 0, 1);

	Matrix44 M2;
	M2.Set(	2, 0, 0, 0,
			0, 2, 0, 0,
			0, 0, 2, 0,
			0, 0, 0, 1);

	Matrix44 M3;
	M3.Set( 2, 0, 0, -1,
			0, 2, 0, 0,
			0, 0, 2, 0,
			0, 0, 0, 1);

	Matrix44 M4;
	M4.Set( 4, 0, 0, 0.5,
			0, 4, 0, -0.5,
			0, 0, 4, 0,
			0, 0, 0, 1);

	Mesh* mesh1 = new Mesh();
	mesh1->LoadOBJ("../res/meshes/anna.obj");

	Mesh* mesh2 = new Mesh();
	mesh2->LoadOBJ("../res/meshes/cleo.obj");

	Mesh* mesh3 = new Mesh();
	mesh3->LoadOBJ("../res/meshes/lee.obj");

	Mesh* mesh4 = new Mesh();
	mesh4->LoadOBJ("../res/meshes/lee.obj");

	camera = Camera();
	camera.LookAt(Vector3(0, 0, 3), Vector3(0, 0, 0), Vector3(0, 1, 0));
	camera.SetPerspective(3.14 / 2, 1.6, 0.1f, 10.0f);  // Adjust near/far planes

    //camera.SetOrthographic(-1,1,1,-1,-1, 1);
	entity[0] = new Entity(mesh1, M1, eRenderMode::TRIANGLES);
	entity[1] = new Entity(mesh2, M2, eRenderMode::POINTCLOUD);
	entity[2] = new Entity(mesh3, M3, eRenderMode::POINTCLOUD);
	entity[3] = new Entity(mesh4, M4, eRenderMode::WIREFRAME);

	current_property = eProperty::FOV;
	current_scene = eScene::STATIC;
}

// Render one frame
void Application::Render(void)
{
	framebuffer.Fill(Color::BLACK);

	if (current_scene == eScene::ANIMATION) {
		entity[0]->Render(&framebuffer, &camera, Color::GREEN);
		entity[1]->Render(&framebuffer, &camera, Color::BLUE);
		entity[2]->Render(&framebuffer, &camera, Color::YELLOW);
	}
	
	else if (current_scene == eScene::STATIC) {
		entity[3]->Render(&framebuffer, &camera, Color::PURPLE);
	}

	framebuffer.Render();
}

// Called after render
void Application::Update(float seconds_elapsed)
{
	if (is_mouse_pressed) {

		float angle = PI / 16 * mouse_delta.x;

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
	if (current_scene == eScene::ANIMATION) {
		moveHarmonic(entity[0], time, 0.5, Vector3(0, 1, 0));
		rotateEntity(entity[1], seconds_elapsed, 1, Vector3(0.5, 0, 0));
		scaleEntity(entity[2], time, 0.5, Vector3(2, 2, 2));
	}
}
 
//keyboard press event 
void Application::OnKeyPressed(SDL_KeyboardEvent event)
{
	// KEY CODES: https://wiki.libsdl.org/SDL2/SDL_Keycode
	switch (event.keysym.sym) {
	case SDLK_ESCAPE: exit(0); break; // ESC key, kill the app

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

	}
}

void Application::OnMouseButtonDown(SDL_MouseButtonEvent event)
{
	if (event.button == SDL_BUTTON_LEFT) {
		is_mouse_pressed = true;
	}
}

void Application::OnMouseButtonUp( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_LEFT) {
		is_mouse_pressed = false;
	}
}

void Application::OnMouseMove(SDL_MouseButtonEvent event)
{
	
}

void Application::OnWheel(SDL_MouseWheelEvent event)
{
	Vector3 viewDir = camera.center - camera.eye;

	viewDir.Normalize();

	float zoomAmount = event.preciseY * 0.5f;

	camera.eye = camera.eye + viewDir * zoomAmount;

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
