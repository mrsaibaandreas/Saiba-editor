#include <iostream>
#include <memory>
#include "Editor.hpp"

void startLoop(std::shared_ptr<Editor> editor) {
    editor->startMainLoop();
}
int main() {
    // auto init by constructor
    std::shared_ptr<Editor> editor = std::make_shared<Editor>();
    
    startLoop(editor);

    return 0;
}

