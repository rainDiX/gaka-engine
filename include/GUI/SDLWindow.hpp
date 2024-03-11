/*
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <SDL2/SDL.h>
#include <SDL_joystick.h>

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
  void checkJoystick();

 protected:
  SDL_Window* m_window = nullptr;
  SDL_Joystick* m_joystick = nullptr;
  WindowState m_state;
};

}  // namespace gk::gui
