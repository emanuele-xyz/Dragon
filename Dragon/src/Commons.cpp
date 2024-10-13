#include <Dragon/pch.h>
#include <Dragon/Commons.h>

namespace Dragon
{
    std::string GetStrFromWStr(const std::wstring& wstr)
    {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter{};
        std::string str{ converter.to_bytes(wstr) };
        return str;
    }

    std::wstring GetWStrFromStr(const std::string& str)
    {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter{};
        std::wstring wstr{ converter.from_bytes(str) };
        return wstr;
    }
}