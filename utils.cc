#include "utils.h"
#include "types.h"
#include "channel.h"
#include "PrivateMsg.h"

void getxy(WINDOW* win,screen_cord_t* cord)
{
	bzero(cord,sizeof(screen_cord_t));
	getyx(win,cord->y,cord->x);
}

void reloadChannelMessageInWindow(WINDOW* win,const str_t& channelName)
{
	rirc::Channel* channel = rirc::channel4Name(channelName);
	wclear(win);
	move(0,0);
	for(int i=0;i<channel->messageList().size();++i) {
		screen_cord_t cord = {0,0};
		getxy(win,&cord);
		rirc::PrivateMessage* message = channel->messageList().at(i);
		wattron(win,COLOR_PAIR(1));
		mvwprintw(win,cord.y,0,"<%s> ",message->speaker().data());
		wattroff(win,COLOR_PAIR(1));
		wattron(win,COLOR_PAIR(2));
		wprintw(win,"%s \n",message->msg().data());
		wattroff(win,COLOR_PAIR(2));
		wprintw(win,">>>");
		wrefresh(win);
	}
}

