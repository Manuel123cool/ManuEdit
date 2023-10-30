#include "movement.h"
#include <iostream>
#include <string>
#include "winFromFile.h"

void Movement::startOfLine(int ch, WinFromFile &winFromFile)
{
    if (ch == '0')
    {
        int y, x;
        getyx(winFromFile.m_mainWin, y, x);

        wmove(winFromFile.m_mainWin, y, 0);
        wrefresh(winFromFile.m_mainWin);
    }
};

void Movement::endOfLine(int ch, WinFromFile &winFromFile)
{
    if (ch == '$')
    {
        int y, x;
        getyx(winFromFile.m_mainWin, y, x);

        int count{ 0 };
        int lastNotEmpty{ 0 };

        for(char& c : winFromFile.m_fileLines.at(y))
        {
            if (count < COLS - 1 && c != ' ')
                lastNotEmpty = count;
            
            count++;
        }
        
        wmove(winFromFile.m_mainWin, y, lastNotEmpty + 1);
        wrefresh(winFromFile.m_mainWin);
    }
};

void Movement::beginingOfWord(int ch, WinFromFile &winFromFile)
{
    if (ch == 'b')
    {
        int y, x;
        getyx(winFromFile.m_mainWin, y, x);

        int count{ 0 };
        int lastEmpty{ 0 };

        for(char& c : winFromFile.m_fileLines.at(y))
        {
            if (count == x)
                break;

            if (c == ' ' && count != x - 1)
                lastEmpty = count;
     
            count++;
        }
        
        wmove(winFromFile.m_mainWin, y, lastEmpty + 1);
        if (lastEmpty == 0)
            wmove(winFromFile.m_mainWin, y, 0);

        wrefresh(winFromFile.m_mainWin);
    }
};

void Movement::endOfWord(int ch, WinFromFile &winFromFile)
{
    if (ch == 'e')
    {
        int y, x;
        getyx(winFromFile.m_mainWin, y, x);

        int count{ 0 };
        int nextEmpty{ 0 };

        for(char& c : winFromFile.m_fileLines.at(y))
        {
            if (c == ' ' && count > x + 1)
            {
                nextEmpty = count;
                break;
            }
            count++;
        }
        
        wmove(winFromFile.m_mainWin, y, nextEmpty - 1);
        if (nextEmpty == 0)
        {
            endOfLine('$', winFromFile);
            getyx(winFromFile.m_mainWin, y, x);

            wmove(winFromFile.m_mainWin, y, x - 1);

        }
        wrefresh(winFromFile.m_mainWin);
    }
};
