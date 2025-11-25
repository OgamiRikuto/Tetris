#include "View.h"
#include "View_private.h"
#include "Model.h"
#include "Title.h"
#include "io.h"

/**
 * 描画します
 */
static void draw(void* self_ptr)
{
    TitleView_class* self = (TitleView_class*)self_ptr;
    TitleModel_class* aModel = (TitleModel_class*)getModel(&self->super);
    SELECT_SCENE now = aModel->nowScene(aModel);
    IO_clear();

    const char* labels[SELECT_MAX] = {"Game Start", "Exit"};

    IO_printf("⬛︎⬛︎⬛︎⬛︎ Tetris ⬜︎⬜︎⬜︎⬜︎\n\n");
    for(int i=0; i<SELECT_MAX; i++) {
        if(i == now) IO_printf("%1s > %s\n\n", " ", labels[i]);
        else IO_printf("%1s  %s\n\n", " ", labels[i]);
    }

    /*
    IO_printf("⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎　　　　　　　　　　⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎\n");
    IO_printf("⬛︎　⬜︎　　　⬛︎　　　　　　　　　　⬛︎　　⬜︎　　　⬛︎　　⬜︎⬜︎　　⬛︎\n");
    IO_printf("⬛︎　⬜︎　　　⬛︎　　　　　　　　　　⬛︎　⬜︎⬜︎⬜︎　　⬛︎　　⬜︎⬜︎　　⬛︎\n");
    IO_printf("⬛︎　⬜︎　　　⬛︎　　　　　　　　　　⬛︎　　　　　　⬛︎　　　　　　⬛︎\n");
    IO_printf("⬛︎　⬜︎　　　⬛︎　　　　　　　　　　⬛︎　　　　　　⬛︎　　　　　　⬛︎\n");
    IO_printf("⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎　　　　　　　　　　⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎\n");
    IO_printf("⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎　　　　　　　　　　⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎　⬜︎⬜︎　　　⬛︎\n");
    IO_printf("⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎　　　　　　　　　　⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎　　⬜︎⬜︎　　⬛︎\n");
    IO_printf("⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎　　　　　　　　　　⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎　　　　　　⬛︎\n");
    IO_printf("⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎　　　　　　　　　　⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎　　　　　　⬛︎\n");
    IO_printf("⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎　　　　　　　　　　⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎\n");
    IO_printf("⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎　　　　　　　　　　⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎　⬜︎　　　　⬛︎\n");
    IO_printf("⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎　　　　　　　　　　⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎　⬜︎　　　　⬛︎\n");
    IO_printf("⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎　　　　　　　　　　⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎　⬜︎⬜︎　　　⬛︎\n");
    IO_printf("⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎　　　　　　　　　　⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎　　　　　　⬛︎\n");
    IO_printf("⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎　　　　　　　　　　⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎\n");
    IO_printf("⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎　　　　　　　　　　⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎　　⬜︎　　　⬛︎\n");
    IO_printf("⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎　　　　　　　　　　⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎　　⬜︎　　　⬛︎\n");
    IO_printf("⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎　　　　　　　　　　⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎　⬜︎⬜︎　　　⬛︎\n");
    IO_printf("⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎　　　　　　　　　　⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎　　　　　　⬛︎\n");
    IO_printf("⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎⬛︎\n");
    */
}

/**
 * コンストラクタです。
 */
TitleView_class* newTitleView(TitleModel_class* aModel)
{
    TitleView_class* self = malloc(sizeof(TitleView_class));
    newView(&self->super, &aModel->super);

    self->super.draw = draw;

    return self;
}

/**
 * デストラクタです。
 */
void destroyTitleView(TitleView_class* self)
{
    destroyView(&self->super);
    free(self);
}
