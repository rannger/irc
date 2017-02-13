#include <stdio.h>
#include <fstream>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sched.h>
#include <iostream>
#include <algorithm>
#include <ncurses.h>
#include <assert.h>
#include <typeinfo>
#include <string.h>
#include <poll.h>
#include "socket.h"
#include "Message.h"
#include "macro.h"
#include "CommandBulider.h"
#include "Command.h"
#include "cache.h"
#include "privateMsg.h"
#include "channel.h"
#include "utils.h"

void exitApp(void);
void commandHandler(const rirc::Message& msg,rirc::Socket* socket);
int main(int argc, char const *argv[])
{
	initscr();
	scrollok(stdscr,TRUE);
	start_color();
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);

	rirc::initQueue();
	rirc::Socket* socket = new rirc::Socket("irc.freenode.net",6667,"rst0aic",commandHandler);
	socket->connect();
	
	printw(">>>");
	do {
		struct pollfd fd = {
			STDIN_FILENO,
			POLLIN,
			0
		};
		const int retval = poll(&fd,1,100);
		if (retval < 0) {
			__IF_DO(EINTR == errno,continue;);
			delete socket;
			__EXIT;
		} else if (0==retval) {
			do {
				rirc::BaseMessage* msg = rirc::getMessage();
				if (NULL!=msg) {
					if (dynamic_cast<rirc::PrivateMessage*>(msg)!=NULL) {
						rirc::PrivateMessage* message = dynamic_cast<rirc::PrivateMessage*>(msg);
						rirc::Channel* channel = rirc::channel4Name(message->channel());
						if (channel->messageListSize() == 0) {
							channel->addMessage(message);	
							reloadChannelMessageInWindow(stdscr,message->channel());
						} else {
							channel->addMessage(message);	
							msg->printInWin(stdscr);
						}
					} else if (dynamic_cast<rirc::JoinMessage*>(msg)!=NULL) {
						rirc::JoinMessage* message = dynamic_cast<rirc::JoinMessage*>(msg);
						rirc::Channel* channel = rirc::channel4Name(message->channel());
						reloadChannelMessageInWindow(stdscr,message->channel());
						delete msg;
					} else if (dynamic_cast<rirc::QuitMessage*>(msg)!=NULL) {
						delete socket;
						exitApp();
					} else {
						msg->printInWin(stdscr);
						delete msg;
					}
				} else {
					break;
				}
			} while(1);
		} else {
			if ((fd.revents & POLLIN) == 0) continue;
			screen_cord_t cord = {0,0};
			getxy(stdscr,&cord);
			std::string in("");
			while(1) {
				const char ch = getch();
				if(0x7F == ch) {
					screen_cord_t cord = {0,0};
					getxy(stdscr,&cord);
					if(cord.x>4) {
						move(cord.y,(cord.x - 3)<=2?3:(cord.x - 3));
						for(int i=0;i<(cord.x - 3);++i) {
							delch();
						}
						if (in.length()!=0)
							in.erase(in.length()-1,1);
					}
					refresh();
					continue;	
				}
				if ('\n'==ch)
					 break;
				in += ch;
			}
			if (in.size() == 0) {
				getxy(stdscr,&cord);
				wmove(stdscr,cord.y,0);
				wclrtoeol(stdscr);
				mvwprintw(stdscr,cord.y,0,">>>");
				wrefresh(stdscr);
				continue;
			} else {
				getxy(stdscr,&cord);
				mvwprintw(stdscr,cord.y+1,0,">>>");
				wrefresh(stdscr);

			}
			rirc::Command* cmd = commandAnalysis(in);
			if (NULL != cmd) {
				socket->sendCommand(*cmd);
				cmd->release();
			} else {
				const std::string str("#rannger");
				rirc::Command* cmd = rirc::CommandBulider::bulidPrivateMsgCommand(in,str);
				socket->sendCommand(*cmd);
				cmd->release();
				rirc::PrivateMessage* message = new rirc::PrivateMessage(socket->username(),in,str);
				rirc::Channel* channel = rirc::channel4Name(message->channel());
				channel->addMessage(message);	
				move(cord.y, 0);
				clrtoeol();
				message->printInWin(stdscr);
				move(cord.y+1,3);
			}
		}
	} while(1);
	endwin();
	return 0;
}

