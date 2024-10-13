#ifndef _ENTITY_HEADER_H
#define _ENTITY_HEADER_H

#include <stdint.h>
#include <stdbool.h>

#define NAME_COMPONENT_TYPE "component::name"
#define ComponentType char*

typedef struct {
    char* data;
} CName;

typedef struct {
    ComponentType type;
    void* data;
} ComponentData;

bool component_is_of_type(const ComponentData* component, const ComponentType rhs);

typedef struct {
    size_t amountOfComponents;
    ComponentData* components;
} Entity;

void entity_alloc(Entity *entity);
void entity_alloc_component_space(Entity *entity);
void entity_cleanup(Entity *entity);
ComponentData *entity_get_component(const Entity *entity, const ComponentType rhs);

typedef struct {
    Entity* table;
    size_t size;
} EntityTable;

void entity_table_alloc(EntityTable *entityTable);
void entity_table_resize(EntityTable *entityMap, size_t newSize);
void entity_table_push(EntityTable *entityMap, Entity entity);
void entity_table_remove(EntityTable *entityMap, size_t index);
void entity_table_cleanup(EntityTable *entityMap);

#endif // !_ENTITY_HEADER_H
