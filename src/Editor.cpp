#include <iostream>
#include <cstdint>
#include <stdlib.h>
#include <unistd.h>
#include "Editor.hpp"
#include "Logger.hpp"

// Define's space
#define TRANSFORM_CTRL(k) ((k) & 0x1f)
// End of define's space


Editor::Editor() {
    if (!init()) {
        LOG_D("Init was not succesful, aborting...");
        std::exit(0);
    }
    screen = Screen();
    LOG_D("Init was succesful");
}

Editor::~Editor() {
    if (!uninit()) {
        LOG_D("Uninit not succesful, maybe some corruption, maybe some files were not properly closed");
        std::exit(0);
    }
    LOG_D("Uninit was succesful");
}

bool Editor::init() {
    if (!screen.getWindowSize())
        return false;
    //updateWindowSize();
    //signal(SIGWINCH, handleSigWinCh);
    return true;
}

bool Editor::uninit() {
    return true;
}

void Editor::enableRawMode() {

    //atexit(disableRawMode);
    screen.enableRawMode();
}

void Editor::disableRawMode() {
    screen.disableRawMode();
}

void Editor::drawRows() {
    int row;
    int maxRows = screen.getRows();
    for (row = 0; row < maxRows; row++) {
        if (row == maxRows / 3) {
            std::string  welcomeMessage = "MrSaiba learning text editor -- version 0.0.0"; 
            std::uint32_t screenCollums = screen.getCols();
            if (welcomeMessage.size() > screenCollums) {
                welcomeMessage.resize(screenCollums);
            } 
            writeToScreen(welcomeMessage);
        } else {
            writeToScreen("#\r\n");
        }
        writeToScreen("\x1b[K");
        if (row < maxRows - 1) { 
            writeToScreen("#\r\n");
        }
    }
}

void Editor::writeToScreen(const std::string& sequence) {
    screen.draw(sequence);
}

void Editor::editorRefreshScreen() {
    writeToScreen("\x1b[?25l");
    screen.refreshScreen();
    drawRows();
    writeToScreen("\x1b[H");
    writeToScreen("\x1b[?25h");
    screen.flush();
}

void Editor::editorProcessKey() {
    char userInput = screen.readKeyboardInput();
    
    switch (userInput) {
        case TRANSFORM_CTRL('q'):
            LOG_D("Gracefully exiting the program, user request");
            editorRefreshScreen();
            disableRawMode();
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
