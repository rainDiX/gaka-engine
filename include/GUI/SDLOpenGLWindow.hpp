/*
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "GUI/SDLWindow.hpp"

namespace gk::gui {
class SDLOpenGLWindow : public SDLWindow {
 public:
  SDLOpenGLWindow(const std::string& title, int width = 1280, int height = 720,
                  bool fullscreen = false, bool vsync = false);
  SDLOpenGLWindow() = delete;
  ~SDLOpenGLWindow();

  SDLOpenGLWindow(const SDLWindow& other) = delete;
  SDLOpenGLWindow& operator=(const SDLOpenGLWindow& other) = delete;

  void update() override;

 private:
  int getMaxMultisampleSamples();

 private:
  SDL_Window* m_window;
  SDL_GLContext m_context;
  WindowState m_state;
};

}  // namespace gk::gui
