#include "Screen.hpp"
#include <unistd.h>

Screen::Screen() {
    write(0, "I work\n", 7);
    init();
}

void Screen::init() {
    cx = 0;
    cy = 0;
    rowoff = 0;
    coloff = 0;
    numrows = 0;
    dirty = 0;
}

char Screen::readKeyboardInput() {
    return 'a';    
}
