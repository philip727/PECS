#ifndef _SYSTEMS_HEADER_H
#define _SYSTEMS_HEADER_H

#include "query.h"
#include "world.h"

// A system set is a specific running set. So Startup runs first and typically runs once.
typedef char* SystemSet;

// A system ptr is a pointer to a function that takes in world. This allows you to query
// for entities and resources in the world.
typedef void (*SystemPtr)(World* world);

// The system runner holds all the system pointers. It will runs these systems based on their sets.
typedef struct {
    size_t startupSystemCount;
    SystemPtr* startupSystems;
    size_t updateSystemCount;
    SystemPtr* updateSystems;
} SystemRunner;


// Allocates the array of startup and update systems to the heap.
void system_runner_alloc(SystemRunner *sysRunner);
// Adds a system to a specific set in the system runner. So if you wanted a system that creates a character
// you would run this in the startup set and insert an entity into the world.
void system_runner_add_system(SystemRunner* ecs, World* world, SystemSet set, SystemPtr sys);
// Runs all the systems inside the startup set.
void system_runner_run_startup_systems(SystemRunner* sysRunner, World* world);
// Runs all the systems inside the update set.
void system_runner_run_update_systems(SystemRunner* sysRunner, World* world);
// Removes the system pointer arrays from memory.
void system_runner_cleanup(SystemRunner *sysRunner);

// The startup set. This should be typically only ever run once on start
#define SYSTEM_SET_STARTUP "system_set::startup"
// The update set. This should be run every frame, typically in your main loop.
#define SYSTEM_SET_UPDATE "system_set::update"

#endif // !_SYSTEMS_HEADER_H
