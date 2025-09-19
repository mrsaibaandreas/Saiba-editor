#pragma once
#include "Screen.hpp"
enum class EditorStates {
    RAW_DISABLED = 0,
    RAW_ENABLED = 1
};
class Editor {
    public:
        Editor();
        ~Editor();
        void startMainLoop(void);
    private:
        static Screen screen;
        EditorStates editorState;              
        
        bool init();
        bool uninit();
        
        void editorProcessKey();
        void editorRefreshScreen();
        void drawRows();
        void writeToScreen(const std::string& sequence);        
        char readKeyboardInput(); 
        static void enableRawMode();
        static void disableRawMode();
};
