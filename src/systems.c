#include "systems.h"
#include "world.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void system_runner_alloc(SystemRunner *sysRunner) {
    sysRunner->startupSystemCount = 0;
    sysRunner->startupSystems = (SystemPtr *)malloc(sizeof(SystemPtr));
    sysRunner->updateSystemCount = 0;
    sysRunner->updateSystems = (SystemPtr *)malloc(sizeof(SystemPtr));
}

void system_runner_add_system(SystemRunner *sysRunner, World *world,
                              SystemSet set, SystemPtr sys) {

    if (strcmp(set, SYSTEM_SET_STARTUP) == 0) {
        // Allocates more space for the startup systems
        sysRunner->startupSystemCount++;
        sysRunner->startupSystems = (SystemPtr *)realloc(
            sysRunner->startupSystems,
            sysRunner->startupSystemCount * sizeof(SystemPtr));

        sysRunner->startupSystems[sysRunner->startupSystemCount - 1] = sys;
    }

    if (strcmp(set, SYSTEM_SET_UPDATE) == 0) {
        // Allocates more space for the update systems
        sysRunner->updateSystemCount++;
        sysRunner->updateSystems = (SystemPtr *)realloc(
            sysRunner->updateSystems,
            sysRunner->updateSystemCount * sizeof(SystemPtr));
        sysRunner->updateSystems[sysRunner->updateSystemCount - 1] = sys;
    }
}

void system_runner_run_startup_systems(SystemRunner *sysRunner, World *world) {
    for (size_t i = 0; i < sysRunner->startupSystemCount; i++) {
        sysRunner->startupSystems[i](world);
    }
}

void system_runner_run_update_systems(SystemRunner *sysRunner, World *world) {
    for (size_t i = 0; i < sysRunner->updateSystemCount; i++) {
        sysRunner->updateSystems[i](world);
    }
}

void system_runner_cleanup(SystemRunner *sysRunner) {
    free(sysRunner->startupSystems);
    free(sysRunner->updateSystems);
}
