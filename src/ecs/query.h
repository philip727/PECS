#ifndef _QUERY_HEADER_H
#define _QUERY_HEADER_H
#include "entity.h"

typedef struct {
    ComponentData** components;
    size_t amountOfComponents;
    Entity* entity;
} QueryEntry;

typedef struct {
    size_t amountOfEntries;
    QueryEntry * entries;
} QueryResult;

void query_result_cleanup(QueryResult* queryResult);

typedef struct {
    size_t amountOfComponents;
    ComponentType * componentTypes;
} Query;

bool query_is_empty(Query* query);

#endif // !_QUERY_HEADER_H
