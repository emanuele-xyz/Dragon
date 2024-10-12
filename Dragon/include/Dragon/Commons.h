#pragma once

#include <stdexcept>

#if defined(_DEBUG)
#define Dragon_Error(message) __debugbreak()
#else
#define Dragon_Error(message) throw ::Dragon::Error{ __FILE__, __LINE__, __FUNCTION__, message }
#endif

#define Dragon_Check(p) do { if (!(p)) { Dragon_Error("predicate '" #p "' failed"); } } while (false)

namespace Dragon
{
    class Error : public std::runtime_error
    {
    public:
        Error(const std::string& file, int line, const std::string& function, const std::string& message)
            : std::runtime_error{ std::format("{}({}): in function '{}': {}", file, line, function, message) }
        {}
    };
}
