#pragma once

class Screen {
    public:
        Screen();

        char readKeyboardInput(); 
    private:
        void init(void);

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
