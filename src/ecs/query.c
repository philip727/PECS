#include "query.h"
#include <stdlib.h>

bool query_is_empty(Query *query) { return query->amountOfComponents == 0; }

void query_result_cleanup(QueryResult *queryResult) {
    free(queryResult->entries);
    queryResult->entries = NULL;
}
