/**
 * SPDX-License-Identifier: MIT
 */

// GL
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_stdinc.h>
#include <glad/gl.h>

// SDL2
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_video.h>

#include <cstdlib>
#include <iostream>
#include <memory>
#include <random>
#include <vector>

#include "AssetManager.hpp"
#include "Geometry/Surface.hpp"
#include "Rendering/Material.hpp"
#include "Rendering/RenderObject.hpp"
#include "Rendering/Renderer.hpp"

struct WindowState {
    SDL_Window* window;
    int width;
    int height;
    bool mouseLocked;
    int mouseX;
    int mouseY;
};

// void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(WindowState& state, bool& running, Renderer& renderer);
void toggleMouseLock(bool& mouseLocked);
int getMaxMultisampleSamples();

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main() {
    bool fullscreen = false;
    bool vsync = true;

    unsigned long flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
    if (fullscreen) flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;

    // SDL: initialize and configure
    // ------------------------------
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "failed to init SDL2: " << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
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
    SDL_Window* window = SDL_CreateWindow("SDL2 OpenGL Demo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                          SCR_WIDTH, SCR_HEIGHT, flags);

    if (window == NULL) {
        std::cerr << "Failed to create SDL2 window" << std::endl;
        SDL_Quit();
        return EXIT_FAILURE;
    }

    // SDL_GLContext is an alias for "void*"
    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (context == NULL) {
        SDL_DestroyWindow(window);
        SDL_Quit();

        fprintf(stderr, "failed to create OpenGL context: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    WindowState state = {window, SCR_WIDTH, SCR_HEIGHT, false, 0, 0};

    auto assetManager = std::make_shared<AssetManager>("./assets");
    auto renderer = Renderer(SDL_GL_GetProcAddress, assetManager);

    renderer.resize(SCR_WIDTH, SCR_HEIGHT);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);

    auto ctrl_grid = std::array<glm::vec3, 4 * 4>();
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            ctrl_grid[i * 4 + j] = glm::vec3(i, 4 * dis(gen), j);
        }
    }

    auto surface = BezierSurface<4, 4>(std::move(ctrl_grid), 1005);

    auto copper = std::make_shared<Material>();
    copper->name = std::string("copper");
    copper->ambient = glm::vec3(0.19125, 0.0735, 0.0225);
    copper->diffuse = glm::vec3(0.7038, 0.27048, 0.0828);
    copper->specular = glm::vec3(0.256777, 0.137622, 0.086014);
    copper->shininess = 0.1;

    bool running = true;
    bool mouseCaptured = false;

    auto mesh_object = renderer.createObject(surface.mesh(), "normal", std::vector<Texture>(), copper);

    auto& scene = renderer.getScene();

    if (mesh_object) {
        scene.addObject("AA", std::move(*mesh_object));
    }

    scene.addPointLight({glm::vec3(1.0, 1.0, 1.0), 1.0, 20.0, 5.0}, glm::vec3(0.0, 10.0, 0.0));
    scene.addPointLight({glm::vec3(1.0, 1.0, 1.0), 1.0, 20.0, 5.0}, glm::vec3(10.0, 0.0, 0.0));
    scene.addPointLight({glm::vec3(1.0, 1.0, 1.0), 1.0, 20.0, 5.0}, glm::vec3(0.0, 10.0, 10.0));

    // render loop
    // -----------
    while (running) {
        // input
        // -----

        processInput(state, running, renderer);

        renderer.renderScene();

        SDL_GL_SwapWindow(window);
    }

    // Cleanup SDL2 resources
    // delete renderer;
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

// process all input: query SDL whether relevant keys are pressed/released this
// frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(WindowState& state, bool& running, Renderer& renderer) {
    SDL_Event event = {0};

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
                            SDL_CaptureMouse(SDL_FALSE);
                            SDL_SetWindowGrab(state.window, SDL_FALSE);
                            SDL_ShowCursor(SDL_ENABLE);
                            state.mouseLocked = false;
                        } else {
                            running = false;
                        }
                        break;
                    case SDLK_LEFT:
                        renderer.getScene().camera().strafeBy(0.5);
                        break;
                    case SDLK_RIGHT:
                        renderer.getScene().camera().strafeBy(-0.5);
                        break;
                    case SDLK_UP:
                        renderer.getScene().camera().moveUp(0.5);
                        break;
                    case SDLK_DOWN:
                        renderer.getScene().camera().moveUp(-0.5);
                        break;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                switch (event.button.button) {
                    case SDL_BUTTON_LEFT:
                        SDL_ShowCursor(SDL_DISABLE);
                        SDL_CaptureMouse(SDL_TRUE);
                        SDL_SetWindowGrab(state.window, SDL_TRUE);
                        SDL_WarpMouseInWindow(state.window, state.width / 2, state.height / 2);
                        SDL_GetRelativeMouseState(&state.mouseX, &state.mouseY);
                        state.mouseLocked = true;
                        break;
                }
                break;
            case SDL_MOUSEWHEEL:
                renderer.getScene().camera().moveBy(event.wheel.y * 0.5);
                break;
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    renderer.resize(event.window.data1, event.window.data2);
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
        SDL_WarpMouseInWindow(state.window, state.width / 2, state.height / 2);
        state.mouseX = 0;
        state.mouseY = 0;
    }

    // Handle mouse movement
    if (deltaX != 0 || deltaY != 0) {
        auto& camera = renderer.getScene().camera();
        camera.rotateRight(0.15 * deltaX);
        camera.rotateUp(0.15 * deltaY);
    }
}

void toggleMouseLock(bool& mouseLocked) {
    mouseLocked = !mouseLocked;
    SDL_SetRelativeMouseMode(mouseLocked ? SDL_TRUE : SDL_FALSE);
}

int getMaxMultisampleSamples() {
    int maxSamples = 1;
    bool maxFound = false;
    while (!maxFound) {
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, maxSamples * 2);
        SDL_Window* window = SDL_CreateWindow("HIDDEN", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1000, 1000,
                                              SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN);
        maxFound = window == NULL;
        if (!maxFound) {
            SDL_DestroyWindow(window);
            maxSamples *= 2;
        }
    }
    return maxSamples;
}