#include "world.h"

// Marker component so we can query for just the local player
#define LOCAL_CHARACTER_COMPONENT_TYPE "component::local"

// A transform component which allows us to specify the position of an entity
#define SPEED_COMPONENT_TYPE "component::speed"
typedef struct {
    float value;
} CSpeed;

void spawn_character_sys(World* world);
void move_character_sys(World *world);
