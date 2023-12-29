/*
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <string_view>

namespace gk::gui {
struct WindowState {
  int width;
  int height;
  bool fullscreen;
  bool vsync;
  bool mouseLocked;
  int mouseX;
  int mouseY;
};

class Window {
 public:
  virtual ~Window() {}
  virtual void toggleMouseLock() = 0;
  virtual const WindowState& state() const = 0;
  virtual const std::string_view title() const = 0;
  virtual void setTitle(const std::string& title) = 0;
  virtual void update() = 0;
};

}  // namespace gk::gui
