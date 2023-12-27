/*
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <expected>
#include <filesystem>
#include <memory>
#include <string>
#include <variant>
#include <vector>

namespace gk::io {

struct IOError {};

using Error = std::variant<IOError>;
class AssetManager {
   public:
    AssetManager(const char* root_dir);
    std::unique_ptr<std::string> readString(const std::string& assetPath) const;
    std::expected<std::vector<char>, Error> readBinary(const std::string& assetPath) const noexcept;

   private:
    std::filesystem::path m_rootDir;
};
}  // namespace gk::io
