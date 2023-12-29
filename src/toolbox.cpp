#include "editor.h"

const int screenWidth = 800;
const int screenHeight = 450;

#if defined(_WIN32)
extern wchar_t* OpenFileDialog();
#elif defined(__linux__)
extern char* OpenFileDialog();
#endif

template <typename T>
T clamp(T value, T minValue, T maxValue) {
    return std::min(std::max(value, minValue), maxValue);
}

void Editor::RenderToolbox()
{
    static Texture2D texture = { 0 };
    Image selected_image = { 0 };
    static bool texture_loaded = false;
    static std::string error_message = "";
    toolboxwindow = {
    (float)GetScreenWidth()*0.75f, 10,
        (float)GetScreenWidth()*0.25f, (float)GetScreenHeight() - 20
    };
    
    if(GuiWindowBox(toolboxwindow,"#140#Toolbox")) // when you click X -> reset toolbox
    {
        if(texture_loaded)
        {
            if(selected_image.data != NULL)
            {
                UnloadImage(selected_image);
                UnloadTexture(texture);
            }
            texture_loaded = false;
        }
        paste_x = 0; 
        paste_y = 0;
        pasting_image = false;
        image_filepath = "";
        error_message = "";
    }

    if(GuiButton({toolboxwindow.x+10,toolboxwindow.y+30,100,20},"#3# Load Image"))
    {
        //platform specific code
        #ifdef _WIN32
        wchar_t* selectedFile = OpenFileDialog();
        if (selectedFile != NULL)
        {
            std::wstring wstr = selectedFile;
            image_filepath = std::string(wstr.begin(),wstr.end());
            free(selectedFile);
        }
        #elif defined(__linux__)
        char* selectedFile = OpenFileDialog();
        if(selectedFile != NULL)
        {
            image_filepath = selectedFile;
            free(selectedFile);
        }
        #endif
        if(selectedFile != NULL)
        {
            if(texture_loaded && selected_image.data != NULL)
            {
                UnloadImage(selected_image);
                UnloadTexture(texture);
            }
            selected_image = LoadImage(image_filepath.c_str());
            texture = LoadTextureFromImage(selected_image);
            texture_loaded = true;
            texture_width = texture.width;
            texture_height = texture.height;
        }
    }
    GuiLabel({toolboxwindow.x+10,toolboxwindow.y+55,toolboxwindow.width-20,20},image_filepath.c_str());
    if(texture_loaded)
    {
        // --- old ui with a slider  ---
        //if(GuiButton({toolboxwindow.x+10,toolboxwindow.y+80,20,20},"-")) texture_width--;
        //if(GuiButton({toolboxwindow.x+30,toolboxwindow.y+80,20,20},"+")) texture_width++;
        //if(GuiButton({toolboxwindow.x+10,toolboxwindow.y+100,20,20},"-")) texture_height--;
        //if(GuiButton({toolboxwindow.x+30,toolboxwindow.y+100,20,20},"+")) texture_height++;
        //GuiSlider({ toolboxwindow.x + 50, toolboxwindow.y + 125, toolboxwindow.width - 90, 20 }, "Width", 
        //(std::to_string((int)texture_width)).c_str(), &texture_width, 1, 1080);
        
        //GuiSlider({ toolboxwindow.x + 50, toolboxwindow.y + 150, toolboxwindow.width - 90, 20 }, 
        //"Height", (std::to_string((int)texture_height)).c_str(), &texture_height, 1, 1080);
        // --- new ui with a spinner ---
        static int selected_editing_size = 0; // neither
        if(GuiSpinner({ toolboxwindow.x + 50, toolboxwindow.y + 125, toolboxwindow.width - 90, 20 }, "Width", 
        &texture_width, 1, 1080, selected_editing_size==1)) selected_editing_size = 1;
        
        if(GuiSpinner({ toolboxwindow.x + 50, toolboxwindow.y + 150, toolboxwindow.width - 90, 20 }, 
        "Height", &texture_height, 1, 1080, selected_editing_size==2)) selected_editing_size = 2;


        texture_width = clamp(texture_width, 0, 1080);
        texture_height = clamp(texture_height, 0, 1080);
        
        
        /*if(image_should_resize)
        {
            UnloadTexture(texture);
            ImageResize(&selected_image, (int)texture_width, (int)texture_height);
            
            if (selected_image.data != NULL)
            {
                texture = LoadTextureFromImage(selected_image);
            }
            else
            {
                TraceLog(LOG_WARNING, "Image resize failed!");
            }
            image_should_resize = false;
        }*/

        if(GuiButton({ toolboxwindow.x + 10, toolboxwindow.y + 175, 100, 20 },"#33#resize"))
        {
            Image newimage;
            newimage = LoadImageFromTexture(texture);
            ImageResize(&newimage, (int)texture_width, (int)texture_height);
            if (newimage.data != NULL)
            {
                UnloadImage(selected_image);
                selected_image = newimage;
                UnloadTexture(texture);
                texture = LoadTextureFromImage(selected_image);
                error_message = "";
            }
            else
            {
                TraceLog(LOG_WARNING, "Image resize failed!");
            }
        }
        

        DrawTexture(texture, toolboxwindow.x+10, toolboxwindow.y+195, WHITE);
        // Paste Image
        GuiSlider({ toolboxwindow.x + 20, toolboxwindow.y + 200 + texture.height, toolboxwindow.width - 40, 20 }, "X", 
        (std::to_string((int)paste_x)).c_str(), &paste_x, 0, w-1);
        
        GuiSlider({ toolboxwindow.x + 20, toolboxwindow.y + 220 + texture.height, toolboxwindow.width - 40, 20 }, "Y",
        (std::to_string((int)paste_y)).c_str(), &paste_y, 0, h-1);

        if(GuiButton({ toolboxwindow.x + 20, toolboxwindow.y + 240 + texture.height, toolboxwindow.width - 40, 20 },"#18#Paste Image"))
        {
            if(((int)texture_width == texture.width) && ((int)texture_height == texture.height))
            {
                error_message = "";
                Image img = LoadImageFromTexture(texture);
                // i is y ; j is x
                for(int i = (int)paste_y; i<std::min(paste_y+h,paste_y+(int)texture_height);i++)
                {
                    if(i>=h)break;
                    for(int j = (int)paste_x; j<std::min(paste_x+w,paste_x+(int)texture_width);j++)
                    {
                        if(j>=w)break;
                        Color pixelcolor = GetImageColor(img,j-(int)paste_x,i-(int)paste_y);
                        data[curframe][i*w+j].r = pixelcolor.r;
                        data[curframe][i*w+j].g = pixelcolor.g;
                        data[curframe][i*w+j].b = pixelcolor.b;
                        data[curframe][i*w+j].a = pixelcolor.a;
                    }
                }
            }
            else
            {
                error_message = "you haven't resized the image";
            }
        }
        GuiLabel({ toolboxwindow.x + 20, toolboxwindow.y + 260 + texture.height, toolboxwindow.width - 40, 20 },error_message.c_str());

        pasting_image = true;
    }
    else { pasting_image = false; }

    // TODO: Add "Rotate Data buffer"

}