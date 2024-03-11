/*
 * SPDX-License-Identifier: MIT
 */

#include "GUI/SDLWindow.hpp"

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_video.h>

#include <iostream>
#include <string>

namespace gk::gui {

void SDLWindow::toggleMouseLock() {
  m_state.mouseLocked = !m_state.mouseLocked;
  const auto locked = m_state.mouseLocked ? SDL_TRUE : SDL_FALSE;
  SDL_ShowCursor(m_state.mouseLocked ? SDL_DISABLE : SDL_ENABLE);
  SDL_CaptureMouse(locked);
  SDL_SetRelativeMouseMode(locked);
  SDL_SetWindowGrab(m_window, locked);
  if (m_state.mouseLocked) {
    SDL_WarpMouseInWindow(m_window, m_state.width / 2, m_state.height / 2);
  }
  SDL_GetRelativeMouseState(&m_state.mouseX, &m_state.mouseY);
}

const WindowState& SDLWindow::state() const { return m_state; }

const std::string_view SDLWindow::title() const {
  return std::string_view{SDL_GetWindowTitle(m_window)};
}

void SDLWindow::setTitle(const std::string& title) { SDL_SetWindowTitle(m_window, title.c_str()); }

void SDLWindow::update() {
  if (m_state.mouseLocked) {
    SDL_WarpMouseInWindow(m_window, m_state.width / 2, m_state.height / 2);
  }
  SDL_GetRelativeMouseState(&m_state.mouseX, &m_state.mouseY);
}

void SDLWindow::checkJoystick() {
  // Check for joysticks
  if (SDL_NumJoysticks() >= 1) {
    // Load joystick
    m_joystick = SDL_JoystickOpen(0);
    if (m_joystick == nullptr) {
      std::cerr << "Warning: Unable to open game controller! SDL Error: \n" << SDL_GetError();
    }
  }
}

}  // namespace gk::gui
