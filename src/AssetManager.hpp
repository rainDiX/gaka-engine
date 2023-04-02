/*
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <cstring>
#include <filesystem>
#include <memory>
#include <string>

class AssetManager {
   public:
    AssetManager(const char* root_dir);
    std::unique_ptr<std::string> readString(const std::string& assetPath) const;

   private:
    std::filesystem::path m_rootDir;
};
