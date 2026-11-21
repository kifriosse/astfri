#ifndef ASTFRI_SERVICE_SHM_HEADER_HPP
#define ASTFRI_SERVICE_SHM_HEADER_HPP

#include <cstddef>
#include <cstdint>

namespace astfri::service::shm {

enum class State : std::uint32_t
{
    Free     = 0,
    Request  = 1,
    Response = 2
};

enum class Result : std::uint32_t
{
    Ok       = 0,
    Error    = 1,
    TooLarge = 2
};

struct Header
{
    std::uint32_t state;
    std::uint32_t heartbeat;
    std::uint32_t input_type;
    std::uint32_t output_type;
    std::uint32_t input_size;
    std::uint32_t config_size;
    std::uint32_t result;
    std::uint32_t output_size;
};

static_assert(sizeof(Header) == 32);

inline constexpr std::size_t data_offset = sizeof(Header);

} // namespace astfri::service::shm

#endif
