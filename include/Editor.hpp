#pragma once
#include "Screen.hpp"
#include <termios.h>
class Editor {
    public:
        Editor();
        ~Editor();
        void startMainLoop(void);
    private:
        Screen screen;
        static struct termios origTermios;
        
        bool init();
        bool uninit();
        void editorProcessKey();
        static void enableRawMode();
        static void disableRawMode();
};
