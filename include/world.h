#ifndef _WORLD_HEADER_H
#define _WORLD_HEADER_H

#include "entity.h"
#include "query.h"
#include "resource.h"
#include <stdint.h>

typedef struct {
    EntityTable entities;
    ResourceData* resources;
    size_t amountOfResources;
} World;

// Allocates the array of entities and resources to the heap.
void world_alloc(World* world);
// Removes every entity in the world from memory and removes the resources.
void world_cleanup(World *world);
// Allows you to query for entities with specific components in the world.
// E.g: You could query for every entity in the world with a position. However,
// if you were to add a second component for example a local player componnet.
// It would only query for the position of the local player and every other entity would be ignored.
QueryResult world_query(World *world, Query query);

// Gets a pointer to a specific entity from their id.
Entity* world_get_entity(const World *world, size_t entityId);
// Creates an empty entity and returns their id. Ids are not static, they are the position of the entity
// in the entity array.
size_t world_create_empty_entity(World *world);
// Removes an entity from the world and frees it's components by its index.
void world_remove_entity(World *world, size_t index);
// Inserts a component onto a specific entity by id.
void world_insert_component(const World *world, size_t entityId, ComponentData data);

// Pushes a global resource into the world that can be accessed through systems.
// E.g: You may have a time resource that gets update in one system every frame that calculates delta time,
// that you can then access in another system(maybe a movement system).
void world_insert_resource(World *world, ResourceData data);
// Gets a pointer to a resource thats inserted into the world.
ResourceData *world_get_resource(World *world, ResourceType type);

#endif // !_WORLD_HEADER_H
