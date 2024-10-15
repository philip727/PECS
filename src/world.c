#include "world.h"
#include "entity.h"
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "query.h"
#include "resource.h"

void world_alloc(World *world) {
    entity_table_alloc(&world->entities);

    world->resources = (ResourceData *)malloc(sizeof(ResourceData));
    world->amountOfResources = 0;
}

QueryResult world_query(World *world, Query query) {
    QueryResult result = {
        .amountOfResults = 0,
        .entries = (QueryResultEntry *)malloc(sizeof(QueryResultEntry)),
    };

    for (size_t i = 0; i < world->entities.size; i++) {
        Entity *entity = world_get_entity(world, i);
        QueryResultEntry entry = {
            // Since we should only find the same amount of components as we
            // asked for then we should be able to just allocate the exact size
            // needed
            .amountOfComponents = query.amountOfComponents,
            .entity = entity,
            .components = (ComponentData **)malloc(query.amountOfComponents *
                                                   sizeof(ComponentData *)),
            .entityId = i,
        };

        bool dontAdd = false;
        for (size_t j = 0; j < query.amountOfComponents; j++) {
            ComponentType type = query.componentTypes[j];
            ComponentData *component = entity_get_component(entity, type);
            // An entity must contain all components from a query
            if (component == NULL) {
                dontAdd = true;
                break;
            }

            entry.components[j] = component;
        }

        if (dontAdd) {
            continue;
        }

        // Allocate more space for the amount of entries
        result.amountOfResults++;
        result.entries = (QueryResultEntry *)realloc(
            result.entries, result.amountOfResults * sizeof(QueryResultEntry));

        // Copy the query entry into the array
        memcpy(&result.entries[result.amountOfResults - 1], &entry,
               sizeof(QueryResultEntry));
    }

    return result;
}

void world_insert_resource(World *world, ResourceData data) {
    world->amountOfResources++;
    world->resources = (ResourceData *)realloc(
        world->resources, world->amountOfResources * sizeof(ResourceData));

    world->resources[world->amountOfResources - 1] = data;
}

ResourceData *world_get_resource(World *world, ResourceType type) {
    for (size_t i = 0; i < world->amountOfResources; i++) {
        ResourceData *resource = &world->resources[i];
        if (strcmp(resource->type, type) == 0) {
            return resource;
        }
    }

    printf("Tried to get non-existent resource %s", type);
    return NULL;
}

size_t world_create_empty_entity(World *world) {
    size_t newSize = world->entities.size + 1;
    // Increase the world entity count
    entity_table_resize(&world->entities, newSize);
    // Allocate the components on the entity
    entity_alloc(&world->entities.table[newSize - 1]);

    // The id of the new entity
    return world->entities.size - 1;
}

void world_insert_component(const World *world, size_t entityId,
                            ComponentData data) {
    Entity *entity = &world->entities.table[entityId];
    for (size_t i = 0; i < entity->amountOfComponents; i++) {
        if (strcmp(entity->components[i].type, data.type) == 0) {
            printf(
                "Can not insert two components of the same type on an Entity\n\
Attempted to insert %s on Entity %llu which already has %s\n",
                data.type, entityId, data.type);

            return;
        }
    }

    entity_alloc_component_space(entity);
    ComponentData dest = entity->components[entity->amountOfComponents - 1];
    entity->components[entity->amountOfComponents - 1] = data;
}

void world_remove_entity(World *world, size_t entityId) {
    entity_cleanup(&world->entities.table[entityId]);
    entity_table_remove(&world->entities, entityId);
}

Entity *world_get_entity(const World *world, size_t entityId) {
    return &world->entities.table[entityId];
}

void world_cleanup(World *world) {
    entity_table_cleanup(&world->entities);
    for (size_t i = 0; i < world->amountOfResources; i++) {
        free((&world->resources[i])->data);
    }

    free(world->resources);
    world->resources = NULL;
}
