#include "File.h"

#include <fstream>

File::File(std::filesystem::path path):  m_Name{}, m_Path{path},m_OriginalContent{}{

    std::ifstream fileStream(path, std::ios::in | std::ios::binary);
    const auto sz = std::filesystem::file_size(path);

    std::string wholeFile(sz, '\0');
    fileStream.read(wholeFile.data(), sz);

    m_OriginalContent = std::move(wholeFile);
    m_Name = path.filename();
}
