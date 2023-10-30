#pragma once
#include <ncurses.h>
#include <array>
#include <fstream>
#include <vector>

class WinFromFile;
namespace Movement
{
    void startOfLine(int, WinFromFile &winFromFile);
    void endOfLine(int ch, WinFromFile &winFromFile);
    void beginingOfWord(int ch, WinFromFile &winFromFile);
    void endOfWord(int ch, WinFromFile &winFromFile);
}
