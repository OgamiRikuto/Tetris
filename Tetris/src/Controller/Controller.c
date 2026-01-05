#include "Controller.h"

struct _controller {
    Model_class* aModel;
    KeyType keyState;
};

// ゲッター, セッタープロトタイプ宣言
static KeyType keyState(Controller_class*);
static void setKey(Controller_class*, KeyType);

/**
 * 入力を受け付けます.
 * 純粋仮装関数より処理は書きません.
 */
static void handleInput(void* self) { return;}

/**
 * コンストラクタです.
 */
void newController(Controller_class* self, Model_class* aModel)
{
    self->private = malloc(sizeof(Controller));

    self->handleInput = handleInput;
    self->keyState = keyState;
    self->setKey = setKey;

    self->private->aModel = NULL;
    self->private->keyState = MYKEY_NONE;
}

/**
 * デストラクタです.
 */
void destroyController(Controller_class* self)
{
    free(self->private);
}

// 以下ゲッター, セッター

/**
 * keyState ゲッター
 */
static KeyType keyState(Controller_class* self)
{
    return self->private->keyState;
}

/**
 * keyState セッター
 */
static void setKey(Controller_class* self, KeyType key)
{
    self->private->keyState = key;
}
