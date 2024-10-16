#include "systems.h"
#include "world.h"
#include "app.h"
#include "character.h"
#include "draw.h"

int main() {
    World world = {};
    world_alloc(&world);
    SystemRunner sysRunner = {};
    system_runner_alloc(&sysRunner);

    RSDLContext sdlCtx = {};
    sdlCtx.run = true;

    if (!sdl_context_init(&sdlCtx)) {
        sdl_context_cleanup(&sdlCtx);
        return 1;
    }

    // Resources
    ResourceData sdlCtxResource = {
        .type = SDL_CTX_RESOURCE_TYPE,
        .data = (void *)&sdlCtx,
    };
    world_insert_resource(&world, sdlCtxResource);

    // Startup systems
    system_runner_add_system(&sysRunner, &world, SYSTEM_SET_STARTUP,
                             spawn_character_sys);

    // Update systems
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
