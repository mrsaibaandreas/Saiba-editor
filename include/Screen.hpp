#pragma once

#include <string>
#include <termios.h>
#include <cstdint>

class Screen {
    public:
        Screen();
        
        void refreshScreen();
        void enableRawMode();
        void disableRawMode();
        void flush(); 
        void abortP(const std::string& message);
        void draw(const std::string& sequence);
     
        bool getWindowSize();
        bool getCursorPosition();

        int readKeyboardInput(); 
        
        // Getters and setters area
        std::int32_t getRows();
        std::int32_t getCols();
        std::int32_t getCx();
        std::int32_t getCy();

        void setCx(std::int32_t cx);
        void setCy(std::int32_t cy);
        
    private:
        struct termios origTermios;
        void init(void);
        void uninit(void);
       
        std::string writeBuffer;

        std::int32_t cx;
        std::int32_t cy;
        std::int32_t rowoff;
        std::int32_t coloff;
        std::int32_t numrows;
        std::int32_t screenrows;
        std::int32_t screencols;
        std::int32_t dirty;

        bool rawmode;

};
