#ifndef VIEW_H
#define VIEW_H

typedef struct model_class Model_class;
typedef struct gameModel_class GameModel_class;
typedef struct titleModel_class TitleModel_class;
typedef struct scoreModel_class ScoreModel_class;

#include <stdlib.h>

// スーパークラス　View_class -------------
typedef struct _view View;
typedef struct view_class View_class;

struct view_class{
    View* private;

    /* ----- */
    void (*draw)(void*); // abstract
};

void newView(View_class*, Model_class*);
void destroyView(View_class*);
// -----------------------------------

// サブクラス GameView_class ------------------
typedef struct gameView_class GameView_class;

struct gameView_class {
    View_class super;
};

GameView_class* newGameView(GameModel_class*);
void destroygameView(GameView_class*);
// --------------------------------------------

// サブクラス TitleView_class ------------------
typedef struct titleView_class TitleView_class;

struct titleView_class {
    View_class super;
};

TitleView_class* newTitleView(TitleModel_class*);
void destroytitleView(TitleView_class*);
// --------------------------------------------

/*
// サブクラス ScoreView_class ------------------
typedef struct scoreView_class ScoreView_class;

struct scoreView_class {
    View_class super;
};

ScoreView_class* newScoreView(ScoreModel_class*);
void destroyscoreView(ScoreView_class*);
// --------------------------------------------
*/
#endif
