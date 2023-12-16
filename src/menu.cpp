#include "editor.h"
#include <string>

void Editor::RGBAMenu()
{
    int rate = 1;
    Vector2 pos = GetMousePosition();

        // RED
    if(DrawColorPickButton("<", 10, 10, RED))
    {
        col.r = (uint8_t)std::max(0,std::min(int(col.r)-rate, 255));
    };
    if(DrawColorPickButton(">", 40, 10, RED))
    {
        col.r = (uint8_t)std::max(0, std::min(int(col.r)+rate, 255));
    };

    // GREEN
    if(DrawColorPickButton("<", 10, 40, GREEN))
    {
        col.g = (uint8_t)std::max(0,std::min(int(col.g)-rate, 255));
    };
    if(DrawColorPickButton(">", 40, 40, GREEN))
    {
        col.g = (uint8_t)std::max(0,std::min(int(col.g)+rate, 255));
    };

    // BLUE
    if(DrawColorPickButton("<", 10, 70, BLUE))
    {
        col.b = (uint8_t)std::max(0,std::min(int(col.b)-rate, 255));
    };
    if(DrawColorPickButton(">", 40, 70, BLUE))
    {
        col.b = (uint8_t)std::max(0,std::min(int(col.b)+rate, 255));
    };
    // TRANSPARENCY
    if(DrawColorPickButton("<", 10, 100, BLANK))
    {
        col.a = (uint8_t)std::max(0,std::min(int(col.a)-rate, 255));
    };
    if(DrawColorPickButton(">", 40, 100, BLANK))
    {
        col.a = (uint8_t)std::max(0,std::min(int(col.a)+rate, 255));
    };
}

void Editor::WBMenu()
{
    int rate = 1;
    Vector2 pos = GetMousePosition();

    if(DrawColorPickButton("<", 10, 10, BLACK))
    {
        col.bw = (uint8_t)std::max(0,std::min(int(col.bw)-rate, 255));
    };
    if(DrawColorPickButton(">", 40, 10, WHITE,BLACK))
    {
        col.bw = (uint8_t)std::max(0, std::min(int(col.bw)+rate, 255));
    };
}

void  Editor::DrawColorPreview()
{
    Vector2 pos = GetMousePosition();

    char datastr[16]; //rrr,ggg,bbb,aaa\0
    if(rgb_or_bw == 0)
    snprintf(datastr,sizeof(datastr),"%u,%u,%u,%u",(int)col.r,(int)col.g,(int)col.b,(int)col.a); //rgba
    else snprintf(datastr,sizeof(datastr),"%u",(int)col.bw); // white/black 
    DrawRectangle(70,10,100,100,WHITE);
    DrawRectangle(80,20,80,80,BLACK);
    DrawText(datastr,70,120,15,WHITE);
    Color c;
    if(rgb_or_bw == 1)
    {
        col.r = col.bw;
        col.g = col.bw;
        col.b = col.bw;
        col.a = 255;
    }
        c.r = col.r;
        c.g = col.g;
        c.b = col.b;
        c.a = col.a;
    DrawRectangle(80,20,80,80,c);
}

void Editor::DrawMenu()
{
    Vector2 pos = GetMousePosition();

    DrawColorPreview();
    if(rgb_or_bw == false)
        RGBAMenu();
    else
        WBMenu();
    
    // Export
    DrawRectangle(10,GetScreenHeight()-150,120,40,GREEN);
    DrawText("Export",35,GetScreenHeight()-140,20,WHITE);
    if((pos.x >= 10 && pos.x <= 130) && (pos.y >= GetScreenHeight()-150 && pos.y <= GetScreenHeight()-110))
    {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        cursorishovering = true;
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            tab = 2; // Export Menu
        }
    }
    
    //------ Frame Manager ------
    
    // Frame Browser
    if(DrawColorPickButton("<",10,GetScreenHeight()-80,ORANGE))
    {
        curframe = (unsigned int)std::max(0,std::min((int)curframe-1,(int)data.size()-1));
    }
    if(DrawColorPickButton(">",40,GetScreenHeight()-80,ORANGE))
    {
        curframe = (unsigned int)std::max(0,std::min((int)curframe+1,(int)data.size()-1));
    }
    char s[10];
    itoa(curframe,s,10);
    DrawText(s,75,GetScreenHeight()-80,25,WHITE);
    // New Frame Button 
    DrawRectangle(10,GetScreenHeight()-50,160,40,ORANGE);
    DrawText("New Frame",35,GetScreenHeight()-40,20,WHITE);
    // Frames Count
    char maxs[20];
    snprintf(maxs,20,"count: %u",data.size());
    DrawText(maxs,35,GetScreenHeight()-100,20,WHITE);
    if((pos.x >= 10 && pos.x <= 170) && (pos.y >= GetScreenHeight()-50 && pos.y <= GetScreenHeight()-10))
    {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        cursorishovering = true;
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            curframe=PushFrame();
        }
    }
    // Removes Current Frame
    DrawRectangle(180,GetScreenHeight()-50,120,40,RED);
    DrawText("Remove",200,GetScreenHeight()-40,20,WHITE);
    if((pos.x >= 180 && pos.x <= 300) && (pos.y >= GetScreenHeight()-50 && pos.y <= GetScreenHeight()-10))
    {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        cursorishovering = true;
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            if(data.size() == 1)
            {
                ResetAll(); // only clears the pixel data
            }
            else 
            {
                data.erase(std::next(data.begin(), curframe));
            }
        }
    }
}

void Editor::DrawExportMenu()
{
    static std::string error = "";
    Vector2 pos = GetMousePosition();
    DrawRectangle(10,10,120,40,GREEN);
    DrawText("Go Back",30,20,20,WHITE);
    if((pos.x >= 10 && pos.x <= 150) && (pos.y >= 10 && pos.y <= 50))
    {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        cursorishovering = true;
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            tab = 1; // Export Menu
        }
    }
    static std::string input = "exports/exportedfile.h";
    static int cursorlocation = input.size()-2;
    // InputBox For export file path
    DrawTextInput(input,210,70,50,std::min(20,(GetScreenWidth()-210)/50),cursorlocation,DARKGRAY,WHITE);
    // Export as array
    DrawRectangle(10,70,200,40,BLUE);
    DrawText("Export As Array",20,80,20,WHITE);
    DrawText(error.c_str(),10,120,20,GRAY);
    if((pos.x >= 10 && pos.x <= 180) && (pos.y >= 70 && pos.y <= 110))
    {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        cursorishovering = true;
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            Export(input,error);
        }
    }
}
