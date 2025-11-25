#include "config.h"
#include "Shape.h"
#include "Mino.h"

struct _mino {
    int x, y;
    Mino_Type mtype;
    Rotate_Type rtype;
    
    const MinoShape* shape;

    const int (*kickTable)[ANGLE_MAX][TABLE_MAX][2];
};

const int SRS_OTHER[ANGLE_MAX][ANGLE_MAX][TABLE_MAX][2] = {
    { // ANGLE_0 -> 
        // ANGLE_0
        { {0,0}, {0,0}, {0,0}, {0,0}, {0,0} },
        // ANGLE_90
        { {0,0}, {-1,0}, {-1,1}, {0,-2}, {-1,-2} },
        // ANGLE_180
        { {0,0}, {0,0}, {0,0}, {0,0}, {0,0} },
        // ANGLE_270
        { {0,0}, {1,0}, {1,1}, {0,-2}, {1,-2} }
    },
    { // ANGLE_90 ->
        // ANGLE_0
        { {0,0}, {1,0}, {1,-1}, {0,2}, {1,2} },
        // ANGLE_90
        { {0,0}, {0,0}, {0,0}, {0,0}, {0,0} },
        // ANGLE_180
        { {0,0}, {1,0}, {1,-1}, {0,2}, {1,2} },
        // ANGLE_270
        { {0,0}, {0,0}, {0,0}, {0,0}, {0,0} },

    },
    { // ANGLE_180 ->
        // ANGLE_0
        { {0,0}, {0,0}, {0,0}, {0,0}, {0,0} },
        // ANGLE_90
        { {0,0}, {-1,0}, {-1,1}, {0,-2}, {-1,-2} },
        // ANGLE_180
        { {0,0}, {0,0}, {0,0}, {0,0}, {0,0} },
        // ANGLE_270
        { {0,0}, {1,0}, {1,1}, {0,-2}, {1,-2} }

    },
    { // NAGLE_270 ->
        // ANGLE_0
        { {0,0}, {-1,0}, {-1,-1}, {0,2}, {-1,2} },
        // ANGLE_90
        { {0,0}, {0,0}, {0,0}, {0,0}, {0,0} },
        // ANGLE_180
        { {0,0}, {-1,0}, {-1,-1}, {0,2}, {-1,2} },
        // ANGLE_270
        { {0,0}, {0,0}, {0,0}, {0,0}, {0,0} }

    }
    
};

const int SRS_I[ANGLE_MAX][ANGLE_MAX][TABLE_MAX][2] = {
    { // ANGLE_0 -> 
        // ANGLE_0
        { {0,0}, {0,0}, {0,0}, {0,0}, {0,0} },
        // ANGLE_90
        { {0,0}, {-2,0}, {-1,0}, {-2,-1}, {1,2} },
        // ANGLE_180
        { {0,0}, {0,0}, {0,0}, {0,0}, {0,0} },
        // ANGLE_270
        { {0,0}, {-1,0}, {2,0}, {-1,2}, {2,-1} }
    },
    { // ANGLE_90 ->
        // ANGLE_0
        { {0,0}, {2,0}, {-1,0}, {2,1}, {-1,-2} },
        // ANGLE_90
        { {0,0}, {0,0}, {0,0}, {0,0}, {0,0} },
        // ANGLE_180
        { {0,0}, {-1,0}, {2,0}, {-1,2}, {2,-1} },
        // ANGLE_270
        { {0,0}, {0,0}, {0,0}, {0,0}, {0,0} }

    },
    { // ANGLE_180 ->
        // ANGLE_0
        { {0,0}, {0,0}, {0,0}, {0,0}, {0,0} },
        // ANGLE_90
        { {0,0}, {1,0}, {-2,0}, {1,-2}, {-2,1} },
        // ANGLE_180
        { {0,0}, {0,0}, {0,0}, {0,0}, {0,0} },
        // ANGLE_270
        { {0,0}, {2,0}, {-1,0}, {2,1}, {-1,-2} },

    },
    { // NAGLE_270 ->
        // ANGLE_0
        { {0,0}, {1,0}, {-2,0}, {1,-2}, {-2,1} },
        // ANGLE_90
        { {0,0}, {0,0}, {0,0}, {0,0}, {0,0} },
        // ANGLE_180
        { {0,0}, {-2,0}, {1,0}, {-2,-1}, {1,2} },
        // ANGLE_270
        { {0,0}, {0,0}, {0,0}, {0,0}, {0,0} }

    }
};

