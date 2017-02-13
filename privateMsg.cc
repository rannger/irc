#include "privateMsg.h"
#include "utils.h"

namespace rirc
{
	void BaseMessage::printInWin(WINDOW* win)
	{
		screen_cord_t cord = {0,0};
		getxy(win,&cord);

		str_t* message = new str_t(this->msg());
		*message += "\n";
		attron(COLOR_PAIR(2));
		mvwprintw(win,cord.y,0,message->data());
		attroff(COLOR_PAIR(2));
		wprintw(win,">>>");
		wrefresh(win);
		delete message;
	}

	PrivateMessage::PrivateMessage(const str_t speaker,const str_t msg,const str_t channel)
	:BaseMessage(msg)
	,m_channel(channel)
	,m_speaker(speaker)
	{
	}

	PrivateMessage::~PrivateMessage() {}

	str_t PrivateMessage::speaker() const 
	{
		return m_speaker;
	}

	str_t PrivateMessage::channel() const 
	{
		return m_channel;
	}

	void PrivateMessage::printInWin(WINDOW* win)
	{
		screen_cord_t cord = {0,0};
		getxy(win,&cord);

		attron(COLOR_PAIR(1));
		mvwprintw(win,cord.y,0,"<%s> ",this->speaker().data());
		attroff(COLOR_PAIR(1));
		attron(COLOR_PAIR(2));
		wprintw(win,"%s \n",this->msg().data());
		attroff(COLOR_PAIR(2));
		wprintw(win,">>>");
		wrefresh(win);
	}

}


