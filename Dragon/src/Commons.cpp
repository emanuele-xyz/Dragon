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

    bool IsEmptyFolder(const std::string& path)
    {
        bool is_directory{ std::filesystem::is_directory(path) };
        return is_directory && std::filesystem::is_empty(path);
    }

    std::vector<char> LoadFile(const std::filesystem::path& path)
    {
        Dragon_Check(std::filesystem::exists(path) && std::filesystem::is_regular_file(path));

        std::ifstream file{ path, std::ios::binary };
        Dragon_Check(file);

        auto size{ std::filesystem::file_size(path) };
        std::vector<char> buffer(size);
        file.read(buffer.data(), size);
        Dragon_Check(file);

        return buffer;
    }
}