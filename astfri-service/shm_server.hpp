#ifndef ASTFRI_SERVICE_SHM_SERVER_HPP
#define ASTFRI_SERVICE_SHM_SERVER_HPP

#include <filesystem>

namespace astfri::service {

bool run_shm_server(const std::filesystem::path& shared_memory_path);

} // namespace astfri::service

#endif
