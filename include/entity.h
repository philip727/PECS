#ifndef _ENTITY_HEADER_H
#define _ENTITY_HEADER_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define NAME_COMPONENT_TYPE "component::name"
// A component type is the name of the component. This is required so we can
// find the correct component and cast properly
typedef char* ComponentType;

// A pointer to a struct that holds the component
typedef void* ComponentPtr;

// A default component to name an entity.
typedef struct {
    char* data;
} CName;

// The data of the component on the entity.
typedef struct {
    // The identifier so that we can query for this component from each entity.
    ComponentType type;
    // A pointer to the actual component.
    ComponentPtr data;
} ComponentData;

// Checks if a component is of a specific type.
bool component_is_of_type(const ComponentData* component, const ComponentType rhs);

// The main data driver. Everything to do with an entity is held on here in components.
typedef struct {
    // The amount of components incase we need to iterate over each component.
    size_t amountOfComponents;
    // An array holding each component on the entity
    ComponentData* components;
} Entity;

// Allocates the components array to the heap.
void entity_alloc(Entity *entity);

// Allocates enough space for a new component
void entity_alloc_component_space(Entity *entity);
// Frees each component and the component array.
void entity_cleanup(Entity *entity);
// Returns a pointer to a component on the entity gotten by its identifier.
ComponentData *entity_get_component(const Entity *entity, const ComponentType rhs);

// A table that holds entities.
typedef struct {
    Entity* table;
    size_t size;
} EntityTable;

// Allocates the table to the heap.
void entity_table_alloc(EntityTable *entityTable);
// Resizes the table to the amount of memory needed.
void entity_table_resize(EntityTable *entityMap, size_t newSize);
// Removes an entity from the table. Resizing the table memory.
void entity_table_remove(EntityTable *entityMap, size_t index);
// Frees all the entities on the table and cleans itself.
void entity_table_cleanup(EntityTable *entityMap);

#endif // !_ENTITY_HEADER_H
