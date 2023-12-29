#if defined(_WIN32)
#include <windows.h>
#include <commdlg.h>
#elif defined(__linux__)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif

const int screenWidth = 800;
const int screenHeight = 450;

#if defined(_WIN32)

wchar_t* OpenFileDialog()
{
    OPENFILENAMEW ofn;
    wchar_t fileName[MAX_PATH] = L"";

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = sizeof(fileName) / sizeof(wchar_t);
    ofn.lpstrFilter = L"All Files\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrInitialDir = NULL;
    ofn.lpstrFileTitle = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileNameW(&ofn) == TRUE)
    {
        wchar_t* result = (wchar_t*)malloc(sizeof(wchar_t) * (wcslen(fileName) + 1));
        wcscpy(result, fileName);
        return result;
    }

    return NULL;
}
#elif defined(__linux__)
char* OpenFileDialog()
{
    char command[256] = "zenity --file-selection";

    FILE* file = popen(command, "r");
    if (!file)
    {
        perror("popen");
        return NULL;
    }

    char buffer[256];
    fgets(buffer, sizeof(buffer), file);
    strtok(buffer, "\n");

    pclose(file);

    if (strlen(buffer) > 0)
    {
        char* result = (char*)malloc(strlen(buffer) + 1);
        strcpy(result, buffer);
        return result;
    }

    return NULL;
}
#endif
