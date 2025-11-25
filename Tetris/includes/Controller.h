#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "io.h"

#include <stdlib.h>
#include <stdbool.h>

typedef struct model_class Model_class;
typedef struct gameModel_class GameModel_class;
typedef struct titleModel_class TitleModel_class;
typedef struct scoreModel_class ScoreModel_class;


// スーパークラス Controller_class -----------------------
typedef struct _controller Controller;
typedef struct controller_class Controller_class;

struct controller_class{
    Controller* private;

    /* ----- */
    void (*handleInput)(void*); // abstract

    KeyType (*keyState)(Controller_class*);
    void (*setKey)(Controller_class*, KeyType);
};
void newController(Controller_class*, Model_class*);
void destroyController(Controller_class*);
// ------------------------------------------------------

// サブクラス GameController_class ------------------------
typedef struct _gameController GameController;
typedef struct gameController_class GameController_class;

struct gameController_class {
    Controller_class super;
};

GameController_class* newGameController(GameModel_class*);
void destroyGameController(GameController_class*);
// -------------------------------------------------------

// サブクラス TitleController_class ------------------------
typedef struct _titleController TitleController;
typedef struct titleController_class TitleController_class;

struct titleController_class {
    Controller_class super;
};

TitleController_class* newTitleController(TitleModel_class*);
void destroyTitleController(TitleController_class*);
// -------------------------------------------------------

/*
// サブクラス ScoreController_class ------------------------
typedef struct _scoreController ScoreController;
typedef struct scoreController_class ScoreController_class;

struct scoreController_class {
    Controller_class super;

    void (*inputeName)(ScoreController_class*);
};

ScoreController_class* newScoreController(ScoreModel_class*);
void destroyScoreController(ScoreController_class*);
// -------------------------------------------------------
*/
#endif
