typedef struct {
    float x;
    float y;
} Vec2;

Vec2 vec2_add(Vec2 lhs, Vec2 rhs);
Vec2 vec2_sub(Vec2 lhs, Vec2 rhs);
double vec2_magnitude(Vec2* vec);
Vec2 vec2_normalize(Vec2 vec);


Vec2 vec2_mul_double(Vec2 lhs, double rhs);
