/**
 * SPDX-License-Identifier: MIT
 */

#include <memory>

#include "GFX/Enums.hpp"
#include "GUI/SDLVulkanWindow.hpp"
#include "IO/RessourceManager.hpp"

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

class Application {
 public:
  Application() {
    m_window = std::make_unique<gk::gui::SDLVulkanWindow>("Gaka Vulkan Demo", SCR_WIDTH, SCR_HEIGHT,
                                                          false, gk::gfx::VSyncMode::eTripleBuffering);
    m_ressourceManager = std::make_shared<gk::io::RessourceManager>("./assets");
  }

  ~Application() = default;

  void processInput(bool& running) {
    SDL_Event event = {0};
    auto& state = m_window->state();

    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT:
          running = false;
          break;
        case SDL_KEYDOWN:
          switch (event.key.keysym.sym) {
            case SDLK_q:
              running = false;
              break;
            case SDLK_ESCAPE:
              if (state.mouseLocked) {
                m_window->toggleMouseLock();
              } else {
                running = false;
              }
              break;
          }
          break;
        case SDL_MOUSEBUTTONDOWN:
          switch (event.button.button) {
            case SDL_BUTTON_LEFT:
              m_window->toggleMouseLock();
              break;
          }
          break;
        case SDL_WINDOWEVENT:
          // if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
          //     m_renderer->resize(event.window.data1, event.window.data2);
          // }
          break;
      }
    }

    // Get mouse state and calculate delta
    int mouseX, mouseY;
    SDL_GetRelativeMouseState(&mouseX, &mouseY);
  }

  void runMainLoop() {
    bool running = true;
    while (running) {
      processInput(running);

      m_window->update();
    }
  }

 private:
  std::unique_ptr<gk::gui::SDLVulkanWindow> m_window;
  std::shared_ptr<gk::io::RessourceManager> m_ressourceManager = nullptr;
  // std::unique_ptr<gk::rendering::Renderer> m_renderer = nullptr;
};

int main() {
  Application app{};

  app.runMainLoop();

  return 0;
}
