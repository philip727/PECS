#include "world.h"

#define LOCAL_CHARACTER_COMPONENT_TYPE "component::local"
#define SPEED_COMPONENT_TYPE "component::speed"
typedef struct {
    float value;
} CSpeed;

void spawn_character_sys(World* world);
void move_character_sys(World *world);
