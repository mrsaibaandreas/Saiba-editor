#include <iostream>
#include <memory>
#include "Editor.hpp"
#include "Logger.hpp"
void startLoop(std::shared_ptr<Editor> editor) {
    editor->startMainLoop();
}
int main() {
    // auto init by constructor
    LOG_I(LogLevel::INFO, "Starting main file");
    std::shared_ptr<Editor> editor = std::make_shared<Editor>();
    
    startLoop(editor);
dasdsadhbas
    return 0;
}

