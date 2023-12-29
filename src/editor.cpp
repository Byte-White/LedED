#include "editor.h"

Editor::Editor(int w, int h,bool rgb_or_bw)
:w(w),h(h),rgb_or_bw(rgb_or_bw)
{
    data.push_back(std::vector<leddata>(w*h));
    if(rgb_or_bw == 0)
    {
        col.r = 0;
        col.g = 0;
        col.b = 0;
        col.a = 255;
    }
    else
    {
        col.bw = 255;
    }
}


Editor::~Editor()
{
}

int Editor::PushFrame()
{
    auto insertPosition = data.begin() + curframe + 1;
    data.insert(insertPosition, 1, std::vector<leddata>(w * h));
    //data.push_back(std::vector<leddata>(w*h));
    return curframe+1;
}


void Editor::ResetAll()
{
    for(auto& ledvec : data)
    {
        for(auto& i : ledvec)
        {
            if(rgb_or_bw==0)
            {
                i.r = 0;
                i.g = 0;
                i.b = 0;
                i.a = 255;
            }
            else
            {
                i.bw = 255;
            }
        }
    }
}



// led buttons
void Editor::DrawButtons(const int& x_items, const int& y_items)
{
    snprintf(hoverdatatext,60,"");
    Vector2 pos = GetMousePosition();
    int starting_x = GetScreenWidth() / 4 + camera_x;
    int starting_y = 0 + camera_y; // or GetScreenHeight() / 4 if you want menu at the top 

    int total_width = GetScreenWidth() * 0.75; // 3/4 from the screen
    int total_height = GetScreenHeight() - 50; // leaves 50 pixels from below

    int x_per_item = total_width / x_items;
    int y_per_item = total_height / y_items;
    int size = x_per_item < y_per_item ? x_per_item - 2: y_per_item - 2;
    size = int(camera_scale*size);

    starting_x += (total_width - x_per_item * x_items) / 2; // Center horizontally
    starting_y += (total_height - y_per_item * y_items) / 2; // Center vertically
    //draws grid
    DrawRectangle(starting_x-2,starting_y,(size+2)*x_items+2,(size+2)*y_items,DARKGRAY);
    //draws paste location
    if(pasting_image)
    DrawRectangle(starting_x-2 + (int)paste_x*(size+2),starting_y + (int)paste_y*(size+2),(size+2)*texture_width,(size+2)*texture_height,YELLOW);
    for (int i = 0; i < y_items; i++)
    {
        for (int j = 0; j < x_items; j++)
        {
            Color color;
            if(data[curframe][(i*w)+j].a != 0)
            {
                color.r = data[curframe][(i*w)+j].r;
                color.g = data[curframe][(i*w)+j].g;
                color.b = data[curframe][(i*w)+j].b;
                color.a = data[curframe][(i*w)+j].a;
            }
            else if (data[curframe][(i*w)+j].r == 0 && data[curframe][(i*w)+j].g == 0 && data[curframe][(i*w)+j].b == 0)
            { // no transparent turned off led
                color.r = 0;
                color.b = 0;
                color.g = 0;
                color.a = 255;
            }
            else{ // no fully transparent led
                color.r = 0;
                color.b = 0;
                color.g = 0;
                color.a = 255;
            }
            int led_x = starting_x + j * (size + 2);
            int led_y = starting_y + i * (size + 2);
            // draws led button
            DrawRectangle(led_x, led_y, size, size, color);
            // can't click it if its behind the menu/toolbar
            if(led_x>170 && led_x<toolboxwindow.x-size && (pos.x >= led_x && pos.x <= led_x + size) && (pos.y >= led_y && pos.y <= led_y + size))
            {
                SetMouseCursor(MOUSE_CURSOR_CROSSHAIR);
                cursorishovering = true;
                
                snprintf(hoverdatatext,60,"hovering over:\nx: %d\ny: %d\n%s",j,i,data[curframe][i*w+j].to_string(rgb_or_bw).c_str());//j - x    i - y
                if(singledrawmode)
                { // click to draw
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) // if led color is the same as preview color clear the led
                    {
                        if(data[curframe][i*w+j] == col)
                        {
                            leddata blank;
                            blank.r = 0;
                            blank.g = 0;
                            blank.b = 0;
                            blank.a = 255;
                            data[curframe][i*w+j] = blank;
                        }
                        else data[curframe][i*w+j]=col;
                    }
                }
                else
                { // hold to draw
                    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) 
                        data[curframe][i*w+j]=col;
                }
            }
            
        }
    }
}

void Editor::HandleCamera()
{
    Vector2 mousePos = GetMousePosition();
    static double pos_rate = 1;
    static double scale_rate = 1.0;
    if(IsKeyDown(KEY_LEFT))
    {
        camera_x+=pos_rate;
    }
    if(IsKeyDown(KEY_RIGHT))
    {
        camera_x-=pos_rate;
    }
    if(IsKeyDown(KEY_UP))
    {
        camera_y+=pos_rate;
    }
    if(IsKeyDown(KEY_DOWN))
    {
        camera_y-=pos_rate;
    }
    if(IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE) || IsKeyPressed(KEY_SPACE))
    {
        camera_y = camera_x = 0;
        camera_scale = 0;
    }
    // Calculate the zoom factor based on the mouse wheel movement
    double zoomFactor = 1.0 + scale_rate * GetMouseWheelMove();

    // Adjust camera_scale based on the zoom factor
    camera_scale *= zoomFactor;
    camera_scale = std::max(1.0, std::min(20.0, camera_scale));
}


void Editor::Render()
{
    ClearBackground(COLOR_BACKGROUND);
    
    if(tab == 0)
    { // introcution page
        // TODO: implement 
    }
    else if(tab == 1)
    { // main editor page
        DrawButtons(w,h);
        DrawMenu();
        HandleCamera();
        RenderToolbox();
    }
    else if(tab == 2)
    { // export page
        DrawExportMenu();
    }
    else
    {
        tab = 1;
    }
    //resets cursor
    if(cursorishovering)
        cursorishovering = false;
    else SetMouseCursor(MOUSE_CURSOR_DEFAULT);

}