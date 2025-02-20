/*  
	+ This class encapsulates the application, is in charge of creating the data, getting the user input, process the update and render.
*/

#pragma once

#include "main/includes.h"
#include "framework.h"
#include "image.h"
#include "entity.h"
#include "shader.h"


#define NUMENTITIES 4

enum class eTask {
	FORMULAS,
	FILTERS,
	TRANSFORMATIONS,
	RENDERMESH
};

class Application
{
public:
	
	Mesh* mesh;
	Shader* shader;
	Texture* texture;

	// Entity and camera
	Camera camera;
    Entity** entity;

	eTask current_task;
	int current_subtask;
    int trans_direction;

	bool occlusions;
	bool usemeshtext;
	bool interpolated;
	bool wireframe;

	// Window

	SDL_Window* window = nullptr;
	int window_width;
	int window_height;

	float time;

	// Input
	bool is_mouse_pressed_left;
    bool is_mouse_pressed_right;
	bool is_mouse_pressed_center;

	const Uint8* keystate;
	int mouse_state; // Tells which buttons are pressed
	Vector2 mouse_position; // Last mouse position
	Vector2 mouse_delta; // Mouse movement in the last frame

	void OnKeyPressed(SDL_KeyboardEvent event);
	void OnMouseButtonDown(SDL_MouseButtonEvent event);
	void OnMouseButtonUp(SDL_MouseButtonEvent event);
	void OnMouseMove(SDL_MouseButtonEvent event);
	void OnWheel(SDL_MouseWheelEvent event);
	void OnFileChanged(const char* filename);

	// Constructor and main methods
	Application(const char* caption, int width, int height);
	~Application();

	void Init( void );
	void Render( void );
	void Update( float dt );

	// Other methods to control the app
	void SetWindowSize(int width, int height) {
		glViewport( 0,0, width, height );
		this->window_width = width;
		this->window_height = height;
	
	}

	Vector2 GetWindowSize()
	{
		int w,h;
		SDL_GetWindowSize(window,&w,&h);
		return Vector2(float(w), float(h));
	}

	void moveHarmonic(Entity* entity, float time, float speed, Vector3 axis);
	void rotateEntity(Entity* entity, float seconds_elapsed, float speed, Vector3 axis);
	void scaleEntity(Entity* entity, float time, float speed, Vector3 axis);
};
