#include "Screen.hpp"
#include "Logger.hpp"

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
    cx = 0;
    cy = 0;
    rowoff = 0;
    coloff = 0;
    numrows = 0;
    dirty = 0;

}
// abort process
void Screen::abortP(const std::string& message) {
    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H", 3);
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
   writeBuffer += "\x1b[H";
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

char Screen::readKeyboardInput() {
    std::int32_t charactersRead;
    char character;
    while ((charactersRead = read(STDIN_FILENO, &character, 1)) != 1) {
        if (charactersRead == - 1 && errno != EAGAIN) std::abort();
    }
    return character;
}

std::uint32_t Screen::getRows() {
    return screenrows;
}

std::uint32_t Screen::getCols() {
    return screencols;
}

void Screen::flush() {
    write(STDOUT_FILENO, writeBuffer.c_str(), writeBuffer.size());
    writeBuffer.clear();
    writeBuffer = std::string();
}


