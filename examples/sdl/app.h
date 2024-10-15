#include "SDL3/SDL_events.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"
#include "world.h"

#define RESOURCE_SDL_CTX "resource::sdl"
typedef struct {
    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_Event* events;
    size_t amountOfEvents;
    bool run;
} RSDLContext;

bool sdl_context_init(RSDLContext* app);
void sdl_context_cleanup(RSDLContext *sdlCtx);

void sdl_context_prepare_render_scene_sys(World* world);
void sdl_context_push_events_sys(World* world);
void sdl_context_quit_event_sys(World *world);
void sdl_context_present_render_scene_sys(World* world);
