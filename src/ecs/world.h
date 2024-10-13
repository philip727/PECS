#ifndef _WORLD_HEADER_H
#define _WORLD_HEADER_H

#include "entity.h"
#include "query.h"
#include <stdint.h>

typedef struct {
    EntityTable entities;
} World;

void world_alloc(World* world);
void world_cleanup(World *world);
QueryResult world_query(World *world, Query query);

// World entity
Entity* world_get_entity(const World *world, size_t entityId);
size_t world_create_empty_entity(World *world);
void world_remove_entity(World *world, size_t index);
void world_insert_component(const World *world, size_t entityId, ComponentData data);

#endif // !_WORLD_HEADER_H
