#include "config.h"
#include "Field.h"
#include "Mino.h"
#include "io.h"

struct _field {
    uint16_t filed[FILED_HEIGHT+1];
};

// プロトタイプ宣言
static uint16_t* getField(Field_class*);

/**
 * ミノの1行を取り出します.
 */
static uint16_t minoLine(uint16_t mino, int y)
{
    return (mino >> y*4) & 0xF;
}

/**
 * フィールドとミノの当たり判定をチェックし,当たりだったら真,それ以外は偽を返します.
 */
static bool hitCollision(Field_class* self, Mino_class* mino)
{
    uint16_t minoBit = mino->getMino(mino);
    int minoX = mino->getX(mino);
    int minoY = mino->getY(mino);

    for(int y = 0; y < MINO_HEIGHT; y++) {
        uint16_t minoRow = minoLine(minoBit, y);

        if(minoRow == 0) continue;

        int checkY = minoY + y;

        if(checkY < 0 || checkY > FLOOR_INDEX) continue;

        int s = shiftBit(minoX);
        uint16_t shiftedMinoRow;

        if(s > 0) shiftedMinoRow = minoRow << s;
        else if(s == 0) shiftedMinoRow = minoRow;
        else shiftedMinoRow = minoRow >> (-s);
        
        uint16_t fieldRow = self->private->filed[checkY];

        if((fieldRow & shiftedMinoRow) != 0) return true;
    }
    return false;
}

/**
 * 引数と反対のDIRECTIONを返します.
 */
static DIRECTION oppoDir(DIRECTION dir) 
{
    switch(dir) {
        case UP: return DOWN;
        case DOWN: return UP;
        case LEFT: return RIGHT;
        case RIGHT: return LEFT;
        default: return dir;
    }
}

/**
 * 移動を試みて,できるなら真を返します.
 */
static bool tryMove(Field_class* self, Mino_class* mino, DIRECTION dir)
{
    mino->move(mino, dir);

    if(!hitCollision(self, mino))   return true;
    mino->move(mino, oppoDir(dir));
    return false;
}

/**
 * 回転を試みて,できるなら真を返します.
 */
static bool tryRotate(Field_class* self, Mino_class* mino, DIRECTION dir)
{
    Rotate_Type from = mino->getRtype(mino);
    Rotate_Type to;
    if(dir == LEFT)
        to = (mino->getRtype(mino) + 3) % ANGLE_MAX;
    else 
        to = (mino->getRtype(mino) + 1) % ANGLE_MAX;

    kickTable* kickTable = mino->getKickTable(mino);

    mino->rotate(mino, dir);
    for(int i=0; i<TABLE_MAX; i++) {
        int dx = kickTable[from][to][i][0];
        int dy = kickTable[from][to][i][1];

        mino->kickMove(mino, dx, dy);

        if(!hitCollision(self, mino)) return true;

        mino->kickMove(mino, -dx, -dy);
    }
    mino->rotate(mino, oppoDir(dir));
    return false;
}

/**
 * フィールドにミノをセットします.
 */
static void fixMino(Field_class* self, Mino_class* mino)
{
    uint16_t minoBit = mino->getMino(mino);
    int minoX = mino->getX(mino);
    int minoY = mino->getY(mino);

    for(int y = 0; y < MINO_HEIGHT; y++) {
        uint16_t minoRow = minoLine(minoBit, y);

        if(minoRow == 0) continue;

        int checkY = minoY + y;

        if(checkY < 0 || checkY > FLOOR_INDEX) continue;

        int s = shiftBit(minoX);
        uint16_t shiftedMinoRow;

        if(s > 0) shiftedMinoRow = minoRow << s;
        else if(s == 0) shiftedMinoRow = minoRow;
        else shiftedMinoRow = minoRow >> (-s);

        self->private->filed[minoY+y] |= shiftedMinoRow;
    }
}

/**
 * フィールド上に揃ったラインを削除します.
 * 削除されたラインの数に応じて,加算されるスコアを返します.
 */
static int clearLine(Field_class* self)
{
    int score = 0;

    const uint16_t wall_mask = 0x801;
    const uint16_t full_row = 0x07FE;
    for(int i=FILED_HEIGHT-1; i>=0; i--) {
        if((self->private->filed[i] & full_row) == full_row) {
            score++;
            self->private->filed[i] = wall_mask;
            for(int j = i; j > 0; j--) {
                self->private->filed[j] = self->private->filed[j-1];
            }
            i++;
            self->private->filed[0] = wall_mask;
        }
    }
    return score == MAX_LINE_DELETE ? score*SCORE_CORRCETION + BONUS : score*SCORE_CORRCETION;
}

/**
 * コンストラクタです.
 */
Field_class* newField()
{
    Field_class* self = malloc(sizeof(Field_class));
    self->private = malloc(sizeof(Field));

    self->hitCollision = hitCollision;
    self->tryMove = tryMove;
    self->tryRotate = tryRotate;
    self->clearLine = clearLine;
    self->fixMino = fixMino;

    self->getField = getField;

    // 壁を含めたフィールドの初期設定
    for(int i=0; i<FILED_HEIGHT; i++) {
        self->private->filed[i] = 0x801;
    }
    // 床の設定
    self->private->filed[FLOOR_INDEX] = 0xFFF;

    return self;
}

/**
 * デストラクタです.
 */
void destroyField(Field_class* self)
{
    free(self->private);
    free(self);
}

// 以下ゲッター
static uint16_t* getField(Field_class* self)
{
    return self->private->filed;
}
