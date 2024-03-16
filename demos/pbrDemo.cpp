/**
 * SPDX-License-Identifier: MIT
 */

// SDL2
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_mouse.h>

#include <memory>
#include <random>
#include <span>

#include "GFX/Enums.hpp"
#include "GFX/FlyingCamera.hpp"
#include "GFX/Material.hpp"
#include "GFX/MaterialParameters.hpp"
#include "GUI/SDLOpenGLWindow.hpp"
#include "Geometry/Surface.hpp"
#include "IO/RessourceManager.hpp"
#include "Rendering/Renderer.hpp"
#include "Rendering/Scene.hpp"

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

class Application {
 public:
  Application() {
    m_window = std::make_unique<gk::gui::SDLOpenGLWindow>("Gaka Demo", SCR_WIDTH, SCR_HEIGHT, false,
                                                          gk::gfx::VSyncMode::eDoubleBuffering);
    m_ressourceManager = std::make_shared<gk::io::RessourceManager>(".");  // TODO improve
    m_renderer = std::make_unique<gk::rendering::Renderer>(m_ressourceManager);
    m_renderer->resize(SCR_WIDTH, SCR_HEIGHT);

    surfaceDemo();
  }

  ~Application() = default;

  void processInput(bool& running) {
    SDL_Event event = {0};
    auto& state = m_window->state();
    auto cameraNode = m_renderer->getScene().activeCamera();

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
            case SDLK_LEFT:
              if (cameraNode.has_value()) {
                (*cameraNode)->camera().strafeBy(-0.5);
              }
              break;
            case SDLK_RIGHT:
              if (cameraNode.has_value()) {
                (*cameraNode)->camera().strafeBy(0.5);
              }
              break;
            case SDLK_UP:
              if (cameraNode.has_value()) {
                (*cameraNode)->camera().moveBy(0.5);
              }
              break;
            case SDLK_DOWN:
              if (cameraNode.has_value()) {
                (*cameraNode)->camera().moveBy(-0.5);
              }
              break;
            case SDLK_PAGEUP:
              if (cameraNode.has_value()) {
                (*cameraNode)->camera().moveUp(0.5);
              }
              break;
            case SDLK_PAGEDOWN:
              if (cameraNode.has_value()) {
                (*cameraNode)->camera().moveUp(-0.5);
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
        case SDL_MOUSEWHEEL:
          if (cameraNode.has_value()) {
            (*cameraNode)->camera().moveBy(event.wheel.y * 0.5);
          }
          break;
        case SDL_WINDOWEVENT:
          if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
            m_renderer->resize(event.window.data1, event.window.data2);
          }
          break;
      }
    }

    // Get mouse state and calculate delta
    int mouseX, mouseY;
    SDL_GetRelativeMouseState(&mouseX, &mouseY);
    int deltaX = 0, deltaY = 0;
    if (state.mouseLocked) {
      deltaX = mouseX - state.mouseX;
      deltaY = mouseY - state.mouseY;
    }

    // Handle mouse movement
    if (deltaX != 0 || deltaY != 0) {
      if (cameraNode.has_value()) {
        auto& camera = (*cameraNode)->camera();
        camera.yawMotion(0.15 * deltaX);
        camera.pitchMotion(0.15 * deltaY);
      }
    }
  }

  void runMainLoop() {
    bool running = true;
    while (running) {
      // input
      // -----
      processInput(running);

      m_renderer->renderScene();

      m_window->update();
    }
  }

 private:
  void surfaceDemo() {
    auto& scene = m_renderer->getScene();

    // add camera
    gk::gfx::FlyingCamera camera{glm::vec3(0.0f, 2.0f, 8.0f), glm::vec3(0.0f, 1.0f, 0.0f)};

    long cameraId = scene.addCamera(std::move(camera));

    scene.setActiveCamera(cameraId);

    auto material = gk::gfx::createMetallicRoughnessMaterial(*m_ressourceManager);
    long materialtId = scene.addMaterial(std::move(material));

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);

    auto ctrl_grid = std::array<glm::vec3, 4 * 4>();
    for (int i = 0; i < 4; ++i) {
      for (int j = 0; j < 4; ++j) {
        ctrl_grid[i * 4 + j] = glm::vec3(i, 4 * dis(gen), j);
      }
    }
    auto surface = gk::geometry::BezierSurface<4, 4>(std::move(ctrl_grid), 100);

    auto copper = std::make_unique<gk::gfx::MetallicRoughnessMaterialParams>(
        glm::vec4{0.7f, 0.1f, 0.8f, 1.f}, 0.4f, 0.4f);

    long copperId = scene.addMaterialParameter(std::move(copper));

    auto surfaceId = scene.addMesh(surface.mesh(), materialtId);

    if (surfaceId.has_value()) {
      scene.connect(scene.rootId(), *surfaceId);
      scene.connect(*surfaceId, materialtId);
      scene.connect(*surfaceId, copperId);

      // auto wallTex = m_ressourceManager->readImage("assets/wall.jpg");

      // if (wallTex) {
      //   auto img = *wallTex;
      //   long texId = scene.addTexture(img.pixels, img.width, img.height);
      //   scene.connect(*surfaceId, texId);
      // }
    }
    long light1 =
        scene.addLight({glm::vec3(1.0, 1.0, 1.0), 1.0, 20.0, 5.0}, glm::vec3(-5.0, 5.0, 0.0));
    long light2 =
        scene.addLight({glm::vec3(1.0, 1.0, 1.0), 1.0, 20.0, 5.0}, glm::vec3(5.0, 5.0, 0.0));
    long light3 =
        scene.addLight({glm::vec3(1.0, 1.0, 1.0), 1.0, 20.0, 5.0}, glm::vec3(0.0, 5.0, 5.0));
    scene.connect(scene.rootId(), light1);
    scene.connect(scene.rootId(), light2);
    scene.connect(scene.rootId(), light3);
  }

  std::unique_ptr<gk::gui::SDLOpenGLWindow> m_window;
  std::shared_ptr<gk::io::RessourceManager> m_ressourceManager;
  std::unique_ptr<gk::rendering::Renderer> m_renderer;
};

int main() {
  Application app{};

  app.runMainLoop();

  return 0;
}
