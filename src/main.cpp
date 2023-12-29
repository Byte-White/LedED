#include "raylib.h"
#include <iostream>
#include "editor.h"
#include <string.h>
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#ifndef __linux__
#include "style_terminal.h"
#include "style_cherry.h"
#endif


int main(int argc,char** argv)
{
    InitWindow(800, 450, "LedED");
    
    // Set a custom terminal-style theme
    // Add more style properties as needed
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    bool rgb_or_bw = false; // false - rgb , true - white/black
    int w_items=8,h_items=8;
    // parse args
    if(argc > 2)
    {
        if(isdigit(argv[1][0]))
        {
            w_items = std::stoi(argv[1]);
        }
        if(isdigit(argv[2][0]))
        {
            h_items = std::stoi(argv[2]);
        }
        if(argc>3)
        {
            if(strncmp(argv[3],"rgb",sizeof(argv[3]))==0)
                rgb_or_bw = false;
            
            else if(strncmp(argv[3],"bw",sizeof(argv[3]))==0)
                rgb_or_bw = true;
        }
    }
    

    #ifndef __linux__
    GuiLoadStyleTerminal();
    //GuiLoadStyleCherry();
    #endif

    Editor editor(w_items,h_items,rgb_or_bw);
    while (!WindowShouldClose())
    {
        BeginDrawing();
            editor.Render();
        EndDrawing();
    }
    
    CloseWindow();

    return 0;
}