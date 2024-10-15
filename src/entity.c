#include "entity.h"
#include <stdlib.h>
#include <string.h>

// Component
bool component_is_of_type(const ComponentData *component,
                          const ComponentType rhs) {
    if (strcmp(component->type, rhs) != 0) {
        return false;
    }

    return true;
}

void component_cleanup(ComponentData *component) {
    free(component->data);
    component->data = NULL;
}

// Entity
/// Initializes entity pointers
void entity_alloc(Entity *entity) {
    entity->components = (ComponentData *)malloc(sizeof(ComponentData *));
    entity->amountOfComponents = 0;
}

void entity_alloc_component_space(Entity *entity) {
    entity->amountOfComponents++;
    entity->components = (ComponentData *)realloc(
        entity->components, entity->amountOfComponents * sizeof(ComponentData));
}

ComponentData *entity_get_component(const Entity *entity,
                                    const ComponentType rhs) {
    for (size_t i = 0; i < entity->amountOfComponents; i++) {
        ComponentData *component = &entity->components[i];
        if (component_is_of_type(component, rhs)) {
            return component;
        }
    }

    return NULL;
}

void entity_cleanup(Entity *entity) {
    for (size_t i = 0; i < entity->amountOfComponents; i++) {
        component_cleanup(&entity->components[i]);
    }

    free(entity->components);
    entity->components = NULL;
}

// Entity table
char *entity_table_ptr_dest(EntityTable *entityMap, size_t index) {
    char *baseAddr = (char *)entityMap->table;
    return baseAddr + (index * sizeof(Entity));
}

/// Initializes the tables entity pointers
void entity_table_alloc(EntityTable *entityTable) {
    entityTable->table = (Entity *)malloc(sizeof(EntityTable));
    entityTable->size = 0;
}

void entity_table_resize(EntityTable *entityMap, size_t newSize) {
    entityMap->size = newSize;
    entityMap->table =
        (Entity *)realloc(entityMap->table, newSize * sizeof(Entity));
}

void entity_table_remove(EntityTable *entityMap, size_t index) {
    char *targetAddr = entity_table_ptr_dest(entityMap, index);

    // The remaining follow entities that need to be moved back in memory
    char *followEntitiesStartAddr = entity_table_ptr_dest(entityMap, index + 1);
    size_t amountOfFollowingEntities = entityMap->size - index - 1;

    // Write over the index we are removing
    memmove(targetAddr, followEntitiesStartAddr,
            amountOfFollowingEntities * sizeof(Entity));
    entity_table_resize(entityMap, entityMap->size - 1);
}

void entity_table_cleanup(EntityTable *entityMap) {
    for (size_t i = 0; i < entityMap->size; i++) {
        entity_cleanup(&entityMap->table[i]);
    }

    free(entityMap->table);
    entityMap->table = NULL;
}
