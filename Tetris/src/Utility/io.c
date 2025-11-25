#include "io.h"
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#if defined(_WIN32) || defined(_WIN64)
    #include <conio.h>
    #include <windows.h>

    static bool initialized =false;

    void IO_init(void) { 
        initialized = true; 
        SetConsoleOutputCP(CP_UTF8);
    }
    void IO_term(void) { initialized = false;}
    bool IO_kbhit(void) {return _kbhit(); }
    int IO_getch() {return _getch(); }
    void IO_clear(void) { system("cls"); }
    int IO_putch(int ch) { return _putch(ch); }
    int IO_printf(const char* format, ...) {
        va_list ap;
        va_start(ap, format);
        int n = vprintf(format, ap);
        va_end(ap);
        return n;
    }

    KeyType IO_readKey(void) {
        if(!IO_kbhit()) return MYKEY_NONE;

        int c = IO_getch();
        /* windows の矢印キーは 224 → コード */
        if(c == 0 || c == 224) {
            int c2 = IO_getch();
            switch(c2) {
                case 72: return MYKEY_UP;
                case 80: return MYKEY_DOWN;
                case 75: return MYKEY_LEFT;
                case 77: return MYKEY_RIGHT;
            }
            return MYKEY_OTHER;
            
        }
        /* 通常キー */
        switch(c) {
            case ' ': return MYKEY_SPACE;
            case '\n':
            case '\r': return MYKEY_ENTER;
            case 27: return MYKEY_ESCAPE;
            case 'z': return MYKEY_Z;
            case 'x': return MYKEY_X;
            default: return MYKEY_OTHER;
        }
    }

    void IO_sleep(double time) {
        DWORD msec = (DWORD) (time / 1000.0);
        if(msec == 0) msec = 1;
        Sleep(msec);
    }

    void IO_forceExit(void) {
        if(initialized) IO_term();
        exit(EXIT_FAILURE);
    }
    static void winCtrHnadler(DWORD ctrlType) {
        (void)ctrlType;
        IO_forceExit();
    }

    void IO_setupSignalHandlers(void) {
        SetConsoleCtrlHandler((PHANDLER_ROUTINE)winCtrHandler, TRUE);
    }

#else
    #include <curses.h>
    #include <unistd.h>
    #include <string.h>
    #include <locale.h>

    static bool initialized = false;

    void IO_init(void) {
        if (initialized) return;
        setlocale(LC_ALL, "");
        initscr();
        cbreak();
        noecho();
        nodelay(stdscr, TRUE);
        keypad(stdscr, TRUE);
        initialized = true;
    }

    void IO_term(void) {
        if(initialized) endwin();
        initialized = false;
    }

    bool IO_kbhit(void) {
        int ch = getch();
        if (ch != ERR) {
            ungetch(ch);
            return true;
        }
        return false;
    }

    int IO_getch(void) {
        return getch();
    }

    void IO_clear(void) {
        clear();
        refresh();
    }

    int IO_putch(int ch) {
        int r = addch(ch);
        refresh();
        return (r == OK) ? ch : EOF;
    }

    int IO_printf(const char* format, ...) {
        va_list ap;
        va_start(ap, format);
        char buf[4096];
        vsnprintf(buf, sizeof(buf), format, ap);
        va_end(ap);
        int r = printw("%s", buf);
        refresh();
        return (r == OK) ? (int)strlen(buf) : EOF;
    }
    
    KeyType IO_readKey(void) {
        if(!IO_kbhit()) return MYKEY_NONE;
    
        int c = IO_getch();
    
        /* UNIX の矢印キーはncursesが全て吸収  */
        switch(c) {
            case KEY_UP: return MYKEY_UP;
            case KEY_DOWN: return MYKEY_DOWN;
            case KEY_LEFT: return MYKEY_LEFT;
            case KEY_RIGHT: return MYKEY_RIGHT;
            case 27: return MYKEY_ESCAPE;
            case ' ': return MYKEY_SPACE;
            case '\n':
            case '\r': return MYKEY_ENTER;
            case 'z': return MYKEY_Z;
            case 'x': return MYKEY_X;
            default: return MYKEY_OTHER;
        }
    }

    void IO_sleep(double time) {
        usleep(time*1000);
    }

    void IO_forceExit(void) {
        if(initialized) IO_term();
        exit(EXIT_FAILURE);
    }

    static void sigHandler(int sig) {
        (void)sig;
        IO_forceExit();
    }

    void IO_setupSignalHandlers(void) {
        signal(SIGINT, sigHandler);
        signal(SIGTERM, sigHandler);
    }
#endif


