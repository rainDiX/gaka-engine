/**
 * SPDX-License-Identifier: MIT
 */

// SDL2
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_mouse.h>

#include <glm/fwd.hpp>
#include <iostream>
#include <memory>

#include "Animation/Skeleton.hpp"
#include "Animation/SkinnedMesh.hpp"
#include "GFX/Enums.hpp"
#include "GFX/FlyingCamera.hpp"
#include "GFX/Material.hpp"
#include "GFX/MaterialParameters.hpp"
#include "GUI/SDLOpenGLWindow.hpp"
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

    skinningDemo();
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
            (*cameraNode)->camera().setFov((*cameraNode)->camera().fov() + event.wheel.y);
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
    float increment = .5f;
    float angle = .0f;

    while (running) {
      // input
      // -----
      processInput(running);

      angle += increment;
      if (angle == 90.f) {
        increment = -.5f;
      } else if (angle == 0.f) {
        increment = .5f;
      }
      m_skeleton->moveBone(2, increment, glm::vec3(0, 0, 1));
      m_renderer->renderScene();

      m_window->update();
    }
  }

 private:
  std::unique_ptr<gk::animation::Skeleton> makeSkeleton() {
    auto skel = std::make_unique<gk::animation::Skeleton>(glm::mat4(1), glm::vec3(0, 0, 0));
    int bone1 = skel->addBone(glm::mat4(1.f), glm::vec3(0.5, 0, 0), 0);
    skel->addBone(glm::mat4(1.f), glm::vec3(1, 0, 0), bone1);
    m_skeleton = skel.get();
    return skel;
  }

  std::unique_ptr<gk::animation::SkinnedMesh> makeCylinder(glm::vec3 base = glm::vec3(0, 0, 0),
                                                           glm::vec3 axis = glm::vec3(1, 0, 0),
                                                           float radius = .5f, float length = 3.,
                                                           unsigned subdiv1 = 64,
                                                           unsigned subdiv2 = 512) {
    std::vector<gk::animation::SkinnedMesh::Vertex> vertices;
    std::vector<unsigned> indices;
    glm::vec3 x = glm::vec3(0, 1, 0);  // orthogonal(axis);
    glm::vec3 y = cross(axis, x);

    for (unsigned i = 0; i < subdiv2; i++) {
      float offset = float(i) / float(subdiv2 - 1);
      float offset2 = (offset - 0.5) * length;
      for (unsigned j = 0; j < subdiv1; j++) {
        float angle = 2. * glm::pi<float>() * float(j) / float(subdiv1);
        gk::animation::SkinnedMesh::Vertex nv;
        nv.position = base + offset2 * axis + radius * cos(angle) * x + radius * sin(angle) * y;
        nv.normal = normalize(cos(angle) * x + sin(angle) * y);
        nv.boneCount = 3;
        nv.boneIdx = {0, 1, 2, -1};

        float w1 = 1.f - (float(i) / float(subdiv2));
        float w3 = (float(i) / float(subdiv2));
        float w2 = 0.f;
        if (i <= (subdiv2 / 2)) {
          w2 = float(i) / float(subdiv2 / 2);
        } else {
          w2 = float(subdiv2 / 2) / float(i);
        }

        float total = pow(w1, 2) + pow(w2, 2) + pow(w3, 2);
        w1 = pow(w1, 2) / total;
        w2 = pow(w2, 2) / total;
        w3 = pow(w3, 2) / total;
        nv.boneWeights = {w1, w2, w3, 0.0f};
        vertices.push_back(nv);
      }
    }

    for (unsigned int i = 0; i < subdiv2 - 1; i++) {
      for (unsigned int j = 0; j < subdiv1; j++) {
        indices.insert(indices.end(), {i * subdiv1 + j, i * subdiv1 + (j + 1) % subdiv1,
                                       i * subdiv1 + j + subdiv1});
        indices.insert(indices.end(), {i * subdiv1 + (j + 1) % subdiv1, i * subdiv1 + j + subdiv1,
                                       i * subdiv1 + (j + 1) % subdiv1 + subdiv1});
      }
    }
    return std::make_unique<gk::animation::SkinnedMesh>(vertices, indices);
  }

  void skinningDemo() {
    auto& scene = m_renderer->getScene();

    // add camera
    gk::gfx::FlyingCamera camera{glm::vec3(0.0f, 0.5f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f)};

    scene.setActiveCamera(scene.addCamera(std::move(camera)));

    auto material = gk::gfx::createPhongMaterialAnimated(*m_ressourceManager);
    long materialId = scene.addMaterial(std::move(material));

    auto cylinder = makeCylinder();

    auto copper = std::make_unique<gk::gfx::PhongMaterialParamsAnimated>(makeSkeleton());

    long copperId = scene.addMaterialParameter(std::move(copper));

    auto cylinderId = scene.addMesh(*cylinder, materialId);

    if (cylinderId.has_value()) {
      scene.connect(scene.rootId(), *cylinderId);
      scene.connect(*cylinderId, materialId);
      scene.connect(*cylinderId, copperId);
    }

    long light1 =
        scene.addLight({glm::vec3(1.0, 1.0, 1.0), 1.0, 20.0, 1.0}, glm::vec3(0.0, 5.0, 5.0));
    scene.connect(scene.rootId(), light1);
  }

  std::unique_ptr<gk::gui::SDLOpenGLWindow> m_window;
  std::shared_ptr<gk::io::RessourceManager> m_ressourceManager;
  std::unique_ptr<gk::rendering::Renderer> m_renderer;
  gk::animation::Skeleton* m_skeleton{nullptr};
};

int main() {
  Application app{};

  app.runMainLoop();

  return 0;
}
