#include "Model.h"
#include "Controller.h"

/**
 * 入力を受け付けます。
 */
static void handleInput(void* self_ptr)
{
    GameController_class* self = (GameController_class*)self_ptr;
    KeyType key = IO_readKey();
    self->super.setKey(&self->super, key);
}

/**
 * コンストラクタです。
 */
GameController_class* newGameController(GameModel_class* aModel)
{
    GameController_class* self = malloc(sizeof(GameController_class));
    newController(&self->super, &aModel->super);

    self->super.handleInput = handleInput;

    return self;
}

/**
 * デストラクタです。
 */
void destroyGameController(GameController_class* self)
{
    free(self);
}
