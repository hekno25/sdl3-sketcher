#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>

typedef struct {
    char *title;
    int width;
    int height;
    SDL_Window *window;
    SDL_GPUDevice *gpu;
    SDL_GPUCommandBuffer *command_buffer;
} Renderer;

typedef struct {
    Renderer *renderer;
} AppState;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv) {
    // Allocate memory
    *appstate = malloc(sizeof(AppState));
    Renderer *renderer = malloc(sizeof(Renderer));

    // Create properly typed pointer to appstate
    AppState *state = (AppState *)appstate;

    // Initialize some known fields
    state->renderer = renderer;
    renderer->title = "SDL3 sketcher!";
    renderer->width = 1280;
    renderer->height = 720;

    SDL_SetAppMetadata("SDL3 Sketcher", "0.1", "org.hekno25.sdl3-sketcher");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    renderer->window =
        SDL_CreateWindow(renderer->title, renderer->width, renderer->height,
                         0 | SDL_WINDOW_VULKAN);
    if (!renderer->window) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    renderer->gpu =
        SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, false, NULL);
    if (!renderer->gpu) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_ClaimWindowForGPUDevice(renderer->gpu, renderer->window)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    renderer->command_buffer = SDL_AcquireGPUCommandBuffer(renderer->gpu);
    if (!renderer->command_buffer) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
    // Setup typed identifiers
    AppState *state = (AppState *)appstate;
    Renderer *renderer = state->renderer;

    SDL_GPUTexture **swapchain_texture = nullptr;
    // Switch to this when we get SDL 3.2.x
    // bool success =
    // SDL_WaitAndAcquireGPUSwapchainTexture(renderer->command_buffer,
    // renderer->window);
    bool swapchain_success = SDL_AcquireGPUSwapchainTexture(
        renderer->command_buffer, renderer->window, swapchain_texture, NULL,
        NULL);

    if (!swapchain_success) {
        // Window is probably just minimized, so it's not an error
        return SDL_APP_CONTINUE;
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    // AppState* state = (AppState*)appstate;
}
