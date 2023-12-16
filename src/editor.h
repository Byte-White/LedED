#include "raylib.h"
#include <vector>
#include <inttypes.h>
#include <math.h>
#include <iostream>
#include <iomanip>
#include <sstream>

#define COLOR_BACKGROUND GetColor(0x151515FF)



class Editor
{
    union leddata
    {
        struct
        {
            uint8_t r,g,b,a;
        };
        uint8_t bw;

        bool operator==(const leddata& other) const
        {
            return (r == other.r) && (g == other.g) && (b == other.b) && (a == other.a);
        }

        std::string rgbatohex()
        {
            std::stringstream stream;
            stream  << std::setfill('0') << std::setw(2) << std::hex << (int)r
                    << std::setfill('0') << std::setw(2) << std::hex << (int)g
                    << std::setfill('0') << std::setw(2) << std::hex << (int)b
                    << std::setfill('0') << std::setw(2) << std::hex << (int)a;
        return stream.str();
        }

        std::string to_string(bool rgba_bw)
        {
            std::stringstream ss;
            
            if (rgba_bw == 0)
                ss << "color(rgba): " << (int)r << "," << (int)g << "," << (int)b << "," << (int)a;
            else
                ss << "color(black/white): " << (int)bw;
            
            return ss.str();
        }
    };
    int tab = 1;
    int w,h; // led screen width, led screen height
    bool rgb_or_bw; // rgb or white/black colors
    //bool normalised = false; // false : 0-255 , true : 0.0-1.0
    std::vector<std::vector<leddata>> data;// color data of each led
    leddata col;// current color shown in the preview
    bool cursorishovering = false;
    bool leftisholding = false;
    double leftdowntime = 0.0;
    unsigned int curframe = 0;// index of the frame
    private:
    void RGBAMenu();
    void WBMenu();
    //returns the index of the frame
    int PushFrame();
public:
    Editor(int w,int h,bool rgb_or_bw);
    ~Editor();
    void ResetAll();
    void DrawColorPreview();
    bool DrawColorPickButton(const char* label, int x, int y, Color color, Color textcolor = WHITE);
    void DrawMenu();
    void DrawExportMenu();
    void DrawButtons(const int& x_items, const int& y_items);
    void Render();
    void Export(const std::string& path,std::string& error);
    void DrawTextInput(std::string& txt,int x,int y,int n,int font,int& cursorlocation,Color bg = GRAY, Color tc = WHITE);
};
