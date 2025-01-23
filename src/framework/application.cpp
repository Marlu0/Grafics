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
    this->TriState.tri_state = 0;
    this->current_activated = 0;
    this->process_activated = false;
}

Application::~Application()
{
}

void Application::InitButtons(void) {
    pencil.image.LoadPNG("../res/images/pencil.png");
    eraser.image.LoadPNG("../res/images/eraser.png");
    line.image.LoadPNG("../res/images/line.png");
    rectangle.image.LoadPNG("../res/images/rectangle.png");
    triangle.image.LoadPNG("../res/images/triangle.png");
    circle.image.LoadPNG("../res/images/circle.png");
    animation.image.LoadPNG("../res/images/animation.png");
    loadimage.image.LoadPNG("../res/images/load.png");
    saveimage.image.LoadPNG("../res/images/save.png");
    fill.image.LoadPNG("../res/images/fill.png");
    newImage.image.LoadPNG("../res/images/clear.png");
    black.image.LoadPNG("../res/images/black.png");
    white.image.LoadPNG("../res/images/white.png");
    red.image.LoadPNG("../res/images/red.png");
    blue.image.LoadPNG("../res/images/blue.png");
    yellow.image.LoadPNG("../res/images/yellow.png");
    pink.image.LoadPNG("../res/images/purple.png");
    green.image.LoadPNG("../res/images/green.png");
}

void Application::ToolbarInit(void) {
    framebuffer.DrawRect(0, 0, window_width, 32, Color::GRAY, 1, true, Color::GRAY);

    // Draw each button image
    framebuffer.DrawImage("../res/images/pencil.png", false, 0, 0);
    framebuffer.DrawImage("../res/images/eraser.png", false, 32, 0);
    framebuffer.DrawImage("../res/images/line.png", false, 64, 0);
    framebuffer.DrawImage("../res/images/rectangle.png", false, 96, 0);
    framebuffer.DrawImage("../res/images/triangle.png", false, 128, 0);
    framebuffer.DrawImage("../res/images/circle.png", false, 160, 0);
    framebuffer.DrawImage("../res/images/animation.png", false, 192, 0);
    framebuffer.DrawImage("../res/images/load.png", false, 224, 0);
    framebuffer.DrawImage("../res/images/save.png", false, 256, 0);
    framebuffer.DrawImage("../res/images/fill.png", false, 288, 0);
    framebuffer.DrawImage("../res/images/clear.png", false, 320, 0);
    framebuffer.DrawImage("../res/images/black.png", false, 352, 0);
    framebuffer.DrawImage("../res/images/white.png", false, 384, 0);
    framebuffer.DrawImage("../res/images/red.png", false, 416, 0);
    framebuffer.DrawImage("../res/images/blue.png", false, 448, 0);
    framebuffer.DrawImage("../res/images/yellow.png", false, 480, 0);
    framebuffer.DrawImage("../res/images/pink.png", false, 512, 0);
    framebuffer.DrawImage("../res/images/green.png", false, 544, 0);
}

void Application::Init(void)
{
    std::cout << "Initiating app..." << std::endl;
    InitButtons();
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
    if(click[1] > (window_height - 32) && process_activated == true){
        return;
    }
    
    /////////////LINE////////////////////
    if ((click[0] < 96 && click[0] > 64 && click[1] > ( window_height - 32) && LineState.draw_line_state == 0 && process_activated == false) ){
        current_activated = 3;
        LineState.draw_line_state = 1;
        process_activated = true;
    
        ToolbarInit();
        framebuffer.DrawRect(64, 0, 32, 32, Color::RED, 3, false, Color::RED);
        
    }else if (current_activated == 3 && click[1] < (window_height - 32)){
        if(process_activated == false ){
            process_activated = true;
            LineState.draw_line_state = 1;
        }
        
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
    if ((click[0] > 96 && click[0] < 128 && click[1] > ( window_height - 32) && RectState.rect_state == 0 && process_activated == false)){
        
        current_activated = 4;
        RectState.rect_state = 1;
        process_activated = true;
        
        ToolbarInit();
        framebuffer.DrawRect(96, 0, 32, 32, Color::RED, 3, false, Color::RED);
        
    }else if (current_activated == 4 && click[1] < (window_height - 32)){
        
        if(process_activated == false ){
            process_activated = true;
            RectState.rect_state = 1;
        }
        
        
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
        current_activated = 6;
        CircState.circ_state = 1;
        process_activated = true;
        
        ToolbarInit();
        framebuffer.DrawRect(160, 0, 32, 32, Color::RED, 3, false, Color::RED);
        
        
    }else if ( current_activated == 6 && click[1] < (window_height - 32)){
        
        if(process_activated == false ){
            process_activated = true;
            CircState.circ_state = 1;
        }
        
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
        ///////////////////////////////
        ///
        //////////TRIANGLE/////////////
        
        if (click[0] > 128 && click[0] < 160 && click[1] > ( window_height - 32) && TriState.tri_state == 0 && process_activated == false){
            
            current_activated = 5;
            TriState.tri_state = 1;
            process_activated = true;
            
            ToolbarInit();
            framebuffer.DrawRect(128, 0, 32, 32, Color::RED, 3, false, Color::RED);
            
        }else if (current_activated == 5 && click[1] < (window_height - 32)){
            
            if(process_activated == false ){
                process_activated = true;
                TriState.tri_state = 1;
            }
            
            if(TriState.tri_state == 1){
                TriState.first = click;
                
                TriState.tri_state = 2;
                return;
                
            }else if (TriState.tri_state == 2){
                TriState.second = click;
                
                TriState.tri_state = 3;
                return;
                
            }else if(TriState.tri_state == 3){
                Vector2 p1(TriState.first[0], window_height - TriState.first[1]);
                Vector2 p2(TriState.second[0], window_height - TriState.second[1]);
                Vector2 p3(click[0], window_height - click[1]);
                
                framebuffer.DrawTriangle(p1, p2, p3, Color::RED, false, Color::RED);
                
                
                TriState.tri_state = 0;
                process_activated = false;
                std::cout << "process end";
                return;
            }
        }
    
    ///////////////////////////////
    //////////PENCIL///////////////
    ///
    
    if (click[0] > 0 && click[0] < 32 && click[1] > ( window_height - 32) && TriState.tri_state == 0 && process_activated == false){
        current_activated = 1;
        
        process_activated = true;
        
        ToolbarInit();
        framebuffer.DrawRect(0, 0, 32, 32, Color::RED, 3, false, Color::RED);
        
    }else if (current_activated == 1 && click[1] < ( window_height - 32)){
        
        
    }
    
}

std::vector<int> Application::OnMouseButtonDown( SDL_MouseButtonEvent event)
{
    
    
    if (event.button == SDL_BUTTON_LEFT) {
        
    
        std::vector <int> coordinates = {event.x, event.y};
        
        
        
        return coordinates;
        
    }
    
}

bool Application::OnMouseButtonUp( SDL_MouseButtonEvent event )
{
    if(event.type == SDL_MOUSEBUTTONUP){
        return true;
    }else{
        return false;
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
