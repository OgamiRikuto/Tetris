#ifndef MODEL_H
#define MODEL_H

typedef struct manager_class Manager_class;

#include "config.h"
#include "Title.h"
#include "Field.h"
#include "Mino.h"

#include <stdlib.h>
#include <stdbool.h>

typedef enum {
    SCENE_NONE = 0,
    TITLE_SCENE,
    GAME_SCENE,
    SCENE_MAX
}SceneType;

// スーパークラス Model_class -----------------
typedef struct _model Model;
typedef struct model_class Model_class;

struct model_class{
    Model* private;

    /* ----- */
    void (*init)(void*); // abstract
    void (*update)(void*); // abstract
    Model_class* (*createNextSceneModel)(void*, SceneType);
    void (*destroy)(void*); // abstract

    SceneType (*nextScene)(Model_class*);
    void (*setNextScene)(Model_class*, SceneType);
    bool (*quit)(Model_class*);
    void (*setQuit)(Model_class*, bool);
};

void newModel(Model_class*, Manager_class*);
void destroyModel(Model_class*);
// -----------------------------------------

// サブクラス GameModel_class ------------------
typedef struct _gameModel GameModel;
typedef struct gameModel_class GameModel_class;

struct gameModel_class {
    Model_class super;
    GameModel* private;

    /* ----- */
    Field_class* (*getField)(GameModel_class*);
    Mino_class* (*getCurrentMino)(GameModel_class*);
    Mino_class* (*getHoldMino)(GameModel_class*);
    Mino_class** (*getNextMinos)(GameModel_class*);
    int (*getScore)(GameModel_class*);
};

GameModel_class* newGameModel(Manager_class*);
void destroyGameModel(GameModel_class*);
// ---------------------------------------------

// サブクラス TitleModel_class ------------------
typedef struct _titleModel TitleModel;
typedef struct titleModel_class TitleModel_class;

struct titleModel_class {
    Model_class super;
    TitleModel* private;

    /* ----- */
    SELECT_SCENE (*nowScene)(TitleModel_class*);
};

TitleModel_class* newTitleModel(Manager_class*);
void destroyTitleModel(TitleModel_class*);
// ---------------------------------------------

/*
// サブクラス ScoreModel_class ------------------
typedef struct _scoreModel ScoreModel;
typedef struct scoreModel_class ScoreModel_class;

struct scoreModel_class {
    Model_class super;
    ScoreModel* private;
};

ScoreModel_class* newScoreModel(Manager_class*, int);
void destroyScoreModel(ScoreModel_class*);
// ---------------------------------------------
*/
#endif
