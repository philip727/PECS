#include "world.h"
#include "time.h"
#include "SDL3/SDL_timer.h"
#include <stdio.h>

// Calculates delta time
void time_update_delta_time_sys(World *world) {
    // Access the time resouce data
    ResourceData *timeResource = world_get_resource(world, TIME_RESOURCE_TYPE);
    // Get the pointer to the time resource
    RTime *time = (RTime *)timeResource->data;

    // Sets the last count value of the counter.
    time->last = time->now;
    // Updates the current count to the value of the counter.
    time->now = SDL_GetPerformanceCounter();

    // Calculates delta time using the count per second of the counter.
    time->deltaTime = (double)((time->now - time->last) /
                               (double)SDL_GetPerformanceFrequency());
}
