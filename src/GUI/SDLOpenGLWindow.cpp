/**
 * SPDX-License-Identifier: MIT
 */

#include "GUI/SDLOpenGLWindow.hpp"

#include <SDL2/SDL_video.h>

#include <iostream>

#include "GUI/SDLWindow.hpp"

namespace gk::gui {

SDLOpenGLWindow::SDLOpenGLWindow(const std::string& title, int width, int height, bool fullscreen,
                                 bool vsync) {
  m_state = {width, height, fullscreen, vsync, false, 0, 0};
  unsigned long flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
  if (fullscreen) flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;

  // SDL: initialize and configure
  // ------------------------------
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    std::cerr << "failed to init SDL2: " << SDL_GetError() << '\n';
    throw std::runtime_error("Failed to init SDL2");
  }

  // Enable v-sync (set 1 to enable, 0 to disable)
  SDL_GL_SetSwapInterval(vsync ? SDL_TRUE : SDL_FALSE);

  // Request at least 32-bit color
  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

  // Request a double-buffered, OpenGL 4.6 (or higher) core profile
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, getMaxMultisampleSamples());

  SDL_SetHint(SDL_HINT_MOUSE_RELATIVE_WARP_MOTION, "1");

  // SDL window creation
  // --------------------
  m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              m_state.width, m_state.height, flags);

  if (m_window == NULL) {
    SDL_Quit();
    throw std::runtime_error("Failed to create SDL2 window");
  }

  m_context = SDL_GL_CreateContext(m_window);
  if (m_context == NULL) {
    SDL_DestroyWindow(m_window);
    SDL_Quit();
    std::cerr << SDL_GetError() << '\n';
    throw std::runtime_error("failed to create OpenGL context");
  }
}

SDLOpenGLWindow::~SDLOpenGLWindow() {
  SDL_GL_DeleteContext(m_context);
  SDL_DestroyWindow(m_window);
  SDL_Quit();
}

int SDLOpenGLWindow::getMaxMultisampleSamples() {
  int maxSamples = 1;
  bool maxFound = false;
  while (!maxFound) {
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, maxSamples * 2);
    SDL_Window* window =
        SDL_CreateWindow("HIDDEN", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1000, 1000,
                         SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN);
    maxFound = window == NULL;
    if (!maxFound) {
      SDL_DestroyWindow(window);
      maxSamples *= 2;
    }
  }
  return maxSamples;
}

void SDLOpenGLWindow::update() {
  SDLWindow::update();
  SDL_GL_SwapWindow(m_window);
}

}  // namespace gk::gui
