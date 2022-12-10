module;

#undef APIENTRY
#define NOMINMAX
//#define VC_EXTRALEAN
//#define WIN32_LEAN_AND_MEAN
#define __SPECSTRINGS_STRICT_LEVEL 0
#include <Windows.h>

module Ultra.Platform.UI.WinAPI.Dialog;

import Ultra.Core.Application;

namespace Ultra {

// ToDo: Export these to String Utility
wstring ConvertChar2WChar(const string &source) {
    wchar_t buffer[256] = {};
    std::mbstowcs(buffer, source.c_str(), source.length());
    wstring result = buffer;
    //delete[] buffer;
    return result;
}
string ConvertWChar2Char(const wstring &source) {
    char buffer[256] = {};
    std::wcstombs(buffer, source.c_str(), source.length());
    string result = buffer;
    //delete[] buffer;
    return result;
}


string WinDialog::OpenFile(const char *filter) const {
    OPENFILENAME dialog;       // common dialog box structure
    WCHAR szFile[260] = { 0 };   // if using TCHAR macros

    // Initialize OPENFILENAME
    ZeroMemory(&dialog, sizeof(dialog));
    dialog.lStructSize = sizeof(dialog);
    dialog.hwndOwner = (HWND)Application::GetWindow().GetNativeWindow();
    dialog.lpstrFile = szFile;
    dialog.nMaxFile = sizeof(szFile);
    dialog.lpstrFilter = ConvertChar2WChar(filter).c_str();
    dialog.nFilterIndex = 1;
    dialog.lpstrFileTitle = NULL;
    dialog.nMaxFileTitle = 0;
    dialog.lpstrInitialDir = NULL;
    dialog.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

    if (GetOpenFileName(&dialog) == TRUE) {
        return ConvertWChar2Char(dialog.lpstrFile);
    }
    return std::string();
}

string WinDialog::SaveFile(const char *filter) const {
    OPENFILENAME dialog;       // common dialog box structure
    WCHAR szFile[260] = { 0 };   // if using TCHAR macros

     // Initialize OPENFILENAME
    ZeroMemory(&dialog, sizeof(dialog));
    dialog.lStructSize = sizeof(dialog);
    dialog.hwndOwner = (HWND)Application::GetWindow().GetNativeWindow();
    dialog.lpstrFile = szFile;
    dialog.nMaxFile = sizeof(szFile);
    dialog.lpstrFilter = ConvertChar2WChar(filter).c_str();
    dialog.nFilterIndex = 1;
    dialog.lpstrFileTitle = NULL;
    dialog.nMaxFileTitle = 0;
    dialog.lpstrInitialDir = NULL;
    dialog.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

    if (GetSaveFileName(&dialog) == TRUE) {
        return ConvertWChar2Char(dialog.lpstrFile);
    }
    return std::string();
}

}
