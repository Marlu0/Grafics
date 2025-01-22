#include "application.h"
#include "mesh.h"
#include "shader.h"
#include "utils.h"
#include <vector>
#include <stack>

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
    
    this->LineState.draw_line_state = 0;
    this->RectState.rect_state = 0;
    this->CircState.circ_state = 0;
    this->process_activated = false;
}

Application::~Application()
{
}

void Application::ToolbarInit(void){
    framebuffer.DrawRect(0, 0, window_width, 32, Color::GRAY, 1, false, Color::GRAY);
    framebuffer.DrawImage("../res/images/pencil.png",false, 0, 0);
    framebuffer.DrawImage("../res/images/eraser.png",false, 32, 0);
    framebuffer.DrawImage("../res/images/line.png",false, 64, 0);
    framebuffer.DrawImage("../res/images/rectangle.png",false, 96, 0);
    framebuffer.DrawImage("../res/images/triangle.png",false, 128, 0);
    framebuffer.DrawImage("../res/images/circle.png",false, 160, 0);
}

void Application::Init(void)
{
    std::cout << "Initiating app..." << std::endl;
    ToolbarInit();
}

// Render one frame
void Application::Render(void)
{
    framebuffer.Render();
}

// Called after render
void Application::Update(float seconds_elapsed)
{

}

//keyboard press event
void Application::OnKeyPressed(SDL_KeyboardEvent event) {
    // KEY CODES: https://wiki.libsdl.org/SDL2/SDL_Keycode
    switch (event.keysym.sym) {
    case SDLK_ESCAPE:
        exit(0); // ESC key, kill the app
        break;

    case SDLK_1:
        // Action: Draw Lines
       
        break;

    case SDLK_2:
        // Action: Draw Rectangles
        break;

    case SDLK_3:
        // Action: Draw Circles
        break;

    case SDLK_4:
        // Action: Draw Triangles
        break;

    case SDLK_5:
        // Action: Paint
        break;

    case SDLK_6:
        // Action: Animation
        break;

    case SDLK_f:
        // Action: Fill Shapes
        break;

    case SDLK_KP_PLUS:
    case SDLK_PLUS:
        if (border_size < 10) { // Limit of change of border width
            border_size++;
        }
        break;

    case SDLK_KP_MINUS:
    case SDLK_MINUS:
        border_size = std::max(1, border_size - 1);
        break;

    default:
        // No action for this key
        break;
    }
}

void Application::PaintTool(SDL_MouseButtonEvent event){
    std::vector <int> click = OnMouseButtonDown(event);
    
    
    ///////////LINE////////////////////
    if (click[0] < 96 && click[0] > 64 && click[1] > ( window_height - 32) && LineState.draw_line_state == 0 && process_activated == false){
        LineState.draw_line_state = 1;
        process_activated = true;
    }else{
        
        if(LineState.draw_line_state == 1){
            LineState.first_point = click;
            
            LineState.draw_line_state = 2;
            return;
            
        }else if(LineState.draw_line_state == 2){
            framebuffer.DrawLineDDA(LineState.first_point[0], window_height - LineState.first_point[1], click[0], window_height - click[1], Color::RED);
            LineState.draw_line_state = 0;
            process_activated = false;
            std::cout << "process end";
            return;
        }
        
        
    }
    
    ///////////////////////////////////
    ///
    /////////////RECTANGLE/////////////
    if (click[0] > 96 && click[0] < 128 && click[1] > ( window_height - 32) && RectState.rect_state == 0 && process_activated == false){
        RectState.rect_state = 1;
        process_activated = true;
        std::cout << "Hola";
    }else{
        
        if(RectState.rect_state == 1){
            RectState.first_point = click;
            
            RectState.rect_state = 2;
            return;
            
        }else if(RectState.rect_state == 2){
            
            if(RectState.first_point[0] >= click[0] && (window_height - RectState.first_point[1]) <= (window_height - click[1])){ //second quadrant
                
                framebuffer.DrawRect(click[0], window_height -  RectState.first_point[1], RectState.first_point[0] - click[0], RectState.first_point[1] - click[1], Color::RED, 1, false, Color::RED);
                
                
                RectState.rect_state = 0;
                process_activated = false;
                return;
                
            }else if( RectState.first_point[0] >= click[0] && (window_height - RectState.first_point[1]) >= (window_height - click[1])){ // third quadrant
                
                framebuffer.DrawRect(click[0], window_height - click[1], RectState.first_point[0] - click[0], click[1] - RectState.first_point[1], Color::RED, 1, false, Color::RED);
                
                
                RectState.rect_state = 0;
                process_activated = false;
                return;
                
            }else if( RectState.first_point[0] <= click[0] && (window_height - RectState.first_point[1]) >= (window_height - click[1])){ // fourth quadrant
                
                framebuffer.DrawRect(RectState.first_point[0], window_height - click[1], click[0] - RectState.first_point[0], click[1] - RectState.first_point[1], Color::RED, 1, false, Color::RED);
                
                
                RectState.rect_state = 0;
                process_activated = false;
                return;
                
            }else{
                framebuffer.DrawRect(RectState.first_point[0], window_height - RectState.first_point[1], click[0] - RectState.first_point[0], RectState.first_point[1] - click[1], Color::RED, 1, false, Color::RED);
                
                
                RectState.rect_state = 0;
                process_activated = false;
                return;
            }
            
            return;
        }
    
        
    }
    
    ////////////////////////////////////
    ///
    //////////////Circle////////////////
    if (click[0] < 192 && click[0] > 160 && click[1] > ( window_height - 32) && CircState.circ_state == 0 && process_activated == false){
        CircState.circ_state = 1;
        process_activated = true;
    }else{
        
        if(CircState.circ_state == 1){
            CircState.center = click;
            
            CircState.circ_state = 2;
            return;
            
        }else if(CircState.circ_state == 2){
            
            int radius = sqrt(pow(abs(CircState.center[0] - click[0]), 2) +  pow(abs(CircState.center[1] - click[1]), 2));
            
            framebuffer.DrawCircle(CircState.center[0], window_height - CircState.center[1], radius, Color::RED, 1, false, Color::RED);
            CircState.circ_state = 0;
            process_activated = false;
            std::cout << "process end";
            return;
        }
        
        
    }
}

std::vector<int> Application::OnMouseButtonDown( SDL_MouseButtonEvent event)
{
    
    
    if (event.button == SDL_BUTTON_LEFT) {
        
    
        std::vector <int> coordinates = {event.x, event.y};
        
        
        
        return coordinates;
        
    }
    
}

void Application::OnMouseButtonUp( SDL_MouseButtonEvent event )
{
    
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
