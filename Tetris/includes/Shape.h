#ifndef SHAPE_H
#define SHAPE_H

#include <stdint.h>
#include <stdlib.h>

typedef enum {
    MINO_I = 0,
    MINO_O,
    MINO_S,
    MINO_Z,
    MINO_J,
    MINO_L,
    MINO_T,
    MINO_TYPE_MAX
}Mino_Type;

typedef enum {
    ANGLE_0 = 0,
    ANGLE_90,
    ANGLE_180,
    ANGLE_270,
    ANGLE_MAX
}Rotate_Type;

typedef struct {
    Mino_Type type;
    uint16_t rotations[ANGLE_MAX];
}MinoShape;

extern const MinoShape SHAPES[MINO_TYPE_MAX];

#endif
