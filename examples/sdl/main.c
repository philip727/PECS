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

    // Spawns the character in the startup system set
    system_runner_add_system(&sysRunner, &world, SYSTEM_SET_STARTUP,
                             spawn_character_sys);

    // Updates the delta time run in the update system set.
    system_runner_add_system(&sysRunner, &world, SYSTEM_SET_UPDATE,
                             time_update_delta_time_sys);
    // Prepares the SDL render scene in the update system set.
    system_runner_add_system(&sysRunner, &world, SYSTEM_SET_UPDATE,
                             sdl_context_prepare_render_scene_sys);
    // Pushes events to an array held in the SDLContext resource, and clears them, ran in the
    // update system set.
    system_runner_add_system(&sysRunner, &world, SYSTEM_SET_UPDATE,
                             sdl_context_push_events_sys);
    // Handles the quit event for SDL and is run in the update system set.
    system_runner_add_system(&sysRunner, &world, SYSTEM_SET_UPDATE,
                             sdl_context_quit_event_sys);
    // Moves the character based on key input from SDL.
    system_runner_add_system(&sysRunner, &world, SYSTEM_SET_UPDATE,
                             move_character_sys);

    // Renders the sprite using the transform
    system_runner_add_system(&sysRunner, &world, SYSTEM_SET_UPDATE,
                             update_sprite_with_transform);

    // Draws the sprites to the renderer with a blue colour
    system_runner_add_system(&sysRunner, &world, SYSTEM_SET_UPDATE,
                             render_sprites_sys);

    // Presents the SDL render scene
    system_runner_add_system(&sysRunner, &world, SYSTEM_SET_UPDATE,
                             sdl_context_present_render_scene_sys);

    // Runs all the systems in the startup system set
    system_runner_run_startup_systems(&sysRunner, &world);
    // Runs all the systems in the update system set in the while loop. Until the quit
    // event is handled by SDL. 
    while (sdlCtx.run) {
        system_runner_run_update_systems(&sysRunner, &world);
    }

    // We finally cleanup the sdl context
    sdl_context_cleanup(&sdlCtx);
    world_cleanup(&world);
    system_runner_cleanup(&sysRunner);
    return 0;
}
