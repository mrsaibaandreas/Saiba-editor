#include "Screen.hpp"
#include "Logger.hpp"
#include "Misc.hpp"
#include <sys/ioctl.h>
#include <unistd.h>


/* Note
 * For operations which don't require an result, std::cout is used
 * Otherwise write will be used
 * TO-DO: Find a more C++ way to do this, if available, else maybe move to write()
 */

Screen::Screen() {
    init();
    writeBuffer = "";
    LOG_D("Screen object initialized");
}

void Screen::init() {
    cx = 15;
    cy = 15;
    rowoff = 0;
    coloff = 0;
    numrows = 0;
    dirty = 0;
}

// abort process
void Screen::abortP(const std::string& message) {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"
    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H", 3);
#pragma GCC diagnostic pop
    LOG_I(LogLevel::ERROR, message.c_str());
    std::abort();
}

void Screen::draw(const std::string& sequence) {
    writeBuffer += sequence;
}
void Screen::enableRawMode() {
    if (tcgetattr(STDIN_FILENO, &origTermios)) abortP("tgetattr");
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
   writeBuffer += "\x1b]H";
}

void Screen::uninit() {}

bool Screen::getWindowSize() {
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
        if (write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12) return false;
        LOG_I(LogLevel::ERROR, "Failed to get window size");
        return getCursorPosition();
    } else {
        screenrows = ws.ws_row;
        screencols = ws.ws_col;
        LOG_D("Row: %d, Col: %d", screenrows, screencols);
        return true;
    }
}

bool Screen::getCursorPosition() {
    char cursorPositionBuffer[32];
    std::uint32_t index = 0;

    if (write(STDOUT_FILENO, "\x1b[6n", 4) != 4) abortP("Cannot get cursor position");

    while (index < sizeof(cursorPositionBuffer) -1) {
        if (read(STDIN_FILENO, &cursorPositionBuffer[index], 1) != 1) break;
        if (cursorPositionBuffer[index] == 'R') break;
        index++;
    }
    cursorPositionBuffer[index] = '\0';

    if (cursorPositionBuffer[0] != '\x1b' || cursorPositionBuffer[1] != '[') return false;
    if (sscanf(&cursorPositionBuffer[2], "%d;%d", &screenrows, &screencols) != 2) return false;

    return true;
}

int Screen::readKeyboardInput() {
    std::int32_t charactersRead;
    char character;
    while ((charactersRead = read(STDIN_FILENO, &character, 1)) != 1) {
        if (charactersRead == - 1 && errno != EAGAIN) std::abort();
    }
    // map arrow keys
    if (character == '\x1b') {
        char sequence[3];
        if (read(STDIN_FILENO, &sequence[0], 1) != 1) return '\x1b';
        if (read(STDIN_FILENO, &sequence[1], 1) != 1) return '\x1b';
        if (sequence[0] == '[') {
            if (sequence[1] >= '0' && sequence[1] <= '9') {
                if (read(STDIN_FILENO, &sequence[2], 1) != 1) return '\x1b';
                if (sequence[2] == '~') {
                    switch (sequence[1]) {
                        case '1': return HOME;
                        case '3': return DELETE;
                        case '4': return END;
                        case '5': return PAGE_UP;
                        case '6': return PAGE_DOWN;
                        case '7': return HOME;
                        case '8': return END;
                    }
                }
            } else {
                switch (sequence[1]) {
                    case 'A': return ARROW_UP;
                    case 'B': return ARROW_DOWN;
                    case 'C': return ARROW_RIGHT;
                    case 'D': return ARROW_LEFT;
                    case 'H': return HOME;
                    case 'F': return END;

                }
            }
        } else if (sequence[0] == 'O') {
            switch (sequence[1]) {
                case 'H': return HOME;
                case 'F': return END;
            }
        }
        return '\x1b';
    } else {
        return character;
    }
}

std::int32_t Screen::getRows() {
    LOG_D("Row value %d", screenrows);
    return screenrows;
}

std::int32_t Screen::getCols() {
    LOG_D("Collumn value %d", screencols);
    return screencols;
}

std::int32_t Screen::getCx() {
    LOG_D("Cx value %d", cx);
    return cx;
}
std::int32_t Screen::getCy() {
    LOG_D("Cy value %d", cy);
    return cy;
}

void Screen::setCx(std::int32_t cx) {
        if (cx == screencols) {
        LOG_I(LogLevel::WARNING, "Screen overflow avoided, not setting the value");
        return ;
    } else if (cx < 0) {
        LOG_I(LogLevel::WARNING, "Screen underflow avoided, not setting the value");
        return ;
    }

    this->cx = cx;
}

void Screen::setCy(std::int32_t cy) {
    if (cy == screenrows) {
        LOG_I(LogLevel::WARNING, "Screen overflow avoided, not setting the value");
        return ;
    } else if (cy < 0) {
        LOG_I(LogLevel::WARNING, "Screen underflow avoided, not setting the value");
        return ;
    }
    this->cy = cy;
}

void Screen::flush() {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"
    write(STDOUT_FILENO, writeBuffer.c_str(), writeBuffer.size());
#pragma GCC diagnostic pop
    writeBuffer.clear();
    writeBuffer = std::string();
}


