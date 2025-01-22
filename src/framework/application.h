/*
    + This class encapsulates the application, is in charge of creating the data, getting the user input, process the update and render.
*/

#pragma once

#include "main/includes.h"
#include "framework.h"
#include "image.h"
#include "button.h"

class Application
{
public:

    // Window

    SDL_Window* window = nullptr;
    int window_width;
    int window_height;

    float time;

    // Input
    const Uint8* keystate;
    int mouse_state; // Tells which buttons are pressed
    Vector2 mouse_position; // Last mouse position
    Vector2 mouse_delta; // Mouse movement in the last frame
    
    
    // Paint tool
     // doa struct later here
    
    bool process_activated;
    struct{
        int draw_line_state;
        std::vector<int> first_point;
    }LineState;
    
    struct{
        int rect_state;
        std::vector<int> first_point;
    }RectState;
    
    struct{
        int circ_state;
        std::vector<int> center;
    }CircState;
    
    void PaintTool( SDL_MouseButtonEvent event);
    
    // Buttons
    
    void InitButtons(void);
    Button pencil;
    Button eraser;
    Button line;
    Button rectangle;
    Button triangle;
    Button circle;
    Button animation;
    Button loadimage;
    Button saveimage;
    Button fill;
    Button newImage;
    Button black;
    Button white;
    Button red;
    Button blue;
    Button yellow;
    Button pink;
    Button green;

    // Particle System variable for testing
    ParticleSystem psys;    
    
    
    void OnKeyPressed(SDL_KeyboardEvent event);
    std::vector<int> OnMouseButtonDown(SDL_MouseButtonEvent event);
    void OnMouseButtonUp(SDL_MouseButtonEvent event);
    void OnMouseMove(SDL_MouseButtonEvent event);
    void OnWheel(SDL_MouseWheelEvent event);
    void OnFileChanged(const char* filename);

    // CPU Global framebuffer
    Image framebuffer;
    int border_size = 1;
    
    
    
    uint32_t last_click_time; // Store the time of the last click
    const uint32_t click_delay = 200; // Minimum delay between clicks in milliseconds
    
    
    // Constructor and main methods
    Application(const char* caption, int width, int height);
    ~Application();

    void Init( void );
    void ToolbarInit( void );
    void Render( void );
    void Update( float dt );

    // Other methods to control the app
    void SetWindowSize(int width, int height) {
        glViewport( 0,0, width, height );
        this->window_width = width;
        this->window_height = height;
        this->framebuffer.Resize(width, height);
    }

    Vector2 GetWindowSize()
    {
        int w,h;
        SDL_GetWindowSize(window,&w,&h);
        return Vector2(float(w), float(h));
    }
};
