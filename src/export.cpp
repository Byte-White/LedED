#include "editor.h"
#include <fstream>
#include <sstream>
#include <filesystem>

namespace fs = std::filesystem;

// 
// bool FileExists(const std::string& path)
// {
//     std::ifstream fileStream(path);
//     if (fileStream.good())
//     {
//         fileStream.close();
//         return true;
//     }
//     return false;
// }

void Editor::Export(const std::string& path, std::string& error)
{
    error = ""; // clear error messages


    // Extract the directory path from the full path
    std::string directory = path.substr(0, path.find_last_of("/\\"));


    if (!fs::exists(directory))
    {
        if (!fs::create_directories(directory))
        {
            error = "Error: Couldn't create directory!";
            return;
        }
    }

    if (fs::exists(path))
    {
        error = "Error: File already exists!";
        return;
    }
    
    std::ofstream out(path);
    if(!out.good())
    {
        error = "Error: Couldn't create file!";
        return;
    }
    std::stringstream ss;
    if(rgb_or_bw == 0)
    { // rgba
        ss<<"uint32_t led_data["; // rrggbbaa 4 bytes of data
        if(data.size() == 1)
        { // handle 1 frame differently

            ss<<data[0].size()<<"] = {\n";
            for(auto& led:data[0])
            {
                ss<<"0x"<<led.rgbatohex()<<", ";
            }
            ss<<"};";
        }
        else
        {
            ss<<data.size()<<"]["<<data[0].size()<<"] = {\n";
            for(auto& frame : data)
            {
                ss<<"{";
                for(auto& led : frame)
                {
                    ss<<"0x"<<led.rgbatohex()<<", ";
                }
                ss<<"},\n";
            }
            ss<<"};";
        }
    }
    else
    { // white/black
        ss<<"uint8_t led_data[";
        if(data.size() == 1)
        { // handle 1 frame differently

            ss<<data[0].size()<<"] = {\n";
            for(auto& led:data[0])
            {
                ss<<led.bw<<", ";
            }
            ss<<"};";
        }
        else
        {
            ss<<data.size()<<"]["<<data[0].size()<<"] = {\n";
            for(auto& frame : data)
            {
                ss<<"{";
                for(auto& led : frame)
                {
                    ss<<led.bw<<", ";
                }
                ss<<"},\n";
            }
            ss<<"};";
        }
    }
    // save to file
    out<<ss.str();
    error = "File saved successfully.";
    out.close();
}