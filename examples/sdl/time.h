#include "world.h"
#include <stdint.h>

#define TIME_RESOURCE_TYPE "resource::time"
typedef struct {
    uint64_t now;
    uint64_t last;
    double deltaTime;
} RTime;

void time_update_delta_time_sys(World *world);

static inline double time_get_delta_seconds(RTime *time) {
    return time->deltaTime;
}

static inline double time_get_delta_ms(RTime *time) {
    return time->deltaTime * 1000;
}
