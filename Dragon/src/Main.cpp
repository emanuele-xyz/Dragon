#include <Dragon/pch.h>
#include <Dragon/Win32Utils.h>
#include <Dragon/Commons.h>
#include <Dragon/Dragon.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, PSTR, int)
{
    try
    {
        Dragon_Check(SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_SYSTEM_AWARE));
        Dragon::Run();
    }
    catch (const Dragon::Error& e)
    {
        MessageBox(0, e.what(), "Error!", MB_ICONERROR | MB_OK);
    }

    return 0;
}
