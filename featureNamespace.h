#pragma once
#include <ncurses.h>
#include <array>
#include <fstream>
#include <vector>

class WinFromFile;
namespace FeatureNamespace
{
    void updateForReplace(int ch, WinFromFile &winFromFile);
    void returnStatusBar(WinFromFile &winFromFile, char* statusString);
    void updateForDelete(int ch, WinFromFile &winFromFile);
    void updateForInsert(int ch, WinFromFile &winFromFile);

}
