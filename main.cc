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
#include "socket.h"
#include "Message.h"
#include "macro.h"
#include "CommandBulider.h"
#include "Command.h"
#include "cache.h"
#include "privateMsg.h"

void commandHandler(const rirc::Message& msg,rirc::Socket* socket);
int main(int argc, char const *argv[])
{
	initscr();
	scrollok(stdscr,TRUE);
	start_color();
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);

	fd_set rfds;
        struct timeval tv = {0,0};
	rirc::initQueue();
	rirc::Socket* socket = new rirc::Socket("irc.freenode.net",6667,"rst0aic",commandHandler);
	socket->connect();
	
	do {
		/* Watch stdin (fd 0) to see when it has input. */
		FD_ZERO(&rfds);
		FD_SET(0, &rfds);
	        /* Wait up to five seconds. */
		tv.tv_sec = 1;
	        tv.tv_usec = 0;

		const int retval = select(1, &rfds, NULL, NULL, &tv);
		if (retval < 0) {
			delete socket;
			exit(-1);
		} else if (0==retval) {
			do {
				rirc::BaseMessage* msg = rirc::getMessage();
				if (NULL!=msg) {
					if (dynamic_cast<rirc::PrivateMessage*>(msg)==NULL) {
						str_t* message = new str_t(msg->msg());
						for(int i = 0;i<message->size();++i) {
							const uint8_t val = static_cast<uint8_t>(message->at(i));
							if (val < 0x20||val > 0x7E )
							message->at(i) = 0x20;
						}

						*message += "\n";
						attron(COLOR_PAIR(2));
						printw(message->data());
						attroff(COLOR_PAIR(2));
						refresh();
						delete message;
						delete msg;
					} else {
						rirc::PrivateMessage* message = dynamic_cast<rirc::PrivateMessage*>(msg);
						attron(COLOR_PAIR(1));
						printw("<%s> ",message->speaker().data());
						attroff(COLOR_PAIR(1));
						attron(COLOR_PAIR(2));
						printw("%s \n",message->msg().data());
						attroff(COLOR_PAIR(2));
						refresh();
						delete message;
					}
				} else {
					break;
				}
			} while(1);
		} else {
			std::string in;
			std::cin >> in;
			const std::string str("#ubuntu");
			rirc::Command* cmd = rirc::CommandBulider::bulidPrivateMsgCommand(in,str);
			socket->sendCommand(*cmd);
			cmd->release();
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
			exit(-1);
		} else if (msg.command() == str_t("376")) {
			__LOG("%s",msg.msg().data());
			rirc::Command* cmd = rirc::CommandBulider::bulidJoinCommand("#ubuntu");
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
			rirc::PrivateMessage* message= new rirc::PrivateMessage(prefix,trail);
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
