#include "Screen.hpp"
#include "Logger.hpp"
#include <unistd.h>

Screen::Screen() {
    init();
    LOG_D("Screen object initialized");
}

void Screen::init() {
    cx = 0;
    cy = 0;
    rowoff = 0;
    coloff = 0;
    numrows = 0;
    dirty = 0;
}
// abort process
void Screen::abortP(const std::string& message) {
    LOG_I(LogLevel::ERROR, message.c_str());
    std::abort();
}

void Screen::draw(const std::string& sequence) {
    std::cout << sequence;
}
void Screen::enableRawMode() {
    tcgetattr(STDIN_FILENO, &origTermios);
    struct termios raw = origTermios;
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);

}

void Screen::disableRawMode() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &origTermios) == -1) {
        abortP("tcsetattr");
    }
}

void Screen::refreshScreen() {
   std::cout << "\x1b[2J\x1b[H" << std::flush;
}

char Screen::readKeyboardInput() {
    int charactersRead;
    char character;
    while ((charactersRead = read(STDIN_FILENO, &character, 1)) != 1) {
        if (charactersRead == - 1 && errno != EAGAIN) std::abort();
    }
    return character;
}

void Screen::uninit() {}
