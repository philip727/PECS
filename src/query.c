#include "query.h"
#include <stdlib.h>

bool query_is_empty(Query query) { return query.amountOfComponents == 0; }

bool query_result_is_empty(QueryResult queryResult) {
    return queryResult.amountOfResults == 0;
}

void query_result_cleanup(QueryResult *queryResult) {
    free(queryResult->entries);
    queryResult->entries = NULL;
}
