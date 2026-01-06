#include <libastfri-cs/impl/Registries.hpp>
#include <libastfri-cs/impl/util/utils.hpp>

#include <cmath>
#include <filesystem>

namespace astfri::csharp::util
{

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

bool almost_equal(const double a, const double b, const double epsilon)
{
    return std::fabs(a - b) < epsilon;
}

void split_namespace(
    std::stack<std::string>& scope_str,
    const std::string_view namespace_name
)
{
    const auto r_begin = std::make_reverse_iterator(namespace_name.end());
    const auto r_end   = std::make_reverse_iterator(namespace_name.begin());
    auto it            = r_begin;
    auto slice_end     = namespace_name.end();

    while (it != r_end)
    {
        if (*it == '.')
        {
            auto slice_start = it.base();
            scope_str.emplace(slice_start, slice_end);
            slice_end = slice_start - 1;
        }
        ++it;
    }
}

bool is_interface_name(const std::string& name)
{
    return name.size() >= 2 && name[0] == 'I' && std::isupper(name[1]);
}

std::string escape_string(const std::string_view str, const bool is_verbatim)
{
    std::string escaped_str;
    for (size_t i = 0; i < str.length(); ++i)
    {
        switch (const char c = str[i])
        {
        case '\n':
            escaped_str += "\\n";
            break;
        case '\t':
            escaped_str += "\\t";
            break;
        case '\r':
            escaped_str += "\\r";
            break;
        case '\"':
        {
            if (! is_verbatim)
            {
                escaped_str += "\\\"";
                break;
            }

            const size_t next = i + 1;
            if (next < str.length() && str.at(next) == '\"')
            {
                escaped_str += "\\\"";
                i += 1;
            }
            break;
        }
        case '\'':
            escaped_str += "\\\'";
            break;
        case '\\':
            escaped_str += "\\\\";
            break;
        case '\0':
            escaped_str += "\\0";
            break;
        case '\a':
            escaped_str += "\\a";
            break;
        case '\b':
            escaped_str += "\\b";
            break;
        case '\f':
            escaped_str += "\\f";
            break;
        case '\v':
            escaped_str += "\\v";
            break;
        default:
            escaped_str += c;
            break;
        }
    }
    return escaped_str;
}

} // namespace astfri::csharp::util