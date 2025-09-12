#include <iostream>
#include <unistd.h>
#include "Editor.hpp"

Editor::Editor() {
    if (!init()) {
        std::cout << "Init was not succesful, aborting...\n";
        std::exit(0);
    }
    std::cout << "Init was succesful\n";
}

Editor::~Editor() {
    if (!uninit()) {
        std::cout << "Uninit not succesful, maybe some corruption, maybe some files were not properly closed\n";
        std::exit(0);
    }
    std::cout << "Uninit was succesful\n";
}

bool Editor::init() {
    //updateWindowSize();
    //signal(SIGWINCH, handleSigWinCh);
    return true;
}

bool Editor::uninit() {
    return true;
}



void Editor::editorProcessKey() {
 //   readKeyboardInput();
}
void Editor::startMainLoop() {
    while (1) {
  //      editorRefreshScreen();
    //    editorProcessKey();
          sleep(1);
          write(1, "Test\n", 5);
    }    

}
