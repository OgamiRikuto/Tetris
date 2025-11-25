#ifndef MANAGER_H
#define MANAGER_H

#include "Model.h"
#include <time.h>

typedef struct _manager Manager;
typedef struct manager_class Manager_class;

struct manager_class {
    Manager* private;

    /* ----- */
    void (*init)(Manager_class*);
    void (*run)(Manager_class*);
    void (*changeScene)(Manager_class*, SceneType);

    double (*deltaTime)(Manager_class*);
    void (*setDeltaTime)(Manager_class*, clock_t);
};

Manager_class* newManager(void);
void destroyManager(Manager_class*);

#endif
