/*
 * SPDX-License-Identifier: MIT
 */

#include "IO/AssetManager.hpp"

#include <fstream>

namespace gk::io {

AssetManager::AssetManager(const char* root_dir_path) {
    auto root_dir = std::filesystem::current_path();
    root_dir /= root_dir_path;
    m_rootDir = std::filesystem::path(std::move(root_dir));
}

std::unique_ptr<std::string> AssetManager::readString(const std::string& assetPath) const {
    auto asset_path = m_rootDir / assetPath;
    auto asset_file = std::ifstream(asset_path);
    auto size = std::filesystem::file_size(asset_path);
    auto out = std::make_unique<std::string>();

    if (asset_file.is_open()) {
        out->resize(size);
        asset_file.read(&(*out)[0], size);
    }
    return out;
}

std::expected<std::vector<char>, Error> AssetManager::readBinary(const std::string& assetPath) const noexcept {
    auto path = m_rootDir / assetPath;
    auto file = std::ifstream(path, std::ios::binary);
    size_t size = std::filesystem::file_size(path);
    std::vector<char> buffer(size);

    if (file.is_open()) {
        file.seekg(0);
        file.read(buffer.data(), size);
        file.close();
        return buffer;
    }

    return std::unexpected{IOError{}};
}

}  // namespace gk::io
