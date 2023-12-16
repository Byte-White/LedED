#include "raylib.h"
#include <iostream>
#include "editor.h"
#include <string.h>

#include "iconembed.h"


int main(int argc,char** argv)
{
    InitWindow(800, 450, "LedED");
    

    SetWindowState(FLAG_WINDOW_RESIZABLE);
    bool rgb_or_bw = false; // false - rgb , true - white/black
    int w_items=8,h_items=8;
    // parse args
    if(argc > 2)
    {
        if(isdigit(argv[1][0]))
        {
            w_items = atoi(argv[1]);
        }
        if(isdigit(argv[2][0]))
        {
            h_items = atoi(argv[2]);
        }
        if(argc>3)
        {
            if(strncmp(argv[3],"rgb",sizeof(argv[3]))==0)
                rgb_or_bw = false;
            
            else if(strncmp(argv[3],"bw",sizeof(argv[3]))==0)
                rgb_or_bw = true;
        }
    }
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