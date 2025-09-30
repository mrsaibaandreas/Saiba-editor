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
        std::uint32_t getRows();
        std::uint32_t getCols();
        std::uint32_t getCx();
        std::uint32_t getCy();

        void setCx(std::uint32_t cx);
        void setCy(std::uint32_t cy);
        
    private:
        struct termios origTermios;
        void init(void);
        void uninit(void);
       
        std::string writeBuffer;

        std::uint32_t cx;
        std::uint32_t cy;
        std::uint32_t rowoff;
        std::uint32_t coloff;
        std::uint32_t numrows;
        std::uint32_t screenrows;
        std::uint32_t screencols;
        std::uint32_t dirty;

        bool rawmode;

};
