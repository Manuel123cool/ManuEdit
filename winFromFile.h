#pragma once
#include <ncurses.h>
#include <array>
#include <fstream>
#include <vector>
#include <iostream>

class WinFromFile
{
public:
    WINDOW* m_mainWin;
    char* m_fileName;
    int m_startLine;
    int m_endLine;
    std::vector<std::string> m_fileLines;
    enum Mode {view, replace, insert};
    Mode m_mode{Mode::view};
    int m_yBefore, m_xBefore;

    WinFromFile(char*);
    ~WinFromFile();
    void make_FileLines();
    void make_Window();
    void updateStatus(Mode);
    void updateForScrolling(int);
    void updateForCursor(int);
    void update();
    void updateForResize();
    void saveToFile();
    void updateForSave(int);
    void renewLine(int);
    void rezisePosBefore();
};
