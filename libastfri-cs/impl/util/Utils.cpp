#include <libastfri-cs/impl/util/Common.hpp>
#include <libastfri-cs/impl/util/Utils.hpp>

#include <cctype>
#include <cmath>
#include <deque>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>

namespace astfri::csharp::util
{

size_t StringHash::operator()(const std::string_view str) const noexcept
{
    return std::hash<std::string_view>()(str);
}

size_t StringHash::operator()(const std::string& str) const noexcept
{
    return std::hash<std::string>()(str);
}

IntSuffix get_suffix_type(const std::string_view suffix)
{
    if (suffix.empty())
        return IntSuffix::None;

    const char first  = static_cast<char>(std::tolower(suffix[0]));
    const char second = static_cast<char>(
        suffix.length() == 2 ? std::tolower(suffix[1]) : '\0'
    );

    // considering that we can't get stuff like UU or LL or L1 or U1
    if ((first == 'u' && second == 'l') || (first == 'l' && second == 'u'))
    {
        return IntSuffix::UL;
    }
    if (first == 'l' || first == 'u')
    {
        return first == 'l' ? IntSuffix::L : IntSuffix::U;
    }
    if (second == 'l' || second == 'u')
    {
        return second == 'l' ? IntSuffix::L : IntSuffix::U;
    }
    return IntSuffix::None;
}

void split_namespace(
    std::deque<std::string>& qualifs,
    const std::string_view nmsQualif
)
{
    if (nmsQualif.empty())
        return;

    size_t end = nmsQualif.length();
    while (true)
    {
        const size_t dotPos = nmsQualif.find_last_of('.', end - 1);
        if (dotPos == std::string::npos)
        {
            qualifs.emplace_front(nmsQualif.substr(0, end));
            break;
        }
        qualifs.emplace_front(nmsQualif.substr(dotPos + 1, end - (dotPos + 1)));
        end = dotPos;
    }
}

bool is_interface_name(const std::string_view name)
{
    return name.size() >= 2 && name[0] == 'I' && std::isupper(name[1]);
}

std::string escape_string(const std::string_view str, const bool isVerbatim)
{
    std::string escapedStr;
    for (size_t i = 0; i < str.length(); ++i)
    {
        switch (const char c = str[i])
        {
        case '\n':
            escapedStr += "\\n";
            break;
        case '\t':
            escapedStr += "\\t";
            break;
        case '\r':
            escapedStr += "\\r";
            break;
        case '\"':
        {
            if (! isVerbatim)
            {
                escapedStr += "\\\"";
                break;
            }

            const size_t next = i + 1;
            if (next < str.length() && str.at(next) == '\"')
            {
                escapedStr += "\\\"";
                i += 1;
            }
            break;
        }
        case '\'':
            escapedStr += "\\\'";
            break;
        case '\\':
            escapedStr += "\\\\";
            break;
        case '\0':
            escapedStr += "\\0";
            break;
        case '\a':
            escapedStr += "\\a";
            break;
        case '\b':
            escapedStr += "\\b";
            break;
        case '\f':
            escapedStr += "\\f";
            break;
        case '\v':
            escapedStr += "\\v";
            break;
        default:
            escapedStr += c;
            break;
        }
    }
    return escapedStr;
}

std::optional<TypeKind> get_type_kind(const std::string_view typeKind)
{
    using enum TypeKind;
    static const std::unordered_map<std::string_view, TypeKind> typeKindMap = {
        {"Class",        Class    },
        {"Struct",       Class    },
        {"RecordStruct", Record   },
        {"RecordClass",  Record   },
        {"Interface",    Interface},
        {"Enum",         Enum     },
        {"Delegate",     Delegate },
        {"Primitive",    Primitive}
    };
    const auto it = typeKindMap.find(typeKind);
    if (it != typeKindMap.end())
        return it->second;

    return {};
}

} // namespace astfri::csharp::util