#ifndef IO_H
#define IO_H

#include <stdarg.h>
#include <stdbool.h>

/* キー統一 */
typedef enum {
    MYKEY_NONE = 0,
    MYKEY_UP,
    MYKEY_DOWN,
    MYKEY_LEFT,
    MYKEY_RIGHT,
    MYKEY_SPACE,
    MYKEY_ENTER,
    MYKEY_ESCAPE,
    MYKEY_Z,
    MYKEY_X,
    MYKEY_OTHER
} KeyType;

void IO_init(void);
void IO_term(void);
bool IO_kbhit(void);
int IO_getch(void);
void IO_clear(void);
int IO_putch(int ch);
int IO_printf(const char* format, ...);
KeyType IO_readKey(void);
void IO_sleep(double);

void IO_forceExit(void);
void IO_setupSignalHandlers(void);

#endif
