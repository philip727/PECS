#ifndef _QUERY_HEADER_H
#define _QUERY_HEADER_H
#include "entity.h"

typedef struct {
    ComponentData** components;
    size_t amountOfComponents;
    Entity* entity;
    size_t entityId;
} QueryResultEntry;

typedef struct {
    size_t amountOfResults;
    QueryResultEntry * entries;
} QueryResult;

bool query_result_is_empty(QueryResult queryResult);
void query_result_cleanup(QueryResult* queryResult);

typedef struct {
    size_t amountOfComponents;
    ComponentType * componentTypes;
} Query;

bool query_is_empty(Query query);

#endif // !_QUERY_HEADER_H
