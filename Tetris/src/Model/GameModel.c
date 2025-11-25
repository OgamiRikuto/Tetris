#include "config.h"
#include "Model.h"
#include "Model_private.h"
#include "View.h"
#include "Controller.h"
#include "Manager.h"
#include "Field.h"
#include "Mino.h"
#include "io.h"

#include <time.h>
#include <stdbool.h>

typedef enum {
    GAME_MAIN = 0,
    STATE_CHECK_LINE,
    STATE_SPAWN,
    STATE_MAX
}GAME_STATE;

struct _gameModel {
    Field_class* aField;
    Mino_class* currentMino;
    Mino_class* holdMino;
    Mino_class* nextMinos[QUEUE_MAX];
    bool isGameOver;
    int score;
    bool canHold;
    double fallTimer;
    double lockTimer;
    double timerInterval;
    GAME_STATE state;
};

// プロトタイプ宣言
static Field_class* getField(GameModel_class*);
static Mino_class* getCurrentMino(GameModel_class*);
static Mino_class* getHoldMino(GameModel_class*);
static Mino_class** getNextMinos(GameModel_class*);
static int getScore(GameModel_class*);
static void spawnMino(GameModel_class*);
static void hold(GameModel_class*);
static void drop(GameModel_class*);

static void init(void* self_ptr) 
{
    GameModel_class* self = (GameModel_class*)self_ptr;
    self->super.setNextScene(&self->super, SCENE_NONE);

    GameView_class* aView = (GameView_class*)getView(&self->super);
    aView->super.draw(aView);
}

/**
 * 更新します。
 */
static void update(void* self_ptr)
{
    GameModel_class* self = (GameModel_class*)self_ptr;
    bool isDraw = false;
    bool grounded = false;

    // キー入力
    GameController_class* aController = (GameController_class*)getController(&self->super);
    aController->super.handleInput(aController);

    KeyType key = aController->super.keyState(&aController->super);

    switch(key) {
        case MYKEY_UP:  
            drop(self); 
            self->private->state = STATE_CHECK_LINE;
            isDraw = true; 
            break;
        case MYKEY_DOWN: 
            grounded = self->private->aField->tryMove(self->private->aField, self->private->currentMino, DOWN); 
            isDraw = grounded;
            break;
        case MYKEY_LEFT: isDraw = self->private->aField->tryMove(self->private->aField, self->private->currentMino, LEFT); break;
        case MYKEY_RIGHT: isDraw = self->private->aField->tryMove(self->private->aField, self->private->currentMino, RIGHT); break;
        case MYKEY_X: isDraw = self->private->aField->tryRotate(self->private->aField, self->private->currentMino, RIGHT); break;
        case MYKEY_Z: isDraw = self->private->aField->tryRotate(self->private->aField, self->private->currentMino, LEFT); break;
        case MYKEY_SPACE: hold(self); isDraw = true; break;
        case MYKEY_ESCAPE: self->super.setQuit(&self->super, true); break;
        default: break;
    }

    if(self->super.quit(&self->super)) return;

    // 重力落下
    Manager_class* aManager = getManager(&self->super);
    double deltaTime = aManager->deltaTime(aManager);
    
    if(grounded) {
        isDraw = true;
        self->private->lockTimer += deltaTime;
        if(self->private->lockTimer >= LOCK_SPEED) {
            self->private->aField->fixMino(self->private->aField, self->private->currentMino);
            self->private->state = STATE_CHECK_LINE;
            self->private->lockTimer = 0.0;
            
        }
    } else self->private->lockTimer = 0.0;
     

    self->private->fallTimer += deltaTime;
    if(self->private->fallTimer >= self->private->timerInterval) {
        if(!self->private->aField->tryMove(self->private->aField, self->private->currentMino, DOWN)) {
            self->private->aField->fixMino(self->private->aField, self->private->currentMino);
            self->private->state = STATE_CHECK_LINE;
        }
        isDraw = true;
        self->private->fallTimer = 0.0;
    }

    // ライン消去
    if(self->private->state == STATE_CHECK_LINE) {
        int score = self->private->aField->clearLine(self->private->aField);
        self->private->score += score;
        self->private->state = STATE_SPAWN;
        isDraw = true;
    }
    // 新ミノ生成
    if(self->private->state == STATE_SPAWN) {
        spawnMino(self);
        if(self->private->aField->hitCollision(self->private->aField, self->private->currentMino))
            self->private->isGameOver = true;
        else self->private->state = GAME_MAIN;
        isDraw = true;
    }

    if(self->private->isGameOver) {
        IO_printf("GAME OVER...");
        IO_sleep(3000);
        self->super.setNextScene(&self->super, TITLE_SCENE);
    }

    if(self->super.quit(&self->super)) return;

    if(isDraw) {
        GameView_class* aView = (GameView_class*)getView(&self->super);
        aView->super.draw(aView); 
    }
}

