/**
 * SPDX-License-Identifier: MIT
 */

// GL
#include <GL/glew.h>

// SDL2
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_mouse.h>

#include <cstddef>
#include <memory>
#include <random>
#include <vector>

#include "GFX/OpenGL/GLShaderProgram.hpp"
#include "GUI/SDLOpenGLWindow.hpp"
#include "Geometry/Curves.hpp"
#include "Geometry/Mesh.hpp"
#include "Geometry/Surface.hpp"
#include "IO/RessourceManager.hpp"
#include "Rendering/Material.hpp"
#include "Rendering/RenderObject.hpp"
#include "Rendering/Renderer.hpp"
#include "Rendering/Scene.hpp"

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

void bezierDemo(std::shared_ptr<gk::gfx::gl::GLShaderProgram> program, gk::rendering::Scene& scene) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);

    auto bezier = gk::geometry::Bezier<4>(glm::vec3(0, 0, 0), glm::vec3(3, 0, 0), 100);

    for (int i = 1; i < 3; ++i) {
        bezier[i] = glm::vec3(i, 2 * dis(gen), 2 * dis(gen));
    }

    auto curveMesh = gk::geometry::Mesh{std::vector<gk::geometry::Vertex>(), bezier.indices()};

    for (size_t i = 0; i < bezier.curve().size(); ++i) {
        float u = float(i) / float(bezier.curve().size());
        curveMesh.vertices.push_back(
            gk::geometry::Vertex{bezier.curve()[i], glm::vec3(0.0, 0.0, 0.0), glm::vec2(u, u)});
    }

    auto copper = std::make_shared<Material>();
    copper->name = std::string("copper");
    copper->ambient = glm::vec3(0.19125, 0.0735, 0.0225);
    copper->diffuse = glm::vec3(0.7038, 0.27048, 0.0828);
    copper->specular = glm::vec3(0.256777, 0.137622, 0.086014);
    copper->shininess = 0.1;

    auto ctrlCurve = bezier.ctrlCurve();
    auto ctrlMesh = gk::geometry::Mesh{std::vector<gk::geometry::Vertex>(), ctrlCurve->indices()};
    for (size_t i = 0; i < ctrlCurve->curve().size(); ++i) {
        float u = float(i) / float(bezier.curve().size());
        ctrlMesh.vertices.push_back(
            gk::geometry::Vertex{ctrlCurve->curve()[i], glm::vec3(0.0, 0.0, 0.0), glm::vec2(1.0 - u, 1.0 - u)});
    }

    auto curveObj = std::make_unique<gk::rendering::RenderObject>(curveMesh, program, std::vector<gk::gfx::gl::GLTexture>(), copper);
    auto ctrlObj = std::make_unique<gk::rendering::RenderObject>(ctrlMesh, program, std::vector<gk::gfx::gl::GLTexture>(), copper);

    curveObj->mesh().setDrawingMode(gk::gfx::gl::LINES);
    ctrlObj->mesh().setDrawingMode(gk::gfx::gl::LINES);

    scene.addObject("control curve", std::move(ctrlObj));
    scene.addObject("bezier Curve", std::move(curveObj));
}

void surfaceDemo(std::shared_ptr<gk::gfx::gl::GLShaderProgram> program, gk::rendering::Scene& scene) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);

    auto ctrl_grid = std::array<glm::vec3, 4 * 4>();
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            ctrl_grid[i * 4 + j] = glm::vec3(i, 4 * dis(gen), j);
        }
    }

    auto surface = gk::geometry::BezierSurface<4, 4>(std::move(ctrl_grid), 1005);

    auto copper = std::make_shared<Material>();
    copper->name = std::string("copper");
    copper->ambient = glm::vec3(0.19125, 0.0735, 0.0225);
    copper->diffuse = glm::vec3(0.7038, 0.27048, 0.0828);
    copper->specular = glm::vec3(0.256777, 0.137622, 0.086014);
    copper->shininess = 0.1;

    auto surfaceObj = std::make_unique<gk::rendering::RenderObject>(surface.mesh(), program, std::vector<gk::gfx::gl::GLTexture>(), copper);

    scene.addObject("surface", std::move(surfaceObj));
}

class Application {
   public:
    Application() {
        m_window = std::make_unique<gk::gui::SDLOpenGLWindow>("Gaka Demo", SCR_WIDTH, SCR_HEIGHT, false, true);
        m_assetManager = std::make_shared<gk::io::RessourceManager>("."); // TODO improve
        m_renderer = std::make_unique<gk::rendering::Renderer>(m_assetManager);
        m_renderer->resize(SCR_WIDTH, SCR_HEIGHT);

        auto line = m_renderer->getProgram("line");
        auto normal = m_renderer->getProgram("normal");
        auto lambertian = m_renderer->getProgram("lambertian");
        auto phong = m_renderer->getProgram("phong");

        auto& scene = m_renderer->getScene();
        // bezierDemo(line, scene);
        surfaceDemo(lambertian, scene);

        scene.addPointLight({glm::vec3(1.0, 1.0, 1.0), 1.0, 20.0, 5.0}, glm::vec3(0.0, 10.0, 0.0));
        scene.addPointLight({glm::vec3(1.0, 1.0, 1.0), 1.0, 20.0, 5.0}, glm::vec3(10.0, 0.0, 0.0));
        scene.addPointLight({glm::vec3(1.0, 1.0, 1.0), 1.0, 20.0, 5.0}, glm::vec3(0.0, 10.0, 10.0));
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
                        case SDLK_LEFT:
                            m_renderer->getScene().camera().strafeBy(0.5);
                            break;
                        case SDLK_RIGHT:
                            m_renderer->getScene().camera().strafeBy(-0.5);
                            break;
                        case SDLK_UP:
                            m_renderer->getScene().camera().moveUp(0.5);
                            break;
                        case SDLK_DOWN:
                            m_renderer->getScene().camera().moveUp(-0.5);
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
                    m_renderer->getScene().camera().moveBy(event.wheel.y * 0.5);
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
            auto& camera = m_renderer->getScene().camera();
            camera.rotateRight(0.15 * deltaX);
            camera.rotateUp(0.15 * deltaY);
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
    std::unique_ptr<gk::gui::SDLOpenGLWindow> m_window;
    std::shared_ptr<gk::io::RessourceManager> m_assetManager = nullptr;
    std::unique_ptr<gk::rendering::Renderer> m_renderer = nullptr;
};

int main() {
    Application app{};

    app.runMainLoop();

    return 0;
}