// プロトタイプ宣言
static int getMinoWidth(Mino_class*);
static uint16_t getMino(Mino_class*);
static Mino_Type getMtype(Mino_class*);
static Rotate_Type getRtype(Mino_class*);
static int getX(Mino_class*);
static int getY(Mino_class*);
kickTable* getKickTable(Mino_class*);

/**
 * ミノを移動させます.
 */
static void move(Mino_class* self, DIRECTION dir)
{
    switch(dir) {
        case UP: self->private->y--; break;
        case DOWN: self->private->y++; break;
        case LEFT: self->private->x--; break;
        case RIGHT: self->private->x++; break;
        default: break;
    }
}

/**
 * キックテーブル用の移動メソッドです.
 */
static void kickMove(Mino_class* self, int dx, int dy)
{
    self->private->x += dx;
    self->private->y += dy;
}

/**
 * ミノを回転させます.
 */
static void rotate(Mino_class* self, DIRECTION dir)
{
    switch(dir) {
        case LEFT: 
            self->private->rtype = (self->private->rtype + 3) % ANGLE_MAX;
            break;
        case RIGHT:
            self->private->rtype = (self->private->rtype + 1) % ANGLE_MAX;
            break;
        default: break;
    }
}

/**
 * ミノをスワップします.
 */
static void swap(Mino_class* self, Mino_class* subject)
{
    if(self == NULL || subject == NULL  
        || self->private == NULL || subject->private == NULL) return;
    
    Mino tmp = *(self->private);
    *(self->private) = *(subject->private);
    *(subject->private) = tmp;
}

static void minoLocateInit(Mino_class* self) 
{
    int width = getMinoWidth(self);
    self->private->x = (FILED_WIDTH - width) / 2;
    self->private->y = 0;
    self->private->rtype = ANGLE_0;
}

/**
 * ミノのスポーン位置を調整するために使用するミノの幅を返す関数です.
 * privateです.
 */
static int getMinoWidthFromShape(const MinoShape* shape, Rotate_Type rtype)
{
    uint16_t mask = shape->rotations[rtype];
    int minX = MINO_WIDTH-1;
    int maxX = 0;

    for(int x = 0; x < MINO_WIDTH; x++) {
        for(int y = 0; y < MINO_HEIGHT; y++) {
            if(mask & (1 << (15 - (y*4 + x)))) {
                if(x < minX) minX = x;
                if(x > maxX) maxX = x;
            }
        }
    }
    return (maxX - minX + 1);
}

/**
 * getMinoWidthFromShapeのラッパーです.
 */
static int getMinoWidth(Mino_class* self)
{
    return getMinoWidthFromShape(self->private->shape, self->private->rtype);
}

/**
 * コンストラクタです.
 */
Mino_class* newMino()
{
    Mino_class* self = malloc(sizeof(Mino_class));
    self->private = malloc(sizeof(Mino));

    self->move = move;
    self->kickMove = kickMove;
    self->rotate = rotate;
    self->swap = swap;
    self->minoLocateInit = minoLocateInit;

    self->getMinoWidth = getMinoWidth;
    self->getMino = getMino;
    self->getMtype = getMtype;
    self->getRtype = getRtype;
    self->getX = getX;
    self->getY = getY;
    self->getKickTable = getKickTable;

    self->private->mtype = rand() % MINO_TYPE_MAX;
    self->private->shape = &SHAPES[self->private->mtype];
    self->private->rtype = ANGLE_0;
    self->private->kickTable = self->private->mtype == MINO_I ? SRS_I : SRS_OTHER;

    int width = getMinoWidth(self);
    self->private->x = (FILED_WIDTH - width) / 2;
    self->private->y = -2;

    return self;
}

/**
 * デストラクタです.
 */
void destroyMino(Mino_class* self)
{
    free(self->private);
    free(self);
}

// 以下ゲッター, セッター

/**
 * ミノのbit列を取り出します.
 */
static uint16_t getMino(Mino_class* self)
{
    return self->private->shape->rotations[self->private->rtype];
}

/**
 * ミノの形状を取り出します.
 */
static Mino_Type getMtype(Mino_class* self)
{
    return self->private->mtype;
}

/**
 * ミノの回転状態を取り出します.
 */
static Rotate_Type getRtype(Mino_class* self)
{
    return self->private->rtype;
}

/**
 * ミノのx座標を取り出します.
 */
static int getX(Mino_class* self)
{
    return self->private->x;
}

/**
 * ミノのy座標を取り出します.
 */
static int getY(Mino_class* self)
{
    return self->private->y;
}

/**
 * キックテーブルのゲッターです.
 */
kickTable* getKickTable(Mino_class* self)
{
    return self->private->kickTable;
}
