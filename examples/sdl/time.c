#include "world.h"
#include "time.h"
#include "SDL3/SDL_timer.h"
#include <stdio.h>

void time_update_delta_time_sys(World *world) {
    ResourceData *timeResource = world_get_resource(world, TIME_RESOURCE_TYPE);
    RTime *time = (RTime *)timeResource->data;

    time->last = time->now;
    time->now = SDL_GetPerformanceCounter();

    time->deltaTime = (double)((time->now - time->last) /
                               (double)SDL_GetPerformanceFrequency());
}
