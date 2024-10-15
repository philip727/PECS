#ifndef _RESOURCE_HEADER_H
#define _RESOURCE_HEADER_H

typedef char *ResourceType;
typedef void *ResourcePtr;

typedef struct {
    // The name type of that resource, this is so we can query for that resource
    ResourceType type;
    // A pointer that points to a struct, for example a struct named RTime that
    // holds delta time
    ResourcePtr data;
} ResourceData;

#endif // !_RESOURCE_HEADER_H
