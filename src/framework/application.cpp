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

	this->framebuffer.Resize(w, h);
	this->entity = (Entity**)malloc(sizeof(Entity*) * 3);
}

Application::~Application()
{
}

void Application::Init(void)
{
	std::cout << "Initiating app..." << std::endl;

	Matrix44 M1;
	M1.Set(	2, 0, 1, 0.5,
			0, 3, 0, -0.75,
			0, 0, 2, 0,
			0, 0, 0, 1);

	Matrix44 M2;
	M2.Set(	3, 0, 0, -0.5,
			0, 3, -2, -0.75,
			0, 0, 3, 0,
			0, 0, 0, 1);

	Matrix44 M3;
	M3.Set(2, 0, 0, 0,
			0, 2, 0, 0,
			0, 0, 2, 0,
			0, 0, 0, 1);

    M1.Translate(0, -1, -1);
	Mesh* mesh1 = new Mesh();
	mesh1->LoadOBJ("../res/meshes/anna.obj");

	Mesh* mesh2 = new Mesh();
	mesh2->LoadOBJ("../res/meshes/cleo.obj");

	Mesh* mesh3 = new Mesh();
	mesh3->LoadOBJ("../res/meshes/lee.obj");

	camera = Camera();
    camera.LookAt(Vector3(0, 0, 1), Vector3(0, 0, -1), Vector3(0, 1, 0));
    camera.SetPerspective(3.14 / 2, 1.6, 0.1, 100);  // Adjust near/far planes
    //camera.SetOrthographic(-1,1,1,-1,-1, 1);
	entity[0] = new Entity(mesh1, M1);
	entity[1] = new Entity(mesh2, M2);
	entity[2] = new Entity(mesh3, M3);
}

// Render one frame
void Application::Render(void)
{
	// ...
	framebuffer.Fill(Color::BLACK);
	entity[0]->Render(&framebuffer, &camera, Color::RED);
	entity[1]->Render(&framebuffer, &camera, Color::GREEN);
	entity[2]->Render(&framebuffer, &camera, Color::YELLOW);
	framebuffer.Render();
}

// Called after render
void Application::Update(float seconds_elapsed)
{

}

//keyboard press event 
void Application::OnKeyPressed( SDL_KeyboardEvent event )
{
	// KEY CODES: https://wiki.libsdl.org/SDL2/SDL_Keycode
	switch(event.keysym.sym) {
		case SDLK_ESCAPE: exit(0); break; // ESC key, kill the app
	}
}

void Application::OnMouseButtonDown( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_LEFT) {

	}
}

void Application::OnMouseButtonUp( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_LEFT) {

	}
}

void Application::OnMouseMove(SDL_MouseButtonEvent event)
{
	
}

void Application::OnWheel(SDL_MouseWheelEvent event)
{
	float dy = event.preciseY;

	// ...
}

void Application::OnFileChanged(const char* filename)
{ 
	Shader::ReloadSingleShader(filename);
}
