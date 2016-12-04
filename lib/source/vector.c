
#include "hexapod/vector.h"

#include <stdint.h>

hpod_vector2_t hpod_vector2_mul(hpod_vector2_t *a, hpod_vector2_t *b)
{
    hpod_vector2_t c;
    
    c.x = a->x + b->x;
    c.y = a->y + b->y;

    return c;   
}

hpod_vector3_t hpod_vector3_mul(hpod_vector3_t *a, hpod_vector3_t *b)
{
    hpod_vector3_t c;
    
    c.x = a->x + b->x;
    c.y = a->y + b->y;
    c.z = a->z + b->z;

    return c;   
}