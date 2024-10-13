#include "ecs/entity.h"
#include "ecs/query.h"
#include "ecs/world.h"
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define HEALTH_COMPONENT_TYPE "component::health"

typedef struct {
    uint32_t health;
    uint32_t maxHealth;
} CHealth;

int main(int argc, char *argv[]) {
    srand(time(NULL));

    World world = {};
    world_alloc(&world);

    // Create a local player
    size_t id = world_create_empty_entity(&world);
    ComponentData component = {};
    CName *name = (CName *)malloc(sizeof(CName));
    name->data = "LocalActor";
    component.data = (void *)name;
    component.type = NAME_COMPONENT_TYPE;
    world_insert_component(&world, id, component);

    printf("AAMEOWW\n");
    ComponentData healthComponent = {};
    CHealth *health = (CHealth *)malloc(sizeof(CHealth));
    health->health = 100;
    health->maxHealth = 100;
    healthComponent.data = (void *)health;
    healthComponent.type = HEALTH_COMPONENT_TYPE;
    world_insert_component(&world, id, healthComponent);
    printf("MEOWW\n");

    size_t idTwo = world_create_empty_entity(&world);
    ComponentData componentTwo = {};
    CName *nameTwo = (CName *)malloc(sizeof(CName));
    nameTwo->data = "Actor";
    componentTwo.data = (void *)nameTwo;
    componentTwo.type = NAME_COMPONENT_TYPE;
    world_insert_component(&world, idTwo, componentTwo);

    world_create_empty_entity(&world);
    world_create_empty_entity(&world);
    world_create_empty_entity(&world);
    world_create_empty_entity(&world);
    world_create_empty_entity(&world);
    world_create_empty_entity(&world);
    world_create_empty_entity(&world);

    char *types[] = {NAME_COMPONENT_TYPE, HEALTH_COMPONENT_TYPE};
    Query query = {
        .amountOfComponents = 2,
        .componentTypes = types,
    };

    QueryResult result = world_query(&world, query);

    printf("%llu\n", result.amountOfEntries);

    query_result_cleanup(&result);
    world_cleanup(&world);
    printf("Gracefully cleaned up world");

    return 0;
}
