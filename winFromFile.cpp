#include "winFromFile.h"
#include "featureNamespace.h"
#include <fstream>
#include <iostream>
#include "randomStuff.h"
#include "movement.h"

/*
Fuetaure List:
- paar movemnts: anfang zeile, ende zeile
*/

WinFromFile::WinFromFile(char* fileName)
{
    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    noecho();

    m_fileName = fileName;
    m_mainWin = newwin(LINES, COLS, 0, 0);
    make_FileLines();
    m_startLine = 0;
    m_endLine = LINES - 1;
    if (LINES > m_fileLines.size())
            m_endLine = m_fileLines.size();

    refresh();

    m_yBefore = LINES; 
    m_xBefore = COLS;

    update();

}

WinFromFile::~WinFromFile()
{
    delwin(m_mainWin);
    endwin();
}


void WinFromFile::make_FileLines()
{
    std::string line;
    std::ifstream myfile (m_fileName);
    if (myfile.is_open())
    {
        while (std::getline(myfile,line))
        {
            m_fileLines.push_back(line);

        }
        myfile.close();
    }
    else
        std::cout << "Unable to open file";
}

void WinFromFile::saveToFile()
{
    std::ofstream myfile(m_fileName);
    if (myfile.is_open())
    {
        for (auto & lineElem : m_fileLines) {
            myfile << lineElem << "\n";
        }
        myfile.close();
    }
    else
        std::cout << "Unable to open file";
}

void WinFromFile::make_Window()
{
    clear();
    refresh();
    for (int i{m_startLine}; i < m_endLine; ++i)
    {
        const char* lineString = m_fileLines.at(i).c_str();
        mvwaddnstr(m_mainWin, i - m_startLine, 0, lineString, COLS);
    }
    updateStatus(m_mode);
    wrefresh(m_mainWin);
}

void WinFromFile::updateStatus(WinFromFile::Mode mode)
{
    int beforeY, beforeX;
    getyx(m_mainWin, beforeY, beforeX);
    m_mode = mode; 


    char statusBarStr[100];
    FeatureNamespace::returnStatusBar(*this, statusBarStr);

    int maxY, maxX;
    getmaxyx(m_mainWin, maxY, maxX);

    mvwprintw(m_mainWin, maxY - 1, 0, statusBarStr);

    wmove(m_mainWin, beforeY, beforeX); //to have pos before status update
    wrefresh(m_mainWin);
}

void WinFromFile::updateForScrolling(int ch)
{
switch(ch)
{
    case KEY_UP:

        if (m_startLine - 1 >= 0)
        {
            m_startLine--;
            /* RandomStuff::outputInBash(std::to_string(COLS));
            RandomStuff::outputInBash(std::to_string(LINES));
 */
            if (LINES < m_fileLines.size())
                m_endLine--;
            make_Window();
        }
        break;
    case KEY_DOWN:

        if (m_fileLines.size() >= m_endLine + 1)
        {
            m_startLine++;
            m_endLine++;
            make_Window();
        }
        break;
    }
}

void WinFromFile::update()
{
    make_Window();
    int ch;
    while((ch = getch()) != KEY_F(1))
    {
        updateForScrolling(ch);
        updateForCursor(ch);
        FeatureNamespace::updateForReplace(ch, *this);
        FeatureNamespace::updateForDelete(ch, *this);
        FeatureNamespace::updateForInsert(ch, *this);

        updateForResize();
        updateForSave(ch);

        Movement::startOfLine(ch, *this);
        Movement::endOfLine(ch, *this);
        Movement::beginingOfWord(ch, *this);
        Movement::endOfWord(ch, *this);
    }
}

void WinFromFile::updateForCursor(int ch)
{
    int y, x;

    getyx(m_mainWin, y, x);           /* get the current curser position */

    switch(ch)
    {
            case 104:
                    wmove(m_mainWin, y, x - 1);
                    break;
            case 108:
                    wmove(m_mainWin, y, x + 1);
                    break;
            case 106:
                    wmove(m_mainWin, y + 1, x);
                    break;
            case 107:
                    wmove(m_mainWin, y - 1, x);
                    break;
    }
    wrefresh(m_mainWin);
}

void WinFromFile::updateForResize()
{
    int maxY, maxX;
    getmaxyx(m_mainWin, maxY, maxX);


    if (maxY != m_yBefore) 
    {
        if (maxY > m_fileLines.size()) 
            m_endLine = m_fileLines.size();
        else 
            m_endLine = maxY - 1;

        rezisePosBefore();
    } else if (maxX != m_xBefore)
        rezisePosBefore();
}

void WinFromFile::rezisePosBefore()
{
    int maxY, maxX;
    getmaxyx(m_mainWin, maxY, maxX);

    int beforeY, beforeX;
    getyx(m_mainWin, beforeY, beforeX);

    make_Window();

    wmove(m_mainWin, beforeY, beforeX); //to have pos before status update
    wrefresh(m_mainWin);

    m_yBefore = maxY;
    m_xBefore = maxX;
}

void WinFromFile::updateForSave(int ch)
{
    if (ch == KEY_F(2)) 
    {
        saveToFile();
    }
}

void WinFromFile::renewLine(int line)
{
    if (line > m_fileLines.size()) 
    {
        for (int i{m_fileLines.size()}; i <= line; ++i)
        {
            m_fileLines.push_back("");
        }
        m_endLine = m_fileLines.size();
    }
    int maxY, maxX;
    getmaxyx(m_mainWin, maxY, maxX);

    int y, x;
    getyx(m_mainWin, y, x);

    int currentX = 0;

    std::string beforeString{ m_fileLines.at(line + m_startLine) };
    m_fileLines.at(line + m_startLine) = "";

    for (int i{0}; i < maxX; ++i)
    {
        unsigned int chtypeText = mvwinch(m_mainWin, line, currentX) & A_CHARTEXT;
        char ch{ static_cast<char>(chtypeText) };
        m_fileLines.at(line + m_startLine) += ch;
        
        currentX++;
    }
    
    int count{ 0 };
    for(char& c : beforeString)
    {
        if (count > maxX)
                m_fileLines.at(line + m_startLine) += c;
        
        count++;
    }

    wmove(m_mainWin, y, x);
}
