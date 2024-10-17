#include "SDL3/SDL_timer.h"
#include "systems.h"
#include "world.h"
#include "app.h"
#include "character.h"
#include "draw.h"
#include "time.h"

int main() {
    // Create the world and the system runner.
    World world = {};
    world_alloc(&world);
    SystemRunner sysRunner = {};
    system_runner_alloc(&sysRunner);

    // Creates a resource that holds the SDL Context
    RSDLContext sdlCtx = {};
    sdlCtx.run = true;

    // Initializes the SDL window and renderer
    if (!sdl_context_init(&sdlCtx)) {
        sdl_context_cleanup(&sdlCtx);
        return 1;
    }

    // Pushes the SDL context resource to the world, so we can later access in a system.
    ResourceData sdlCtxResource = {
        .type = SDL_CTX_RESOURCE_TYPE,
        .data = (void *)&sdlCtx,
    };
    world_insert_resource(&world, sdlCtxResource);

    // Creates a resource so we can handle delta time
    RTime time = {};
    time.now = SDL_GetPerformanceCounter();
    time.last = 0;
    time.deltaTime = 0;

    // Pushes the time resource to the world, so we can later access it.
    ResourceData timeResource = {
        .type = TIME_RESOURCE_TYPE,
        .data = (void *)&time,
    };
    world_insert_resource(&world, timeResource);

    // Startup systems
    system_runner_add_system(&sysRunner, &world, SYSTEM_SET_STARTUP,
                             spawn_character_sys);
    // Update systems
    system_runner_add_system(&sysRunner, &world, SYSTEM_SET_UPDATE,
                             time_update_delta_time_sys);
    system_runner_add_system(&sysRunner, &world, SYSTEM_SET_UPDATE,
                             sdl_context_prepare_render_scene_sys);
    system_runner_add_system(&sysRunner, &world, SYSTEM_SET_UPDATE,
                             sdl_context_push_events_sys);
    system_runner_add_system(&sysRunner, &world, SYSTEM_SET_UPDATE,
                             sdl_context_quit_event_sys);
    // Moving the character
    system_runner_add_system(&sysRunner, &world, SYSTEM_SET_UPDATE,
                             move_character_sys);

    // Sprite Rendering
    system_runner_add_system(&sysRunner, &world, SYSTEM_SET_UPDATE,
                             update_sprite_with_transform);
    system_runner_add_system(&sysRunner, &world, SYSTEM_SET_UPDATE,
                             render_sprites_sys);

    system_runner_add_system(&sysRunner, &world, SYSTEM_SET_UPDATE,
                             sdl_context_present_render_scene_sys);

    // Runtime loop
    system_runner_run_startup_systems(&sysRunner, &world);
    while (sdlCtx.run) {
        system_runner_run_update_systems(&sysRunner, &world);
    }

    sdl_context_cleanup(&sdlCtx);
    return 0;
}
