/*
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <expected>
#include <filesystem>
#include <string>
#include <variant>
#include <vector>

namespace gk::io {

struct NotFoundError {};
struct IOError {};

using Error = std::variant<NotFoundError, IOError>;
class AssetManager {
   public:
    AssetManager(const char* root_dir);
    std::expected<std::string, Error> readString(const std::string& assetPath) const noexcept;
    std::expected<std::vector<char>, Error> readBinary(const std::string& assetPath) const noexcept;

   private:
    std::filesystem::path m_rootDir;
};
}  // namespace gk::io
