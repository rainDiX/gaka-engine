/*
 * SPDX-License-Identifier: MIT
 */

#include "IO/RessourceManager.hpp"

#include <sail-c++/image.h>

#include <filesystem>
#include <fstream>

namespace gk::io {

RessourceManager::RessourceManager(const char* root_dir_path) {
  auto root_dir = std::filesystem::current_path();
  root_dir /= root_dir_path;
  m_rootDir = std::filesystem::path(std::move(root_dir));
}

std::expected<std::string, Error> RessourceManager::readString(
    const std::string& assetPath) const noexcept {
  auto path = m_rootDir / assetPath;
  auto asset_file = std::ifstream(path);

  if (!std::filesystem::exists(path)) {
    return std::unexpected{NotFoundError{}};
  }

  auto size = std::filesystem::file_size(path);
  std::string buffer;
  buffer.resize(size);

  if (asset_file.is_open()) {
    asset_file.read(&buffer[0], size);
    return buffer;
  }
  return std::unexpected{IOError{}};
}

std::expected<std::vector<char>, Error> RessourceManager::readBinary(
    const std::string& assetPath) const noexcept {
  auto path = m_rootDir / assetPath;
  auto file = std::ifstream(path, std::ios::binary);
  if (!std::filesystem::exists(path)) {
    return std::unexpected{NotFoundError{}};
  }
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

std::expected<Image, Error> RessourceManager::readImage(
    const std::string& assetPath) const noexcept {
  auto path = m_rootDir / assetPath;
  auto file = std::ifstream(path, std::ios::binary);
  if (!std::filesystem::exists(path)) {
    return std::unexpected{NotFoundError{}};
  }
  sail::image image(path);

  if (image.is_valid()) {
    return Image{std::move(std::span<std::byte>{static_cast<std::byte*>(image.pixels()),
                                                image.pixels_size()}),
                 image.width(), image.height()};
  }
  return std::unexpected{IOError{}};
}

}  // namespace gk::io
