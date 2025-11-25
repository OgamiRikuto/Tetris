#ifndef MODEL_PRIVATE_H
#define MODEL_PRIVATE_H

typedef struct model_class Model_class;
typedef struct view_class View_class;
typedef struct controller_class Controller_class;

Manager_class* getManager(Model_class*);
View_class* getView(Model_class*);
void setView(Model_class*, View_class*);
Controller_class* getController(Model_class*);
void setController(Model_class*, Controller_class*);

#endif
