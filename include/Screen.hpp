#pragma once

#include <string>
#include <termios.h>

class Screen {
    public:
        Screen();
        
        void refreshScreen();
        void enableRawMode();
        void disableRawMode();
        void abortP(const std::string& message);
        void draw(const std::string& sequence);

        char readKeyboardInput(); 
    private:
        struct termios origTermios;
        void init(void);
        void uninit(void);

        int cx;
        int cy;
        int rowoff;
        int coloff;
        int numrows;
        int screenrows;
        int screencols;
        bool rawmode;
        int dirty;

};
