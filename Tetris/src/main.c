#include <time.h>
#include <stdlib.h>

#include "Manager.h"
#include "io.h"

int main(void)
{
    IO_init();
    atexit(IO_term);
    IO_setupSignalHandlers();

    srand(time(NULL));
    
    Manager_class* aManager = newManager();

    aManager->init(aManager);
    aManager->run(aManager);

    destroyManager(aManager);

    IO_term();
    
    return 0;
}
