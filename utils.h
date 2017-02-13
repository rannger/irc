#ifndef __UTILS_H
#define __UTILS_H
#include <ncurses.h>
#include "types.h"

namespace rirc
{
	class Command;
}
typedef struct screen_cord_t
{
	int x;
	int y;
} screen_cord_t;


void getxy(WINDOW* win,screen_cord_t* cord);
void reloadChannelMessageInWindow(WINDOW* win,const str_t& channelName);
rirc::Command* commandAnalysis(const str_t &cmdStr);
#endif

