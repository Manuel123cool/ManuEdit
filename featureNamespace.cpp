#include "featureNamespace.h"
#include "winFromFile.h"
#include <iostream>
#include <string>
#include "randomStuff.h"
#include <sstream>

void FeatureNamespace::updateForReplace(int ch, WinFromFile &winFromFile)
{
    if (ch == 114)
    {
        winFromFile.updateStatus(WinFromFile::Mode::replace);

        halfdelay(40);

        ch = getch();
        nocbreak();
        cbreak();
        keypad(winFromFile.m_mainWin, TRUE);

        winFromFile.updateStatus(WinFromFile::Mode::view);

        if (ch == ERR) return;
        if (ch == 27) return; //when esc is pressed
        if (ch == KEY_F(3)) return; //if f3 is pressed cancel

        waddch(winFromFile.m_mainWin, ch);
        wrefresh(winFromFile.m_mainWin);

        int y, x;
        getyx(winFromFile.m_mainWin, y, x);
        
        winFromFile.renewLine(y);
    }
};

void FeatureNamespace::updateForDelete(int ch, WinFromFile &winFromFile)
{
    if (ch == 120)
    {
        int y, x;
        getyx(winFromFile.m_mainWin, y, x);

        int count{ 0 };
        int index{winFromFile.m_startLine + y};
        std::string fileLineStringBefore{ "" };
        std::string fileLineString{ "" };

        if (y >= winFromFile.m_fileLines.size() || x >= winFromFile.m_fileLines.at(index).length())
            return;

        for(char& c : winFromFile.m_fileLines.at(index))
        {
            if (count > x)
                    fileLineString += c;
            else if (count != x)
                fileLineStringBefore += c;
            count++;
        }

        wprintw(winFromFile.m_mainWin, fileLineString.c_str());
        mvwaddch(winFromFile.m_mainWin, y, winFromFile.m_fileLines.at(index).length() - 1, ' ');
        wmove(winFromFile.m_mainWin, y, x);
        wrefresh(winFromFile.m_mainWin);

        winFromFile.m_fileLines.at(index) = fileLineStringBefore + fileLineString; 
    }
};

void FeatureNamespace::returnStatusBar(WinFromFile &winFromFile, char* statusString)
{
    //Mode m_mode{ view };
    const char modeStringView[] = "-- View --   ";
    const char modeStringRep[] = "-- Replace --";
    const char modeStringInsert[] = "-- Insert -- ";

    int count{ 0 };

    switch( winFromFile.m_mode )
    {
        case WinFromFile::Mode::view:
            count = 0;
            for (auto &ch : modeStringView)
            {
                statusString[count] = ch;
                count++;
            }
            break;

        case WinFromFile::Mode::replace:
            count = 0;
            for (auto &ch : modeStringRep)
            {
                statusString[count] = ch;
                count++;
            }
            break;
        case WinFromFile::Mode::insert:
            count = 0;
            for (auto &ch : modeStringInsert)
            {
                statusString[count] = ch;
                count++;
            }
            break;
    }
};

void FeatureNamespace::updateForInsert(int ch, WinFromFile &winFromFile)
{
    if (ch == 'i')
    {
        winFromFile.updateStatus(WinFromFile::Mode::insert);

        int y, x;
        getyx(winFromFile.m_mainWin, y, x);

        std::string addedChars{ "" };
        while((ch = getch()) != KEY_F(3) && ch != 27)
        {
            waddch(winFromFile.m_mainWin, ch);
            wrefresh(winFromFile.m_mainWin);

            addedChars += ch;
        }
        winFromFile.updateStatus(WinFromFile::Mode::view);

        winFromFile.renewLine(y);
    }
};
