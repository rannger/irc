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
		const int retval = poll(&fd,1,1000);
		if (retval < 0) {
			__IF_DO(EINTR == errno,continue;);
			delete socket;
			__EXIT;
		} else if (0==retval) {
			do {
				screen_cord_t cord = {0,0};
				getxy(stdscr,&cord);
				rirc::BaseMessage* msg = rirc::getMessage();
				if (NULL!=msg) {
					if (dynamic_cast<rirc::PrivateMessage*>(msg)==NULL) {
						str_t* message = new str_t(msg->msg());
						*message += "\n";
						attron(COLOR_PAIR(2));
						mvprintw(cord.y,0,message->data());
						attroff(COLOR_PAIR(2));
						printw(">>>");
						refresh();
						delete message;
						delete msg;
					} else {
						rirc::PrivateMessage* message = dynamic_cast<rirc::PrivateMessage*>(msg);
						rirc::Channel* channel = rirc::channel4Name(message->channel());
						channel->addMessage(message);	
						attron(COLOR_PAIR(1));
						mvprintw(cord.y,0,"<%s> ",message->speaker().data());
						attroff(COLOR_PAIR(1));
						attron(COLOR_PAIR(2));
						printw("%s \n",message->msg().data());
						attroff(COLOR_PAIR(2));
						printw(">>>");
						refresh();
					}
				} else {
					break;
				}
			} while(1);
		} else {
			if ((fd.revents & POLLIN) == 0) continue;
			screen_cord_t cord = {0,0};
			getxy(stdscr,&cord);
			std::string in;
			while(1) {
				char ch = 0;
				ch = getch();
				if(0x7F == ch) {
					screen_cord_t cord = {0,0};
					getxy(stdscr,&cord);
					if(cord.x>4) {
						move(cord.y,(cord.x - 3)<=2?3:(cord.x - 3));
						for(int i=0;i<(cord.x - 3);++i)
							delch();
					}
					refresh();
					continue;	
				}
				if ('\n'==ch) break;
				in += ch;
			}
			const std::string str("#rannger");
			rirc::Command* cmd = rirc::CommandBulider::bulidPrivateMsgCommand(in,str);
			socket->sendCommand(*cmd);
			cmd->release();
			move(cord.y, 0);
			clrtoeol();

			attron(COLOR_PAIR(1));
			mvprintw(cord.y,0,"<%s> ",socket->username().data());
			attroff(COLOR_PAIR(1));
			attron(COLOR_PAIR(2));
			printw("%s \n",in.data());
			attroff(COLOR_PAIR(2));

			printw(">>>");
			refresh();
			move(cord.y+1,3);

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
			rirc::Command* cmd = rirc::CommandBulider::bulidJoinCommand("#rannger");
			socket->sendCommand(*cmd);
			cmd->release();
		} else if (msg.command() == str_t("JOIN")||
			msg.command() == str_t("QUIT")||
			msg.command() == str_t("353")||
			msg.command() == str_t("366")) {
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
			str_t channelName(msg.parameters().at(0));
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

