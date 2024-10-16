#include <math.h>
#include "vec2.h"
inline Vec2 vec2_add(Vec2 lhs, Vec2 rhs) {
    Vec2 vec = {
        .x = lhs.x + rhs.x,
        .y = lhs.y + rhs.y,
    };

    return vec;
}

inline Vec2 vec2_sub(Vec2 lhs, Vec2 rhs) {
    Vec2 vec = {
        .x = lhs.x - rhs.x,
        .y = lhs.y - rhs.y,
    };

    return vec;
}

Vec2 vec2_mul_double(Vec2 lhs, double rhs) {
    Vec2 vec = {
        .x = lhs.x * rhs,
        .y = lhs.y * rhs,
    };

    return vec;
}

inline double vec2_magnitude(Vec2* vec) {
    return sqrt(vec->x * vec->x + vec->y * vec->y);
}

Vec2 vec2_normalize(Vec2 vec) {
    double mag = vec2_magnitude(&vec);
    if (mag == 0) {
        return vec;
    }

    Vec2 newVec = {
        .x = vec.x / mag,
        .y = vec.y / mag,
    };

    return newVec;
}
