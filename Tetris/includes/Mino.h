#ifndef MINO_H
#define MINO_H

#include "config.h"
#include "Shape.h"

#define TABLE_MAX 5

typedef struct _mino Mino;
typedef struct mino_class Mino_class;

typedef const int kickTable[ANGLE_MAX][TABLE_MAX][2];

struct mino_class{
    Mino* private;

    /* ----- */
    void (*move)(Mino_class*, DIRECTION);
    void (*kickMove)(Mino_class*, int, int);
    void (*rotate)(Mino_class*, DIRECTION);
    void (*swap)(Mino_class*, Mino_class*);
    void (*minoLocateInit)(Mino_class*);

    int (*getMinoWidth)(Mino_class*);

    uint16_t (*getMino)(Mino_class*);

    Mino_Type (*getMtype)(Mino_class*);
    Rotate_Type (*getRtype)(Mino_class*);
    int (*getX)(Mino_class*);
    int (*getY)(Mino_class*);
    kickTable* (*getKickTable)(Mino_class*);
};

Mino_class* newMino();
void destroyMino(Mino_class*);

extern const int SRS_OTHER[ANGLE_MAX][ANGLE_MAX][TABLE_MAX][2];
extern const int SRS_I[ANGLE_MAX][ANGLE_MAX][TABLE_MAX][2];


#endif
