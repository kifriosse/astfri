#include "shm_server.hpp"

#include "shm_header.hpp"

#include <llvm/Support/FileSystem.h>

#include <cstdint>
#include <iostream>
#include <system_error>

namespace astfri::service {

bool run_shm_server(const std::filesystem::path& shared_memory_path)
{
    namespace fs = llvm::sys::fs;

    std::uint64_t shared_memory_size = 0;
    if (fs::file_size(shared_memory_path.string(), shared_memory_size))
    {
        std::cerr << "Cannot read size of " << shared_memory_path.string() << "\n";
        return false;
    }

    auto file = fs::openNativeFileForReadWrite(shared_memory_path.string(), fs::CD_OpenExisting, fs::OF_None);
    if (! file)
    {
        std::cerr << llvm::toString(file.takeError()) << "\n";
        return false;
    }

    std::error_code ec;
    fs::mapped_file_region shared_memory(*file, fs::mapped_file_region::readwrite, shared_memory_size, 0, ec);
    fs::closeFile(*file);
    if (ec || shared_memory_size < shm::data_offset)
    {
        std::cerr << "Cannot map " << shared_memory_path.string() << "\n";
        return false;
    }

    return true;
}

} // namespace astfri::service
