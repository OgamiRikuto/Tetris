#include "Model.h"
#include "View.h"
#include "Controller.h"

#include <stdbool.h>

#define SCENE_MAX 3

struct _model {
    View_class* aView;
    Controller_class*  aController;
    Manager_class* aManager;
    SceneType nextScene;
    bool quit;
};

// ゲッター, セッタープロトタイプ宣言
static SceneType nextScene(Model_class*);
static void setNextScene(Model_class*, SceneType);
static bool quit(Model_class*);
static void setQuit(Model_class*, bool);

/**
 * 初期化を行います.
 * 純粋仮装環境より処理は書きません.
 */
static void init(void* self) { return;}

/**
 * 更新を行います.
 * 純粋仮装関数より処理は書きません.
 */
static void update(void* self) { return;}

/**
 * 次のシーンのモデルのインスタンスを作成して返します.
 * 純粋仮装関数より処理は書きません.
 */
static Model_class* createNextSceneModel(void* self, SceneType next) { return NULL;}

/**
 * 破棄を行います.
 * 純粋仮装関数より処理は書きません.
 */
static void destroy(void* self) { return;}

/**
 * コンストラクタです.
 */
void newModel(Model_class* self, Manager_class* aManeger)
{
    self->private = malloc(sizeof(Model));

    self->init = init;
    self->update = update;
    self->createNextSceneModel = createNextSceneModel;
    self->destroy = destroy;
    self->nextScene = nextScene;
    self->setNextScene = setNextScene;
    self->quit = quit;
    self->setQuit = setQuit;

    self->private->aManager = aManeger;
    self->private->aController = NULL;
    self->private->aView = NULL;
    self->private->nextScene = SCENE_NONE;
    self->private->quit = false;
}

/**
 * デストラクタです.
 */
void destroyModel(Model_class* self)
{
    destroyController(self->private->aController);
    destroyView(self->private->aView);
    free(self->private);
}

// 以下ゲッター, セッター

/**
 * aView ゲッター
 */
View_class* getView(Model_class* self)
{
    return self->private->aView;
}

/**
 * aView セッター
 */
void setView(Model_class* self, View_class* scene)
{
    self->private->aView = scene;
}

/**
 * aController ゲッター
 */
Controller_class* getController(Model_class* self)
{
    return self->private->aController;
}

/**
 * aController セッター
 */
void setController(Model_class* self, Controller_class* scene)
{
    self->private->aController = scene;
}

/**
 * aManager ゲッター
 */
Manager_class* getManager(Model_class* self)
{
    return self->private->aManager;
}

/**
 * nextScene ゲッター
 */
static SceneType nextScene(Model_class* self)
{
    return self->private->nextScene;
}

/**
 * nextScene セッター
 */
static void setNextScene(Model_class* self, SceneType type)
{
    self->private->nextScene = type;
}

/**
 * quit ゲッター
 */
static bool quit(Model_class* self)
{
    return self->private->quit;
}

/**
 * quit セッター
 */
static void setQuit(Model_class* self, bool boolean)
{
    self->private->quit = boolean;
}

