#ifndef FIELD_H
#define FIELD_H

#include <stdint.h>
#include <stdbool.h>

typedef struct mino_class Mino_class;

typedef struct _field Field;
typedef struct field_class Field_class;


struct field_class{
    Field* private;

    /* ----- */
    bool (*hitCollision)(Field_class*, Mino_class*);
    bool (*tryMove)(Field_class*, Mino_class*, DIRECTION);
    bool (*tryRotate)(Field_class*, Mino_class*, DIRECTION);
    void (*fixMino)(Field_class*, Mino_class*);
    int (*clearLine)(Field_class*);

    uint16_t* (*getField)(Field_class*);
};

Field_class* newField();
void destroyField(Field_class*);

#endif
