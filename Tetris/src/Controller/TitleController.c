#include "Model.h"
#include "Controller.h"
#include "io.h"

/**
 * 不必要なキー入力を全て OTHER にします.
 */
static KeyType keyFilter(KeyType key)
{
    switch(key) {
        case MYKEY_UP:
        case MYKEY_DOWN:
        case MYKEY_NONE:
        case MYKEY_ENTER:
        case MYKEY_ESCAPE:
            return key;
        default: 
            return MYKEY_OTHER;
    }
}

/**
 * 入力を受け付けます。
 */
static void handleInput(void* self_ptr)
{
    TitleController_class* self = (TitleController_class*)self_ptr;
    KeyType key = keyFilter(IO_readKey());
    self->super.setKey(&self->super, key);
}

/**
 * コンストラクタです。
 */
TitleController_class* newTitleController(TitleModel_class* aModel)
{
    TitleController_class* self = malloc(sizeof(TitleController_class));
    newController(&self->super, &aModel->super);

    self->super.handleInput = handleInput;

    return self;
}

/**
 * デストラクタです。
 */
void destroyTitleController(TitleController_class* self)
{
    destroyController(&self->super);
    free(self);
}
