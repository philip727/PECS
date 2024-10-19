# What is PECS?
PECS is a simple barebones ECS architecture written in C. It is not meant for production and is a hobby project.

### WARNING
This should not be used for any big projects. A lot of features are missing that you would typically find in an ECS
for any sort of development. I built this to understand how an ECS can be built from scratch. Here are some better alternatives:

- [Bevy](https://github.com/bevyengine/bevy) - A simple data-driven game engine built in rust.
- [Flecs](https://github.com/SanderMertens/flecs) - A fast and lightweight ECS built with C and C++.

# Goals
- Capable of creating entities and inserting data on them.
- Being able to query for the components from the entities on the world.
- A performant and barebones ECS to create simple projects quickly.

# Why ECS?
**Entity, Component, System** architecture is very scalable and allows to build complex systems easily and efficiently.
Entities are empty containers which hold components. Components store data and systems handle behaviour dependant on components.
This is allows you to separate concerns across your source code allowing for cleaner, more maintable, and simpler code.
The logic is separated from the data, and allows you to have specific behaviours for different scenarios. You will see the use of this
in the [Examples](#Example).

# Getting Started
As this isn't intended for re-use in other projects or to be a mainstream ECS. There will be no proper getting started guide.
If you wish to use this, clone the project into a directory and build with **CMake** using the **CMakeLists.txt** inside the root directory. Once built
make sure to include the headers in your project, and link the **lib** directory from the build folder and link the libraries dll from the **bin** dir.

# Example
### SDL Example
https://github.com/user-attachments/assets/b2691bd4-53e3-41f5-8c10-9d5cb56d61c1

[Source](https://github.com/philip727/PECS/tree/main/examples/sdl)

Setup main. To initialize SDL please follow an [SDL tutorial](https://wiki.libsdl.org/SDL2/Tutorials).
Here we create resources that can be acccessed throughout all systems, this allows us to hold data that doesn't need to be singled out
to an entity.
```c
// Create the world and the system runner.
World world = {};
world_alloc(&world);
SystemRunner sysRunner = {};
system_runner_alloc(&sysRunner);

// Creates a resource that holds the SDL Context
RSDLContext sdlCtx = {};
sdlCtx.run = true;

// Initializes the SDL window and renderer
if (!sdl_context_init(&sdlCtx)) {
    sdl_context_cleanup(&sdlCtx);
    return 1;
}

// Pushes the SDL context resource to the world, so we can later access in a system.
ResourceData sdlCtxResource = {
    .type = SDL_CTX_RESOURCE_TYPE,
    .data = (void *)&sdlCtx,
};
world_insert_resource(&world, sdlCtxResource);

// Creates a resource so we can handle delta time
RTime time = {};
time.now = SDL_GetPerformanceCounter();
time.last = 0;
time.deltaTime = 0;

// Pushes the time resource to the world, so we can later access it.
ResourceData timeResource = {
    .type = TIME_RESOURCE_TYPE,
    .data = (void *)&time,
};
world_insert_resource(&world, timeResource);
```

Define the required components
```c
// Marker component so we can query for just the local player
#define LOCAL_CHARACTER_COMPONENT_TYPE "component::local"

// Speed component which allows us to define the speed of an entity
#define SPEED_COMPONENT_TYPE "component::speed"
typedef struct {
    float value;
} CSpeed;

// A transform component which allows us to specify the position of an entity
#define TRANSFORM_COMPONENT_TYPE "component::transform"
typedef struct {
    Vec2 position;
} CTransform;

// A sprite component that will hold the rect data for rendering to SDL
#define SPRITE_COMPONENT_TYPE "component::sprite"
typedef struct {
    SDL_FRect *rect;
} CSprite;
```

Update sprite with correct transform position. Now every entity that has a CSprite and CTransform will be 
automatically handled without having to write anymore handling or code.
```c
// Update all the sprite rects with the transform position
void update_sprite_with_transform(World *world) {
    // Query for the sprite component and transform component
    char *types[] = {SPRITE_COMPONENT_TYPE, TRANSFORM_COMPONENT_TYPE};
    Query query = {
        .amountOfComponents = 2,
        .componentTypes = types,
    };

    // Gets the result from the query
    QueryResult result = world_query(world, query);
    // Makes sure the query isn't empty
    if (query_result_is_empty(result)) {
        query_result_cleanup(&result);
        return;
    }

    // Iterate through each entity in the result
    for (size_t i = 0; i < result.amountOfResults; i++) {
        QueryResultEntry entry = result.entries[i];
        // Get the sprite component. Components are returned in the exact order they are queried in.
        // This allows us directly access through indexing
        CSprite *sprite = (CSprite *)entry.components[0]->data;
        // Get the transform component
        CTransform *transform = (CTransform *)entry.components[1]->data;

        // Set the rect x and y to the position x and y on the transform
        sprite->rect->x = transform->position.x;
        sprite->rect->y = transform->position.y;
    }

    // Ensure we cleanup the query result
    query_result_cleanup(&result);
}

```

Spawning the character
```c 
// The system for spawning the character. We must remember this doesn't directly interact with SDL.
// This simply just creates the data that SDL uses to render the character to the screen.
void spawn_character_sys(World *world) {
    // Create an entity and get it's id
    size_t id = world_create_empty_entity(world);

    // Marker component so we can query just the local player.
    ComponentData localComponent = {};
    localComponent.data = NULL;
    localComponent.type = LOCAL_CHARACTER_COMPONENT_TYPE;
    world_insert_component(world, id, localComponent);

    // The rect required so we can render a rectangle to the SDL_Renderer
    SDL_FRect rect = {
        .x = 0,
        .y = 0,
        .w = 128,
        .h = 128,
    };
    ComponentData spriteComponent = {};
    CSprite *sprite = (CSprite *)malloc(sizeof(CSprite));
    sprite->rect = (SDL_FRect *)malloc(sizeof(SDL_FRect));
    // Copy the rect into the allocated space for the rect
    memcpy(sprite->rect, &rect, sizeof(SDL_FRect));

    // We MUST declare the component type or else we can't query for it
    spriteComponent.type = SPRITE_COMPONENT_TYPE;
    spriteComponent.data = (void *)sprite;
    // Insert the sprite component on to the entity
    world_insert_component(world, id, spriteComponent);

    // The transform component so we can modify the position of the sprite
    ComponentData transformComponent = {};
    CTransform *transform = (CTransform *)malloc(sizeof(CTransform));
    transform->position.x = 50;
    transform->position.y = 50;
    transformComponent.type = TRANSFORM_COMPONENT_TYPE;
    transformComponent.data = (void *)transform;
    // Insert the transform component on to the entity
    world_insert_component(world, id, transformComponent);

    // The speed component so we can change the speed of the player
    ComponentData speedComponent = {};
    speedComponent.type = SPEED_COMPONENT_TYPE;
    CSpeed *speed = (CSpeed *)malloc(sizeof(CSpeed));
    speed->value = 500;
    speedComponent.data = (void *)speed;
    // Insert the speed component on to the entity
    world_insert_component(world, id, speedComponent);
}
```

Calculating delta time for movement
```c
// Calculates delta time
void time_update_delta_time_sys(World *world) {
    // Access the time resouce data
    ResourceData *timeResource = world_get_resource(world, TIME_RESOURCE_TYPE);
    // Get the pointer to the time resource
    RTime *time = (RTime *)timeResource->data;

    // Sets the last count value of the counter.
    time->last = time->now;
    // Updates the current count to the value of the counter.
    time->now = SDL_GetPerformanceCounter();

    // Calculates delta time using the count per second of the counter.
    time->deltaTime = (double)((time->now - time->last) /
                               (double)SDL_GetPerformanceFrequency());
}
```

Moving the character with a system. This allows us to separate specific behaviour for different
components on the character. If we wanted a roll mechanic. We could add A CRoll to the entity.
Query for CRoll and CSpeed and when a key is pressed, we can adjust the CSpeed, which in return
will be changed for this system as well.
```c
// Move the character using SDL keys
void move_character_sys(World *world) {
    // Gets the time resource so we can access delta time
    ResourceData *timeResource = world_get_resource(world, TIME_RESOURCE_TYPE);
    RTime *time = (RTime *)timeResource->data;

    // Marked with local character component, so we dont move other transforms.
    char *types[] = {LOCAL_CHARACTER_COMPONENT_TYPE, TRANSFORM_COMPONENT_TYPE,
                     SPEED_COMPONENT_TYPE};
    Query query = {
        .amountOfComponents = 3,
        .componentTypes = types,
    };

    // Get the results of the query
    QueryResult result = world_query(world, query);
    // Ensure the query isn't empty (Has 0 results)
    if (query_result_is_empty(result)) {
        query_result_cleanup(&result);
        return;
    }

    // As we know there's only one local player. there only should be a single query result
    QueryResultEntry entry = result.entries[0];
    // Get the transform component. Should be the second component as local character is first in query
    CTransform *transform = (CTransform *)entry.components[1]->data;
    // Get the speed component
    CSpeed *speed = (CSpeed *)entry.components[2]->data;
    // 

    // Gets the delta time in seconds from the time resource
    // 
    double deltaSeconds = time_get_delta_seconds(time);
    // Gets the keyboard state from SDL
    const bool *keyboardState = SDL_GetKeyboardState(NULL);
    // The initial movement dir. (Vec2 is literally just an x and y double).
    Vec2 movementDir = {.x = 0, .y = 0};
    if (keyboardState[SDL_SCANCODE_W]) {
        movementDir.y -= 1;
    }

    if (keyboardState[SDL_SCANCODE_S]) {
        movementDir.y += 1;
    }

    if (keyboardState[SDL_SCANCODE_D]) {
        movementDir.x += 1;
    }

    if (keyboardState[SDL_SCANCODE_A]) {
        movementDir.x -= 1;
    }

    // Add the calculated distance travelled to the transform position.
    // We multiply the normalized movement dir by the speed, and then multiply by delta time(to ensure we move at the same
    // speed no matter the frame rate). We then add that final value to the transform position.
    transform->position =
        vec2_add(transform->position,
                 vec2_mul_double(
                     vec2_mul_double(vec2_normalize(movementDir), speed->value),
                     deltaSeconds));

    // Ensure we clean up the query result.
    query_result_cleanup(&result);
}

```

The system configuration for the runtime. For SDL systems, please refer to the source and SDL
tutorials to understand why and how they work.
```c
// Spawns the character in the startup system set
system_runner_add_system(&sysRunner, &world, SYSTEM_SET_STARTUP,
                         spawn_character_sys);

// Updates the delta time run in the update system set.
system_runner_add_system(&sysRunner, &world, SYSTEM_SET_UPDATE,
                         time_update_delta_time_sys);
// Prepares the SDL render scene in the update system set.
system_runner_add_system(&sysRunner, &world, SYSTEM_SET_UPDATE,
                         sdl_context_prepare_render_scene_sys);
// Pushes events to an array held in the SDLContext resource, and clears them, ran in the
// update system set.
system_runner_add_system(&sysRunner, &world, SYSTEM_SET_UPDATE,
                         sdl_context_push_events_sys);
// Handles the quit event for SDL and is run in the update system set.
system_runner_add_system(&sysRunner, &world, SYSTEM_SET_UPDATE,
                         sdl_context_quit_event_sys);
// Moves the character based on key input from SDL.
system_runner_add_system(&sysRunner, &world, SYSTEM_SET_UPDATE,
                         move_character_sys);

// Renders the sprite using the transform
system_runner_add_system(&sysRunner, &world, SYSTEM_SET_UPDATE,
                         update_sprite_with_transform);

// Draws the sprites to the renderer with a blue colour
system_runner_add_system(&sysRunner, &world, SYSTEM_SET_UPDATE,
                         render_sprites_sys);

// Presents the SDL render scene
system_runner_add_system(&sysRunner, &world, SYSTEM_SET_UPDATE,
                         sdl_context_present_render_scene_sys);

// Runs all the systems in the startup system set
system_runner_run_startup_systems(&sysRunner, &world);
// Runs all the systems in the update system set in the while loop. Until the quit
// event is handled by SDL. 
while (sdlCtx.run) {
    system_runner_run_update_systems(&sysRunner, &world);
}

// We finally cleanup the sdl context
sdl_context_cleanup(&sdlCtx);
world_cleanup(&world);
system_runner_cleanup(&sysRunner);
```
