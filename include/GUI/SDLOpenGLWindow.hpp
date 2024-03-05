/*
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "GUI/SDLWindow.hpp"
#include "GUI/Window.hpp"

namespace gk::gui {
class SDLOpenGLWindow : public SDLWindow {
 public:
  SDLOpenGLWindow(const std::string& title, uint32_t width = 1280, uint32_t height = 720,
                  bool fullscreen = false, gfx::VSyncMode vsync = gfx::VSyncMode::eDisabled);
  SDLOpenGLWindow() = delete;
  ~SDLOpenGLWindow();

  SDLOpenGLWindow(const SDLWindow& other) = delete;
  SDLOpenGLWindow& operator=(const SDLOpenGLWindow& other) = delete;

  void update() override;

 private:
  int getMaxMultisampleSamples();

 private:
  SDL_GLContext m_context;
};

}  // namespace gk::gui
