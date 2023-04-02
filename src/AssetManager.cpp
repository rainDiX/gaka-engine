/*
 * SPDX-License-Identifier: MIT
 */

#include "AssetManager.hpp"

#include <filesystem>
#include <fstream>
#include <memory>
#include <string>

AssetManager::AssetManager(const char* root_dir_path) {
    auto root_dir = std::filesystem::current_path();
    root_dir /= root_dir_path;
    m_rootDir = std::filesystem::path(std::move(root_dir));
}

#include <iostream>

std::unique_ptr<std::string> AssetManager::readString(
    const std::string& assetPath) const {
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