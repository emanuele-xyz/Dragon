#pragma once

#include <stdexcept>
#include <string>
#include <vector>
#include <filesystem>

#define DRAGON_PROJECT_FILE_NAME "dragon.xml"

#if defined(_DEBUG)
#define Dragon_Error(message) __debugbreak()
#else
#define Dragon_Error(message) throw ::Dragon::Error{ __FILE__, __LINE__, __FUNCTION__, message }
#endif

#define Dragon_Check(p) do { if (!(p)) { Dragon_Error("predicate '" #p "' failed"); } } while (false)

#define Dragon_CountOf(a) sizeof(a) / sizeof((a)[0])
#define Dragon_Unreachable() Dragon_Error("Unreachable code path")

namespace Dragon
{
    class Error : public std::runtime_error
    {
    public:
        Error(const std::string& file, int line, const std::string& function, const std::string& message)
            : std::runtime_error{ std::format("{}({}): in function '{}': {}", file, line, function, message) }
        {}
    };

    std::string GetStrFromWStr(const std::wstring& wstr);
    std::wstring GetWStrFromStr(const std::string& str);

    bool IsEmptyFolder(const std::string& path);

    std::vector<char> LoadFile(const std::filesystem::path& path);
}
