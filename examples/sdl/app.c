#include "app.h"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_log.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"
#include "world.h"
#include <stdlib.h>
#include <string.h>

bool sdl_context_init(RSDLContext *sdlCtx) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Couldn't initialize SDL: %s\n",
                     SDL_GetError());
        return false;
    }

    sdlCtx->window = SDL_CreateWindow("ECS Test", 800, 600,
                                      SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

    if (sdlCtx->window == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Couldn't create SDL window: %s\n",
                     SDL_GetError());
        return false;
    }

    sdlCtx->renderer = SDL_CreateRenderer(sdlCtx->window, NULL);

    if (sdlCtx->renderer == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,
                     "Couldn't create SDL opengl renderer: %s\n",
                     SDL_GetError());
        return false;
    }

    sdlCtx->events = (SDL_Event*)malloc(sizeof(SDL_Event));

    return true;
}

void sdl_context_prepare_render_scene_sys(World *world) {
    ResourceData *sdlCtxResource = world_get_resource(world, SDL_CTX_RESOURCE_TYPE);
    RSDLContext *sdlCtx = (RSDLContext *)sdlCtxResource->data;

    SDL_SetRenderDrawColor(sdlCtx->renderer, 96, 128, 255, 255);
    SDL_RenderClear(sdlCtx->renderer);
}

void sdl_context_present_render_scene_sys(World *world) {
    ResourceData *sdlCtxResource = world_get_resource(world, SDL_CTX_RESOURCE_TYPE);
    RSDLContext *sdlCtx = (RSDLContext *)sdlCtxResource->data;

    SDL_RenderPresent(sdlCtx->renderer);
}

void sdl_context_push_events_sys(World *world) {
    ResourceData *sdlCtxResource = world_get_resource(world, SDL_CTX_RESOURCE_TYPE);
    RSDLContext *sdlCtx = (RSDLContext *)sdlCtxResource->data;

    sdlCtx->events = NULL;
    sdlCtx->amountOfEvents = 0;

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        sdlCtx->amountOfEvents++;
        sdlCtx->events = (SDL_Event *)realloc(
            sdlCtx->events, sdlCtx->amountOfEvents * sizeof(SDL_Event));

        memcpy_s(&sdlCtx->events[sdlCtx->amountOfEvents - 1], sizeof(SDL_Event),
                 &event, sizeof(SDL_Event));
    }
}

void sdl_context_quit_event_sys(World *world) {
    ResourceData *sdlCtxResource = world_get_resource(world, SDL_CTX_RESOURCE_TYPE);
    RSDLContext *sdlCtx = (RSDLContext *)sdlCtxResource->data;

    for (size_t i = 0; i < sdlCtx->amountOfEvents; i++) {
        SDL_Event event = sdlCtx->events[i];
        if (event.type == SDL_EVENT_QUIT) {
            sdlCtx->run = false;
        }
    }
}

void sdl_context_cleanup(RSDLContext *sdlCtx) {
    SDL_Quit();
    free(sdlCtx->events);
    sdlCtx->events = NULL;

    SDL_DestroyRenderer(sdlCtx->renderer);
    SDL_DestroyWindow(sdlCtx->window);
}
