#include "utils.h"
#include "types.h"
#include "channel.h"
#include "PrivateMsg.h"
#include "CommandBulider.h"
#include "Command.h"
#include <algorithm>
#include "CommandBulider.h"

void getxy(WINDOW* win,screen_cord_t* cord)
{
	bzero(cord,sizeof(screen_cord_t));
	getyx(win,cord->y,cord->x);
}

void reloadChannelMessageInWindow(WINDOW* win,const str_t& channelName)
{
	rirc::Channel* channel = rirc::channel4Name(channelName);
	wclear(win);
	mvwprintw(win,0,0,">>>");
	for(int i=0;i<channel->messageList().size();++i) {
		screen_cord_t cord = {0,0};
		getxy(win,&cord);
		rirc::PrivateMessage* message = channel->messageList().at(i);
		message->printInWin(stdscr);
	}
}

rirc::Command* commandAnalysis(const str_t &cmdStr)
{
	if (cmdStr.length() == 0) return NULL;
	if (cmdStr.at(0) != '/') return NULL;
	str_t cmd;
	int i = 0;
	for(i=1;i<cmdStr.size();++i) {
		if(cmdStr.at(i)!=' ')
			cmd += cmdStr.at(i);
		else
			break;
	}
	++i;
	std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);
	rirc::Command* ret = NULL;
	if (str_t("join") == cmd) {
		str_t channelName;
		for(;i<cmdStr.size();++i) {
			channelName += cmdStr.at(i);
		}
		ret = rirc::CommandBulider::bulidJoinCommand(channelName);
	} else if (str_t("part") == cmd) {
		str_t channelName;
		for(;i<cmdStr.size();++i) {
			channelName += cmdStr.at(i);
		}
		ret = rirc::CommandBulider::bulidPartCommand(channelName);
	} else if (str_t("quit") == cmd) {
		str_t msg;
		for(;i<cmdStr.size();++i) 
			msg += cmdStr.at(i);
		ret = rirc::CommandBulider::bulidQuitCommand(msg);
	}

	return ret;
}
