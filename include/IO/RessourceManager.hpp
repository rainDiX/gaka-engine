/*
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <expected>
#include <filesystem>
#include <span>
#include <string>
#include <variant>
#include <vector>

namespace gk::io {

struct NotFoundError {};
struct IOError {};

struct Image {
  std::span<std::byte> pixels;
  unsigned width = 0;
  unsigned height = 0;
};

using Error = std::variant<NotFoundError, IOError>;
class RessourceManager {
 public:
  RessourceManager(const char* root_dir);
  std::expected<std::string, Error> readString(const std::string& assetPath) const noexcept;
  std::expected<std::vector<char>, Error> readBinary(const std::string& assetPath) const noexcept;
  std::expected<Image, Error> readImage(const std::string& assetPath) const noexcept;

 private:
  std::filesystem::path m_rootDir;
};
}  // namespace gk::io
