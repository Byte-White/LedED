#include "editor.h"

std::string GetKeyString(int keyCode, bool shiftPressed)
{
    if (keyCode >= KEY_A && keyCode <= KEY_Z)
    {
        return shiftPressed ? std::string(1, 'A' + (keyCode - KEY_A)) : std::string(1, 'a' + (keyCode - KEY_A));
    }
    else if (keyCode >= KEY_ZERO && keyCode <= KEY_NINE)
    {
        // Map numbers based on shift state
        return shiftPressed ? std::string(1, ")!@#$%^&*("[keyCode - KEY_ZERO]) : std::string(1, '0' + (keyCode - KEY_ZERO));
    }
    else
    {
        // Handle other special keys as needed
        switch (keyCode)
        {
        case KEY_SPACE:
            return " ";
        case KEY_MINUS:
            return shiftPressed ? "_" : "-";
        case KEY_SEMICOLON:
            return shiftPressed ? ":" : ";";
        // Add more cases as needed
        default:
            return std::string(1,(char)keyCode);
        }
    }
}


bool Editor::DrawColorPickButton(const char* label, int x, int y,Color color, Color textcolor)
{

    Vector2 pos = GetMousePosition();

    // Draw color button
    DrawRectangle(x, y, 25, 25, color);
    DrawText(label, x + 10, y, 25, textcolor);

    // Check if the button is pressed
    if((pos.x >= x && pos.x <= x + 25) && (pos.y >= y && pos.y <= y + 25))
    {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        cursorishovering = true;
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            if(!leftisholding)
            {
                leftdowntime = GetTime(); 
                leftisholding = true;
                return true;
            }
            if(leftisholding &&(GetTime() > (leftdowntime+0.5))) // fast growing after 0.5 seconds
            {
                leftdowntime+=0.01;
                return true;
            }
        }
        else
        {
            leftdowntime = 0.0;
            leftisholding = false;
        }

    }

    return false;
}
void Editor::DrawTextInput(std::string& txt,int x,int y,int n, int font, int& cursorlocation, Color bg, Color tc)
{
    static double lastblink = GetTime();
    static bool hasblinked = false; // if true will draw the cursor
    Vector2 pos = GetMousePosition();

    DrawRectangle(x, y, font*n, font + 20, bg);

    DrawText(txt.c_str(), x + font / 2, y + 10, font, tc);

    if ((pos.x >= x && pos.x <= x + MeasureText(txt.c_str(), font)) && (pos.y >= y && pos.y <= y + font + 20))
    {
        SetMouseCursor(MOUSE_CURSOR_IBEAM);
        cursorishovering = true;
    }

    // Calculate cursor position
    int relative_x = x + font / 2 + MeasureText(txt.substr(0, cursorlocation).c_str(), font);
    if(hasblinked && GetTime() <= lastblink)
    {
        DrawLine(relative_x+2, y + 10, relative_x+2, y + font + 10, WHITE);
    }
    else if(GetTime() >= lastblink && hasblinked)
    {
        hasblinked = false;
        lastblink = GetTime()+0.5;
    }
    else if(hasblinked==false && GetTime()>=lastblink)
    {
        hasblinked = true;
        lastblink = GetTime()+0.5;
    }

    // cursor movement
    if (IsKeyPressed(KEY_LEFT))
    {
        cursorlocation = std::max(0, cursorlocation - 1);
        hasblinked = true; // so it doesnt blink while moving
        lastblink = GetTime()+0.5; // so it doesnt blink while moving
    }

    if (IsKeyPressed(KEY_RIGHT))
    {
        cursorlocation = std::min(cursorlocation + 1, static_cast<int>(txt.size()));
        hasblinked = true; // so it doesnt blink while moving
        lastblink = GetTime()+0.5; // so it doesnt blink while moving
    }

    // space-z

    for(int i = KEY_SPACE; i<=KEY_Z; i++)
    {
        bool shiftPressed = IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT);
        if(IsKeyPressed(i))
        {
            std::string c = GetKeyString(i, shiftPressed);
            if(c!="")
            {
                txt.insert(cursorlocation, c);
                cursorlocation++;
            }
            hasblinked = true; // so it doesnt blink while typing
            lastblink = GetTime()+0.5; // so it doesnt blink while typing
        }
    }

    // backspace
    if(IsKeyPressed(KEY_BACKSPACE) && txt.size()>=0 && cursorlocation != 0)
    {
        txt = txt.erase(cursorlocation-1,1);
        cursorlocation--;
            hasblinked = true; // so it doesnt blink while typing
            lastblink = GetTime()+0.5; // so it doesnt blink while typing
    }
}
