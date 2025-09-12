#pragma once
#include "Screen.hpp"
class Editor {
    public:
        Editor();
        ~Editor();
        void startMainLoop(void);
    private:
        bool init();
        bool uninit();
        Screen screen;

        void editorProcessKey();
};
