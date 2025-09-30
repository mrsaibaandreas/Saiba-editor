#include <iostream>
#include <cstdint>
#include <stdlib.h>
#include <unistd.h>
#include <sstream>
#include "Editor.hpp"
#include "Logger.hpp"
#include "Misc.hpp"

// Define's space
#define TRANSFORM_CTRL(k) ((k) & 0x1f)
// End of define's space



Editor::Editor() {
    if (!init()) {
        LOG_D("Init was not succesful, aborting...");
        std::exit(0);
    }
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
            std::string  welcomeMessage = "MrSaiba learning text editor -- version 1.0.0"; 
            std::uint32_t screenCollums = screen.getCols();
            if (welcomeMessage.size() > screenCollums) {
                welcomeMessage.resize(screenCollums);
            } 
            std::int32_t padding = (screenCollums - welcomeMessage.size()) / 2;
            if (padding) {
                writeToScreen("#");
                padding--;
            }
            while (padding--) {
                writeToScreen(" ");
            }
            writeToScreen(welcomeMessage);
        } else {
            writeToScreen("#");    
        }
        writeToScreen("\x1b[K");
        if (row < maxRows - 1) { 
            writeToScreen("\r\n");
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

    std::stringstream cursorPosition;
    cursorPosition<< "\x1b[" << screen.getCy() + 1 << ";" <<  screen.getCx() + 1 << "H";
    writeToScreen(cursorPosition.str());

    writeToScreen("\x1b[?25h");

    screen.flush();
}

void Editor::editorProcessKey() {
    int userInput = screen.readKeyboardInput();
    
    switch (userInput) {
        case TRANSFORM_CTRL('q'):
            LOG_D("Gracefully exiting the program, user request");
            editorRefreshScreen();
            disableRawMode();
            // in the future find a greater approach than just hardcoding here
            writeToScreen("\x1b[H\x1b[2J");
            screen.flush();
            std::exit(0);
            break;
        case ARROW_UP:
        case ARROW_DOWN:
        case ARROW_LEFT:
        case ARROW_RIGHT:
            editorMoveCursor(userInput);
            break;
        default:
            LOG_D("Key %c pressed\r", userInput);

    }
}

void Editor::editorMoveCursor(int key) {
    std::uint32_t cx = screen.getCx();
    std::uint32_t cy = screen.getCy();
    switch (key) {
        case 'w':
            screen.setCy(--cy);
            break;
        case 'a':
            screen.setCx(--cx);
            break;
        case 's':
            screen.setCy(++cy);
            break;
        case 'd':
            screen.setCx(++cx);
            break;
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

