#include "config.h"
#include "Manager.h"
#include "Model.h"
#include "View.h"
#include "io.h"

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

struct _manager {
    Model_class* aModel;
    SceneType currentScene;
    double deltaTime;
};

// ゲッター, セッタープロトタイプ宣言
static double deltaTime(Manager_class*);
static void setDeltaTime(Manager_class*, clock_t);

/**
 * 初期化します。
 */
static void init(Manager_class* self)
{
    self->private->aModel->init(self->private->aModel);
}

/**
 * メインループです。
 */
static void run(Manager_class* self)
{
    double lastTime = (double)clock() / CLOCKS_PER_SEC;
    while(1) {
        double now = (double)clock() / CLOCKS_PER_SEC;
        self->private->deltaTime = now - lastTime;
        lastTime = now;

        if(self->private->aModel->quit(self->private->aModel)) break;

        self->private->aModel->update(self->private->aModel);

        SceneType nextScene = self->private->aModel->nextScene(self->private->aModel);
        if(nextScene != SCENE_NONE) self->changeScene(self, nextScene);
    }   
}

/**
 * シーンを変更します。
 */
static void changeScene(Manager_class* self, SceneType next)
{   
    Model_class* tmp = self->private->aModel->createNextSceneModel(self->private->aModel, next);
    if(tmp == NULL) {
        fprintf(stderr, "Scene Error: cannot prepare next scene.\n");
        exit(EXIT_FAILURE);
    }
    self->private->aModel->destroy(self->private->aModel);
    self->private->aModel = tmp;
    self->private->currentScene = next;
    self->private->aModel->init(self->private->aModel);
}

/**
 * コンストラクタです。
 */
Manager_class* newManager(void)
{
    Manager_class* self = malloc(sizeof(Manager_class));
    self->private = malloc(sizeof(Manager));

    self->init = init;
    self->run = run;
    self->changeScene = changeScene;

    self->deltaTime = deltaTime;
    self->setDeltaTime = setDeltaTime;

    self->private->currentScene = TITLE_SCENE;
    self->private->deltaTime = 0.0;

    self->private->aModel = (Model_class*)newTitleModel(self);

    return self;
}

/**
 * デストラクタです。
 */
void destroyManager(Manager_class* self)
{
    self->private->aModel->destroy(self->private->aModel);
    free(self->private);
    free(self);
}

// 以下ゲッター, セッター

/**
 * deltaTime ゲッター
 */
static double deltaTime(Manager_class* self)
{
    return self->private->deltaTime;
}

static void setDeltaTime(Manager_class* self, clock_t time)
{
    self->private->deltaTime = time;
}