void commandHandler(const rirc::Message& msg,rirc::Socket* socket)
{
		if (msg.command() == str_t("PING")) {
			rirc::Command* cmd = rirc::CommandBulider::bulidPongCommand(msg.trail());
			socket->sendCommand(*cmd);
			cmd->release();
		} 
		else if (msg.command() == str_t("ERROR")) {
			__LOG("%s",msg.msg().data());
			delete socket;
			__EXIT;
		} else if (msg.command() == str_t("376")) {
			__LOG("%s",msg.msg().data());
			//rirc::Command* cmd = rirc::CommandBulider::bulidJoinCommand("#rannger");
			//socket->sendCommand(*cmd);
			//cmd->release();
		} else if (msg.command() == str_t("JOIN")) {
			const str_t& channelStr = msg.parameters().at(0);
			str_t channelName;
			str_t userName("");
			for(int i=0;i<msg.prefix().size();++i) {
				if (msg.prefix().at(i)=='!') 
					break;
				userName+=msg.prefix().at(i);
			}
			for(int i=0;i<channelStr.size();++i) {
				const uint8_t val = static_cast<uint8_t>(channelStr.at(i));
				if (val>=0x20&&val<=0x7E)
					channelName+=channelStr.at(i);	
			}

			rirc::JoinMessage* message = new rirc::JoinMessage(channelName,userName);
			queueAdd(message);
		} else if (msg.command() == str_t("QUIT")) {
			str_t userName("");
			for(int i=0;i<msg.prefix().size();++i) {
				if (msg.prefix().at(i)=='!') 
					break;
				userName+=msg.prefix().at(i);
			}
			if (socket->username() == userName) {
				rirc::QuitMessage* message = new rirc::QuitMessage(userName);
				queueAdd(message);
			}
		} else if (msg.command() == str_t("353")||
			msg.command() == str_t("366")) {
			__LOG("%s",msg.msg().data());
			//DO NOT NOTHING.
		} else if(msg.command() == str_t("332")) {
			__LOG("%s%s",KCYN,msg.trail().data());
		} else if (msg.command() == str_t("005")) {
			/*
			for(const str_t &param : msg.parameters())
				__LOG("%s ",param.data());
			__LOG("%s\n",msg.trail().data());
			*/
		} else if (msg.command() == str_t("372")||
			msg.command() == str_t("NOTICE")||
			msg.command() == str_t("001")||
			msg.command() == str_t("002")||
			msg.command() == str_t("003")||
			msg.command() == str_t("004")||
			msg.command() == str_t("251")||
			msg.command() == str_t("252")||
			msg.command() == str_t("253")||
			msg.command() == str_t("254")||
			msg.command() == str_t("255")||
			msg.command() == str_t("250")||
			msg.command() == str_t("265")||
			msg.command() == str_t("266")||
			msg.command() == str_t("375")||
			msg.command() == str_t("376")||
			msg.command() == str_t("333")) {
			//puts(RESET "--------------------------------------------------------------------------------\n" RESET);
			//__LOG("%sprefix %s\n",KRED,msg.prefix().data());
			//__LOG("%scommand %s\n",KRED,msg.command().data());
			//for(const str_t &param : msg.parameters())
			//		__LOG("%sparam %s\n",KRED,param.data());
			//__LOG("%strail %s\n",KRED,msg.trail().data());
			//puts(RESET "--------------------------------------------------------------------------------\n" RESET);
			__LOG("%s%s",KRED,msg.trail().data());
		} else if (msg.command() == str_t("PRIVMSG")) {
			str_t prefix(msg.prefix());
			str_t trail(msg.trail());
			const str_t& channelStr = msg.parameters().at(0);
			str_t channelName("");
			for(int i = 0;i<prefix.size();++i) {
				const uint8_t val = static_cast<uint8_t>(prefix.at(i));
				if (val < 0x20||val > 0x7E )
						prefix.at(i) = 0x20;
			}
			for(int i = 0;i<trail.size();++i) {
				const uint8_t val = static_cast<uint8_t>(trail.at(i));
				if (val < 0x20||val > 0x7E )
						trail.at(i) = 0x20;
			}
			for(int i=0;i<channelStr.size();++i) {
				const uint8_t val = static_cast<uint8_t>(channelStr.at(i));
				if (val>=0x20&&val<=0x7E)
					channelName+=channelStr.at(i);	
			}
			rirc::PrivateMessage* message = new rirc::PrivateMessage(prefix,trail,channelName);
			queueAdd(message);
			//__LOG("%s<%s>%s%s %s",KGRN,msg.prefix().data(),RESET,KMAG,msg.trail().data());
		} else {
			/*
			puts(RESET "--------------------------------------------------------------------------------\n" RESET);
			__LOG("%sprefix %s\n",KRED,msg.prefix().data());
			__LOG("%scommand %s\n",KRED,msg.command().data());
			for(const str_t &param : msg.parameters())
				__LOG("%sparam %s\n",KRED,param.data());
			__LOG("%strail %s\n",KRED,msg.trail().data());
			puts(RESET "--------------------------------------------------------------------------------\n" RESET);
			*/
		}
		sched_yield();
}

void exitApp(void)
{
		do {
			rirc::BaseMessage* msg = rirc::getMessage();
			if (NULL == msg) break;
			delete msg;
		} while(1);
		rirc::clearChannelMap();
		exit(0);
}

