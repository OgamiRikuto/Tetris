#ifndef CONFIG_H
#define CONFIG_H

#include <time.h>

#define FILED_WIDTH 10
#define FILED_HEIGHT 20
#define FLOOR_INDEX FILED_HEIGHT

#define MINO_WIDTH 4
#define MINO_HEIGHT 4

#define DRAW_MINO_WIDTH (MINO_HEIGHT+2)
#define DRAW_QUEUE_HIGHT (MINO_HEIGHT+1)

#define QUEUE_MAX 5

#define UPDATE_SECOND 1.0
#define UPDATE_TIME ( UPDATE_SECOND / CLOCKS_PER_SEC )
#define FALL_SPEED 1.0
#define LOCK_SPEED 0.5

#define PATH_MAX 1028
#define MAX_NAME_LEN 10
#define MAX_SOCRE 10

#define BONUS 100
#define SCORE_CORRCETION 100
#define MAX_LINE_DELETE 4

#define shiftBit(x) (FILED_WIDTH - MINO_WIDTH + 1 - x)
//#define shiftBit(x) (x + 1)

typedef enum {
    UP = 0,
    DOWN,
    LEFT,
    RIGHT,
    DIRECTION_MAX
}DIRECTION;

#endif
