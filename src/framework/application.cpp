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
    this->pencil_state = 1;
    this->eraser_state = 1;
    this->process_activated = false;
}

Application::~Application()
{
}

void Application::InitButtons(void) {
    pencil.coordinates = { 0 };
    pencil.image.LoadPNG("../res/images/pencil.png");
    eraser.coordinates = { 32 };
    eraser.image.LoadPNG("../res/images/eraser.png");
    line.coordinates = { 64 };
    line.image.LoadPNG("../res/images/line.png");
    rectangle.coordinates = { 96 };
    rectangle.image.LoadPNG("../res/images/rectangle.png");
    triangle.coordinates = { 128 };
    triangle.image.LoadPNG("../res/images/triangle.png");
    circle.coordinates = { 160 };
    circle.image.LoadPNG("../res/images/circle.png");
    animation.coordinates = { 192 };
    animation.image.LoadPNG("../res/images/animation.png");
    loadimage.coordinates = { 224 };
    loadimage.image.LoadPNG("../res/images/load.png");
    saveimage.coordinates = { 256 };
    saveimage.image.LoadPNG("../res/images/save.png");
    fill.coordinates = { 288 };
    fill.image.LoadPNG("../res/images/fill.png");
    newImage.coordinates = { 320 };
    newImage.image.LoadPNG("../res/images/clear.png");
    black.coordinates = { 352 };
    black.image.LoadPNG("../res/images/black.png");
    white.coordinates = { 384 };
    white.image.LoadPNG("../res/images/white.png");
    red.coordinates = { 416 };
    red.image.LoadPNG("../res/images/red.png");
    blue.coordinates = { 448 };
    blue.image.LoadPNG("../res/images/blue.png");
    yellow.coordinates = { 480 };
    yellow.image.LoadPNG("../res/images/yellow.png");
    pink.coordinates = { 512 };
    pink.image.LoadPNG("../res/images/purple.png");
    green.coordinates = { 544 };
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

    
    framebuffer.DrawRect(static_cast<int>(current_button.coordinates[0]), 0, 32, 32, border_color, 3, false, border_color);
    if (current_fill.coordinates[0] == black.coordinates[0]) {
        framebuffer.DrawCircle(static_cast<int>(current_fill.coordinates[0] + 16), 16, 8, Color::WHITE, 2, true, Color::WHITE);
    }
    else if (current_fill.coordinates[0] != black.coordinates[0]) {
        framebuffer.DrawCircle(static_cast<int>(current_fill.coordinates[0] + 16), 16, 8, Color::BLACK, 2, true, Color::BLACK);
    }
    if (fill_activated == true) {
        framebuffer.DrawRect(fill.coordinates[0], 0, 32, 32, Color::GREEN);
    }
    else if (fill_activated == false){
        framebuffer.DrawRect(fill.coordinates[0], 0, 32, 32, Color::RED);
    }
}