/**
 * 次のシーンのモデルのインスタンスを作成して返します.
 */
static Model_class* createNextSceneModel(void* self_ptr, SceneType next)
{
    GameModel_class* self = (GameModel_class*)self_ptr;

    switch(next) {
        case TITLE_SCENE:
            return (Model_class*)newTitleModel(getManager(&self->super));
        default: return NULL;
    }
}

/**
 * 破棄します.
 */
static void destroy(void* self_ptr) 
{
    GameModel_class* self = (GameModel_class*)self_ptr;
    destroyGameModel(self);
}

/**
 * ミノをキューから取り出し、フィールド上にスポーンさせます。
 */
static void spawnMino(GameModel_class* self)
{
    self->private->currentMino->swap(self->private->currentMino, self->private->nextMinos[0]);
    for(int i=0; i<QUEUE_MAX-1; i++) {
        self->private->nextMinos[i]->swap(self->private->nextMinos[i], self->private->nextMinos[i+1]);
    }
    destroyMino(self->private->nextMinos[QUEUE_MAX-1]);
    self->private->nextMinos[QUEUE_MAX-1] = newMino();

    self->private->canHold = true;
}

/**
 * ミノをホールドします。
 */
static void hold(GameModel_class* self)
{
    Mino_class* current = self->private->currentMino;
    Mino_class* hold = self->private->holdMino;

    if(!self->private->canHold) return;

    if(hold == NULL) {
        hold = newMino();
        self->private->holdMino = hold;
        hold->swap(hold, current);

        spawnMino(self);
    } else {
        hold->swap(hold, current);
    }
        
    self->private->currentMino->minoLocateInit(self->private->currentMino);

    self->private->canHold = false;
}

/**
 * ミノを一番下まで落下させます。
 */
static void drop(GameModel_class* self)
{
    while(self->private->aField->tryMove(self->private->aField, self->private->currentMino, DOWN)) {}
    self->private->aField->fixMino(self->private->aField, self->private->currentMino);
}

/**
 * コンストラクタです。
 */
GameModel_class* newGameModel(Manager_class* aManeger)
{
    GameModel_class* self = malloc(sizeof(GameModel_class));
    newModel(&self->super, aManeger);
    self->private = malloc(sizeof(GameModel));

    self->super.init = init;
    self->super.update = update;
    self->super.createNextSceneModel = createNextSceneModel;
    self->super.destroy = destroy;

    self->getField = getField;
    self->getCurrentMino = getCurrentMino;
    self->getHoldMino = getHoldMino;
    self->getNextMinos = getNextMinos;
    self->getScore = getScore;

    GameView_class* aView = newGameView(self);
    setView(&self->super, (View_class*)aView);

    GameController_class* aController = newGameController(self);
    setController(&self->super, (Controller_class*)aController);

    self->private->aField = newField();;
    self->private->currentMino = newMino();
    self->private->holdMino = NULL;
    for(int i=0; i<QUEUE_MAX; i++) {
        self->private->nextMinos[i] = newMino();
    }
    self->private->isGameOver = false;
    self->private->score = 0;
    self->private->canHold = true;
    self->private->fallTimer = 0.0;
    self->private->lockTimer = 0.0;
    self->private->timerInterval = FALL_SPEED;
    self->private->state = GAME_MAIN;

    return self;
}

/**
 * デストラクタです。
 */
void destroyGameModel(GameModel_class* self)
{
    destroyField(self->private->aField);
    destroyMino(self->private->currentMino);
    if(self->private->holdMino != NULL) destroyMino(self->private->holdMino);
    for(int i=0; i<QUEUE_MAX; i++) {
        destroyMino(self->private->nextMinos[i]);
    } 
    free(self->private);
    destroyModel(&self->super);
    free(self);
}


// 以下ゲッター

/**
 * フィールドクラス ゲッター
 */
static Field_class* getField(GameModel_class* self)
{
    return self->private->aField;
}

/**
 * カレントミノ ゲッター
 */
static Mino_class* getCurrentMino(GameModel_class* self)
{
    return self->private->currentMino;
}

/**
 * ホールドミノ ゲッター
 */
static Mino_class* getHoldMino(GameModel_class* self)
{
    return self->private->holdMino;
}

/**
 * ネクストミノキュー ゲッター
 */
static Mino_class** getNextMinos(GameModel_class* self)
{
    return self->private->nextMinos;
}

/**
 * スコア ゲッター
 */
static int getScore(GameModel_class* self)
{
    return self->private->score;
}
