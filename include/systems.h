#ifndef _SYSTEMS_HEADER_H
#define _SYSTEMS_HEADER_H

#include "query.h"
#include "world.h"

typedef char* SystemSet;

typedef void (*SystemPtr)(World* world);
typedef struct {
    size_t startupSystemCount;
    SystemPtr* startupSystems;
    size_t updateSystemCount;
    SystemPtr* updateSystems;
} SystemRunner;


void system_runner_alloc(SystemRunner *sysRunner);
void system_runner_add_system(SystemRunner* ecs, World* world, SystemSet set, SystemPtr sys);
void system_runner_run_startup_systems(SystemRunner* sysRunner, World* world);
void system_runner_run_update_systems(SystemRunner* sysRunner, World* world);
void system_runner_cleanup(SystemRunner *sysRunner);

#define SYSTEM_SET_STARTUP "system_set::startup"
#define SYSTEM_SET_UPDATE "system_set::update"

#endif // !_SYSTEMS_HEADER_H
