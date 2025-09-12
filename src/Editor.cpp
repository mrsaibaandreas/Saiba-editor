#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include "Editor.hpp"
#include "Logger.hpp"

struct termios Editor::origTermios;

Editor::Editor() {
    if (!init()) {
        LOG_D("Init was not succesful, aborting...\n");
        std::exit(0);
    }
    LOG_D("Init was succesful\n");
}

Editor::~Editor() {
    if (!uninit()) {
        LOG_D("Uninit not succesful, maybe some corruption, maybe some files were not properly closed\n");
        std::exit(0);
    }
    LOG_D("Uninit was succesful\n");
}

bool Editor::init() {
    //updateWindowSize();
    //signal(SIGWINCH, handleSigWinCh);
    return true;
}

bool Editor::uninit() {
    return true;
}

void Editor::enableRawMode() {
    tcgetattr(STDIN_FILENO, &origTermios);
    atexit(disableRawMode);
    struct termios raw = origTermios;
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void Editor::disableRawMode() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &origTermios) == -1) {
        LOG_I(LogLevel::ERROR, "tcsetattr");
        exit(-1);
    }
}

void Editor::editorProcessKey() {
 //   readKeyboardInput();
}
void Editor::startMainLoop() {
    enableRawMode();
    char c = '\0';
    while (1) {
  //      editorRefreshScreen();
    //    editorProcessKey();
        if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN) std::abort();
        if (iscntrl(c)){
            LOG_D("%d\r\n", c);
        } else {
            LOG_D("%d ('%c')\r\n", c, c); 
        }
        if (c == 'q') break;
    }
    disableRawMode();
}