void Application::Init(void)
{
    std::cout << "Initiating app..." << std::endl;
    fill_activated = false;
    border_color = Color::WHITE;
    fill_color = Color::WHITE;
    InitButtons();
    current_button = pencil;
    current_fill = white;
    current_border = white;
    current_activated = 1;
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
    if ((click[0] < 96 && click[0] > 64 && click[1] > ( window_height - 32) && LineState.draw_line_state == 0 && process_activated == false && click[2] != 1) ){
        current_button = line;
        current_activated = 3;
        LineState.draw_line_state = 1;
        process_activated = true;
    
        ToolbarInit();
        
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
            framebuffer.DrawLineDDA(LineState.first_point[0], window_height - LineState.first_point[1], click[0], window_height - click[1], border_color);
            LineState.draw_line_state = 0;
            process_activated = false;
            std::cout << "process end";
            return;
        }
        
        
    }
    
    ///////////////////////////////////
    ///
    /////////////RECTANGLE/////////////
    if ((click[0] > 96 && click[0] < 128 && click[1] > ( window_height - 32) && RectState.rect_state == 0 && process_activated == false && click[2] != 1)){
        current_button = rectangle;
        current_activated = 4;
        RectState.rect_state = 1;
        process_activated = true;
        
        ToolbarInit();
        
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
                
                framebuffer.DrawRect(click[0], window_height -  RectState.first_point[1], RectState.first_point[0] - click[0], RectState.first_point[1] - click[1], border_color, 1, fill_activated, fill_color);
                
                
                RectState.rect_state = 0;
                process_activated = false;
                return;
                
            }else if( RectState.first_point[0] >= click[0] && (window_height - RectState.first_point[1]) >= (window_height - click[1])){ // third quadrant
                
                framebuffer.DrawRect(click[0], window_height - click[1], RectState.first_point[0] - click[0], click[1] - RectState.first_point[1], border_color, 1, fill_activated, fill_color);
                
                
                RectState.rect_state = 0;
                process_activated = false;
                return;
                
            }else if( RectState.first_point[0] <= click[0] && (window_height - RectState.first_point[1]) >= (window_height - click[1])){ // fourth quadrant
                
                framebuffer.DrawRect(RectState.first_point[0], window_height - click[1], click[0] - RectState.first_point[0], click[1] - RectState.first_point[1], border_color, 1, fill_activated, fill_color);
                
                
                RectState.rect_state = 0;
                process_activated = false;
                return;
                
            }else{
                framebuffer.DrawRect(RectState.first_point[0], window_height - RectState.first_point[1], click[0] - RectState.first_point[0], RectState.first_point[1] - click[1], border_color, 1, fill_activated, fill_color);
                
                
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
    if (click[0] < 192 && click[0] > 160 && click[1] > ( window_height - 32) && CircState.circ_state == 0 && process_activated == false && click[2] != 1){
        current_button = circle;
        current_activated = 6;
        CircState.circ_state = 1;
        process_activated = true;
        
        ToolbarInit();
        
        
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
            
            framebuffer.DrawCircle(CircState.center[0], window_height - CircState.center[1], radius, border_color, 1, fill_activated, fill_color);
            CircState.circ_state = 0;
            process_activated = false;
            ToolbarInit();
            return;
        }
    }
        ///////////////////////////////
        ///
        //////////TRIANGLE/////////////
        
        if (click[0] > 128 && click[0] < 160 && click[1] > ( window_height - 32) && TriState.tri_state == 0 && process_activated == false && click[2] != 1){
            current_button = triangle;
            current_activated = 5;
            TriState.tri_state = 1;
            process_activated = true;
            
            ToolbarInit();
            
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
                
                framebuffer.DrawTriangle(p1, p2, p3, border_color, fill_activated, fill_color);
                
                
                TriState.tri_state = 0;
                process_activated = false;
                std::cout << "process end";
                return;
            }
        }
    
    ///////////////////////////////
    //////////PENCIL///////////////
    ///
    if (click[0] > 0 && click[0] < 32 && click[1] > ( window_height - 32) && pencil_state == 1 && process_activated == false && click[2] != 1){
        current_activated = 1;
        current_button = pencil;
        process_activated = true;
        std::cout <<"caca1";
        ToolbarInit();
        pencil_state = 1;
        return;
        
    }else if (current_activated == 1 && click[1] < ( window_height - 32) && pencil_state == 1){
        pencil_state = 2;
        process_activated = true;
        return;
    }else if (current_activated == 1 && click[1] < ( window_height - 32) && pencil_state == 2){
        
        pencil_state = 1;
        process_activated = false;
        return;
        
    }
    /// ERASER
    if (click[0] > 32 && click[0] < 64 && click[1] > (window_height - 32) && eraser_state == 1 && process_activated == false && click[2] != 1) {
        current_activated = 2;
        current_button = eraser;
        process_activated = true;
        ToolbarInit();
        eraser_state = 1;
        return;

    }
    else if (current_activated == 2 && click[1] < (window_height - 32) && eraser_state == 1) {
        eraser_state = 2;
        process_activated = true;
        return;
    }
    else if (current_activated == 2 && click[1] < (window_height - 32) && eraser_state == 2) {

        eraser_state = 1;
        process_activated = false;
        return;

    }

    // LOADIMAGE
    if (click[0] < loadimage.coordinates[0] + 32 && click[0] > loadimage.coordinates[0] && click[1] > (window_height - 32) && process_activated == false && click[2] != 1) {
        framebuffer.DrawImage("../res/images/pimfroi.png", false, 0, 32);
        ToolbarInit();
    }

    //SAVEIMAGE
    if (click[0] < saveimage.coordinates[0] + 32 && click[0] > saveimage.coordinates[0] && click[1] > (window_height - 32) && process_activated == false && click[2] != 1) {
        framebuffer.SaveTGA("drawing.tga");
        ToolbarInit();
    }

    //ANIMATION
    if (click[0] < animation.coordinates[0] + 32 && click[0] > animation.coordinates[0] && click[1] > (window_height - 32) && process_activated == false && click[2] != 1) {
        
        ToolbarInit();
    }

    // FILL
    if (click[0] < fill.coordinates[0]+32 && click[0] > fill.coordinates[0] && click[1] > (window_height - 32) && process_activated == false && click[2] != 1) {
        if (fill_activated == true) {
            fill_activated = false;
        }
        else {
            fill_activated = true;
        }
        ToolbarInit();
    }

    // CLEAR
    if (click[0] < 352 && click[0] > 320 && click[1] > (window_height - 32) && process_activated == false && click[2] != 1) {
        current_activated = 10;
        process_activated = true;

        framebuffer.Fill(Color::BLACK);
        ToolbarInit();
        process_activated = false;
    }
    // COLORS
    // BLACK
    if (click[0] < 384 && click[0] > 352 && click[1] > (window_height - 32) && process_activated == false) {

        if (click[2] == 1) {
            current_fill = black;
            fill_color = Color::BLACK;
        }
        else {
            current_border = black;
            border_color = Color::BLACK;
        }
        
        ToolbarInit();
    }
    // WHITE
    if (click[0] < 416 && click[0] > 384 && click[1] > (window_height - 32) && process_activated == false) {

        if (click[2] == 1) {
            current_fill = white;
            fill_color = Color::WHITE;
        }
        else {
            current_border = white;
            border_color = Color::WHITE;
        }
        ToolbarInit();
    }
    // RED
    if (click[0] < red.coordinates[0]+32 && click[0] > red.coordinates[0] && click[1] > (window_height - 32) && process_activated == false) {

        if (click[2] == 1) {
            current_fill = red;
            fill_color = Color::RED;
        }
        else {
            current_border = red;
            border_color = Color::RED;
        }
        ToolbarInit();
    }
    // BLUE
    if (click[0] < blue.coordinates[0] + 32 && click[0] > blue.coordinates[0] && click[1] > (window_height - 32) && process_activated == false) {

        if (click[2] == 1) {
            current_fill = blue;
            fill_color = Color::BLUE;
        }
        else {
            current_border = blue;
            border_color = Color::BLUE;
        }
        ToolbarInit();
    }
    // YELLOW
    if (click[0] < yellow.coordinates[0] + 32 && click[0] > yellow.coordinates[0] && click[1] > (window_height - 32) && process_activated == false) {

        if (click[2] == 1) {
            current_fill = yellow;
            fill_color = Color::YELLOW;
        }
        else {
            current_border = yellow;
            border_color = Color::YELLOW;
        }
        ToolbarInit();
    }
    // PINK
    if (click[0] < pink.coordinates[0] + 32 && click[0] > pink.coordinates[0] && click[1] > (window_height - 32) && process_activated == false) {

        if (click[2] == 1) {
            current_fill = pink;
            fill_color = Color::PURPLE;
        }
        else {
            current_border = pink;
            border_color = Color::PURPLE;
        }
        ToolbarInit();
    }
    // GREEN
    if (click[0] < green.coordinates[0] + 32 && click[0] > green.coordinates[0] && click[1] > (window_height - 32) && process_activated == false) {

        if (click[2] == 1) {
            current_fill = green;
            fill_color = Color::GREEN;
        }
        else {
            current_border = green;
            border_color = Color::GREEN;
        }
        ToolbarInit();
    }
}

std::vector<int> Application::OnMouseButtonDown( SDL_MouseButtonEvent event)
{
    
    if (event.button == SDL_BUTTON_LEFT) {
        
    
        std::vector <int> coordinates = {event.x, event.y, 0};
        
        
        
        return coordinates;
        
    }
    else {
        std::vector <int> coordinates = {event.x, event.y, 1};
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
    if(pencil_state == 2){
        if (event.y < window_height - 32){
            framebuffer.DrawCircle(event.x, window_height - event.y, 4, border_color, 1, true, border_color);
        }
    }
    else if (eraser_state == 2) {
        if (event.y < window_height - 32) {
            framebuffer.DrawCircle(event.x, window_height - event.y, 15, Color::BLACK, 1, true, Color::BLACK);
        }
    }
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
