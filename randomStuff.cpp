#include "randomStuff.h"
#include "winFromFile.h"
#include <iostream>
#include <string>

void RandomStuff::outputInBash(std::string text)
{
    def_prog_mode();		/* Save the tty modes		  */
	endwin();			/* End curses mode temporarily	  */
    std::string echoTxt = "echo " + text;
	system(echoTxt.c_str());		/* Do whatever you like in cooked mode */
	reset_prog_mode();		/* Return to the previous tty mode*/
					/* stored by def_prog_mode() 	  */
	refresh();			/* Do refresh() to restore the	  */
}

