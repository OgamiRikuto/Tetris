#include "config.h"
#include "Manager.h"
#include "Model.h"
#include "View.h"
#include "Controller.h"
#include "Model_private.h"
#include "Title.h"
#include "io.h"

struct _titleModel{
    SELECT_SCENE nowScene;
};

// ゲッター, セッタープロトタイプ宣言
static SELECT_SCENE nowScene(TitleModel_class*);

static void init(void* self_ptr) 
{
    TitleModel_class* self = (TitleModel_class*)self_ptr;
    self->super.setNextScene(&self->super, SCENE_NONE);
    TitleView_class* aView = (TitleView_class*)getView(&self->super);
    aView->super.draw(aView);
}

/**
 * エンターキーが押された時の挙動を定義します.
 */
static void handleEnterKey(TitleModel_class* self)
{
    switch(self->private->nowScene) {
        case GAME:
            self->super.setNextScene(&self->super, GAME_SCENE);
            break;
        case EXIT:
            self->super.setQuit(&self->super, true);
            break;
        default: break;
    }
}

/**
 * 更新します。
 */
static void update(void* self_ptr)
{
    TitleModel_class* self = (TitleModel_class*)self_ptr;

    TitleController_class* aController = (TitleController_class*)getController(&self->super);
    aController->super.handleInput(aController);

    KeyType key = aController->super.keyState(&aController->super);

    if(key == MYKEY_NONE) return;

    switch(key) {
        case MYKEY_UP:
            self->private->nowScene = (self->private->nowScene + (SELECT_MAX-1)) % SELECT_MAX;
            break;
        case MYKEY_DOWN:
            self->private->nowScene = (self->private->nowScene + 1) % SELECT_MAX;
            break;
        case MYKEY_ENTER:
            handleEnterKey(self);
            break;
        case MYKEY_ESCAPE:
            self->super.setQuit(&self->super, true);
            break;
        default:
            return;
    }

    if(self->super.quit(&self->super)) return;

    TitleView_class* aView = (TitleView_class*)getView(&self->super);
    aView->super.draw(aView);
}

/**
 * 次のシーンのモデルのインスタンスを作成して返します.
 */
static Model_class* createNextSceneModel(void* self_ptr, SceneType next)
{
    TitleModel_class* self = (TitleModel_class*)self_ptr;

    switch(next) {
        case GAME_SCENE:
            return (Model_class*)newGameModel(getManager(&self->super));
        default: return NULL;
    }
}

/**
 * 破棄します.
 */
static void destroy(void* self_ptr) 
{
    TitleModel_class* self = (TitleModel_class*)self_ptr;
    destroyTitleModel(self);
}

/**
 * コンストラクタです。
 */
TitleModel_class* newTitleModel(Manager_class* aManager)
{
    TitleModel_class* self = malloc(sizeof(TitleModel_class));
    newModel(&self->super, aManager);
    self->private = malloc(sizeof(TitleModel));

    self->super.init = init;
    self->super.update = update;
    self->super.createNextSceneModel = createNextSceneModel;
    self->super.destroy = destroy;

    TitleView_class* aView = newTitleView(self);
    setView(&self->super, (View_class*)aView);

    TitleController_class* aController = newTitleController(self);
    setController(&self->super, (Controller_class*)aController);

    self->nowScene = &nowScene;

    self->private->nowScene = GAME;

    return self;
}

/**
 * デストラクタです。
 */
void destroyTitleModel(TitleModel_class* self)
{
    free(self->private);
    destroyModel(&self->super);
    free(self);
}

// 以下ゲッター, セッター

/**
 * nowScene ゲッター
 */
static SELECT_SCENE nowScene(TitleModel_class* self)
{
    return self->private->nowScene;
}
