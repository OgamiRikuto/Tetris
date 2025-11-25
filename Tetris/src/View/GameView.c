#include "config.h"
#include "View.h"
#include "View_private.h"
#include "Model.h"
#include "Field.h"
#include "Mino.h"
#include "io.h"

#include <stdbool.h>

/* 目指す形
⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎　　　　　　　　　　⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎
　　⬜︎　　　⬛︎　　　　　　　　　　⬛︎　　⬜︎　　　⬛︎　　⬜︎⬜︎　　⬛︎
　　⬜︎　　　⬛︎　　　　　　　　　　⬛︎　⬜︎⬜︎⬜︎　　⬛︎　　⬜︎⬜︎　　⬛︎
　　⬜︎　　　⬛︎　　　　　　　　　　⬛︎　　　　　　⬛︎　　　　　　⬛︎
　　⬜︎　　　⬛︎　　　　　　　　　　⬛︎　　　　　　⬛︎　　　　　　⬛︎
⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎　　　　　　　　　　⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎
⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎　　　　　　　　　　⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎　⬜︎⬜︎　　　⬛︎
⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎　　　　　　　　　　⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎　　⬜︎⬜︎　　⬛︎
⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎　　　　　　　　　　⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎　　　　　　⬛︎
⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎　　　　　　　　　　⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎　　　　　　⬛︎
⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎　　　　　　　　　　⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎
⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎　　　　　　　　　　⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎　⬜︎　　　　⬛︎
⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎　　　　　　　　　　⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎　⬜︎　　　　⬛︎
⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎　　　　　　　　　　⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎　⬜︎⬜︎　　　⬛︎
⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎　　　　　　　　　　⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎　　　　　　⬛︎
⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎　　　　　　　　　　⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎
⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎　　　　　　　　　　⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎　　⬜︎　　　⬛︎
⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎　　　　　　　　　　⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎　　⬜︎　　　⬛︎
⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎　　　　　　　　　　⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎　⬜︎⬜︎　　　⬛︎
⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎　　　　　　　　　　⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎　　　　　　⬛︎
⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎

*/

/**
 * ミノの1行を取り出します.
 */
static uint16_t minoLine(uint16_t mino, int y)
{
    return (mino >> y*4) & 0xF;
}

/**
 * ミノのx座標にミノがあるかを判定します.
 */
static bool isFilled(uint16_t mino, int x)
{
    uint16_t mask = mino >> (MINO_WIDTH - x - 1);
    return mask & 0b1;
}

/**
 * ミノを描きます.
 */
static void drawMino(uint16_t bit)
{
    for(int i = 0; i < MINO_WIDTH; i++) {
        isFilled(bit, i) ?  IO_printf("⬜︎") : IO_printf("　");
    }
    return;
}

/**
 * ホールド部分を描画します.
 */
static void drawHold(Mino_class* mino, int y)
{
    if(y == 0 || y > MINO_HEIGHT) {
        for(int i = 0; i < DRAW_MINO_WIDTH; i++) {
            IO_printf("⬛︎");
        }
        return;
    }
    IO_printf("⬛︎");
    if(y >= 1 && y <= MINO_HEIGHT) {
        uint16_t minoBit;
        if(mino != NULL) {
             minoBit = minoLine(mino->getMino(mino), y-1);
        } else {
            minoBit = 0x0000;
        }

        drawMino(minoBit);
    }
    IO_printf("⬛︎");
}

/**
 * フィールド部分を描画します.
 */
static void drawField(uint16_t* field, int y)
{
    uint16_t buffer = field[y];
    IO_printf("⬛︎");
    for(int i=0; i<FILED_WIDTH; i++) {
        if(y == FLOOR_INDEX) {IO_printf("⬛︎"); continue;}
        uint16_t mask = buffer >> (FILED_WIDTH+1 - i - 1);
        (mask & 0b1) ? IO_printf("⬜︎") : IO_printf("　");
    }
    IO_printf("⬛︎");
}

/**
 * ネクストミノ部分を描画します.
 */
static void drawNext(Mino_class* mino, int y)
{
    if(y == 0 || y > MINO_HEIGHT) {
        for(int i = 0; i < DRAW_MINO_WIDTH; i++) {
            IO_printf("⬛︎");
        }
        return;
    }
    IO_printf("⬛︎");
    if(y >= 1 && y <= MINO_HEIGHT) {
        uint16_t minoBit = minoLine(mino->getMino(mino), y-1);
        drawMino(minoBit);
    }
    IO_printf("⬛︎");
}

/**
 * キュー部分を描画します.
 */
static void drawQueue(Mino_class** minos, int y)
{
    if(y % DRAW_QUEUE_HIGHT == 0) {
        for(int i = 0; i < DRAW_MINO_WIDTH; i++) {
            IO_printf("⬛︎");
        }
        return;
    }
    IO_printf("⬛︎");
    int minoNo = y / DRAW_QUEUE_HIGHT + 1;
    uint16_t minoBit = minoLine(minos[minoNo]->getMino(minos[minoNo]), y % DRAW_QUEUE_HIGHT - 1);
    drawMino(minoBit);
    IO_printf("⬛︎");
}

/**
 * 描画用のミノを追加したフィールドを作成します.
 */
static uint16_t* makeField(uint16_t* field, Mino_class* mino)
{
    static uint16_t buffer[FILED_HEIGHT+1];
    for(int i=0; i<FILED_HEIGHT+1; i++) {
        buffer[i] = field[i];
    }
    uint16_t minoBit = mino->getMino(mino);
    int minoX = mino->getX(mino);
    int minoY = mino->getY(mino);
    

    for(int y = 0; y < MINO_HEIGHT; y++) {
        if(minoY + y < 0 || minoY + y > FLOOR_INDEX) continue;
        uint16_t minoRow = minoLine(minoBit, y);

        int s = shiftBit(minoX);
        uint16_t shiftedMinoRow;

        if(s > 0) shiftedMinoRow = minoRow << s;
        else if(s == 0) shiftedMinoRow = minoRow;
        else shiftedMinoRow = minoRow >> (-s);

        buffer[minoY+y] |= shiftedMinoRow;
    }
    return buffer;
}

/**
 * 描画します
 */
static void draw(void* self_ptr)
{
    IO_clear();
    GameView_class* self = (GameView_class*)self_ptr;
    GameModel_class* aModel = (GameModel_class*)getModel(&self->super);
    Field_class* field = aModel->getField(aModel);
    Mino_class* current = aModel->getCurrentMino(aModel);
    Mino_class* hold = aModel->getHoldMino(aModel);
    Mino_class** nextQueue = aModel->getNextMinos(aModel);
    int score = aModel->getScore(aModel);

    uint16_t* fields = makeField(field->getField(field), current);
    Mino_class* next = nextQueue[0];

    for(int y = 0; y < FILED_HEIGHT + 1; y++) {
        drawHold(hold, y);
        drawField(fields, y);
        drawNext(next, y);
        drawQueue(nextQueue, y);
        IO_printf("\n");
    }
    IO_printf("Score: %d\n", score);
}

/**
 * コンストラクタです。
 */
GameView_class* newGameView(GameModel_class* aModel)
{
    GameView_class* self = malloc(sizeof(GameView_class));
    newView(&self->super, &aModel->super);

    self->super.draw = draw;

    return self;
}

/**
 * デストラクタです。
 */
void destroyGameView(GameView_class* self)
{
    free(self);
}
