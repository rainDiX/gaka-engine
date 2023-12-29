/*
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <SDL2/SDL.h>

#include "GUI/Window.hpp"

namespace gk::gui {

class SDLWindow : public Window {
 public:
  virtual ~SDLWindow() {}
  void toggleMouseLock() override;
  const WindowState& state() const override;
  const std::string_view title() const override;
  void setTitle(const std::string& title) override;
  void update() override;

 protected:
  SDL_Window* m_window = nullptr;
  WindowState m_state;
};

}  // namespace gk::gui
