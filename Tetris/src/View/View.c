#include "View.h"
#include "Model.h"

struct _view {
    Model_class* aModel;
};

/**
 * 描画します。
 * 純粋仮装関数より処理は書きません.
 */
static void draw(void* self){ return;}

/**
 * コンストラクタです。
 */
void newView(View_class* self, Model_class* aModel)
{
    self->private = malloc(sizeof(View));

    self->draw = draw;

    self->private->aModel = aModel;
}

/**
 * デストラクタです。
 */
void destroyView(View_class* self)
{
    free(self->private);
}

// 以下ゲッター, セッター
Model_class* getModel(View_class* self) 
{
    return self->private->aModel;
}
