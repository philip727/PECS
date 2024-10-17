#ifndef _QUERY_HEADER_H
#define _QUERY_HEADER_H
#include "entity.h"

// An entry for each entity found in the query.
typedef struct {
    // Each component that you queried for will be in order on this array.
    ComponentData **components;
    // The amount of components found. Incase you need to iterate over them.
    size_t amountOfComponents;
    // A pointer to the entity found.
    Entity *entity;
    // The entity id that was found.
    size_t entityId;
} QueryResultEntry;

// The result of a query, holding all of its entries.
typedef struct {
    // The amount of entries found.
    size_t amountOfResults;
    // The array of entries.
    QueryResultEntry *entries;
} QueryResult;

// Returns true if a query result has no entries
bool query_result_is_empty(QueryResult queryResult);
// Frees the entries array
void query_result_cleanup(QueryResult *queryResult);

// The parameters of a query.
typedef struct {
    // The amount of components you are passing into the query. MUST be specified or else 
    // it wont check for all components.
    size_t amountOfComponents;
    // An array of identifiers of which you wish to query from each entity.
    ComponentType *componentTypes;
} Query;

// Checks if a query is empty
bool query_is_empty(Query query);

#endif // !_QUERY_HEADER_H
