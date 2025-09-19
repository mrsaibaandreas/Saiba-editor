#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include "Editor.hpp"
#include "Logger.hpp"

// Define's space
#define TRANSFORM_CTRL(k) ((k) & 0x1f)
// End of define's space

Screen Editor::screen;

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

    atexit(disableRawMode);
    screen.enableRawMode();
}

void Editor::disableRawMode() {
    screen.disableRawMode();
}

void Editor::drawRows() {
    for (int raw = 0; raw < 24; raw++)
        writeToScreen("#\r\n");
}
void Editor::writeToScreen(const std::string& sequence) {
    screen.draw(sequence);
}
void Editor::editorRefreshScreen() {
    screen.refreshScreen();
    drawRows();
    writeToScreen("\x1b[H");
}
void Editor::editorProcessKey() {
    char userInput = screen.readKeyboardInput();
    
    switch (userInput) {
        case TRANSFORM_CTRL('q'):
            LOG_D("Gracefully exiting the program, user request");
            editorRefreshScreen();
            std::exit(0);
            break;
        default:
            LOG_D("Key %c pressed\r", userInput);

    }
}
void Editor::startMainLoop() {
    enableRawMode();
    while (1) {
        editorRefreshScreen();
        editorProcessKey();
    }
    disableRawMode();  
}
