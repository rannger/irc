#include "socket.h"
#include "Message.h"
#include "macro.h"
#include "CommandBulider.h"
#include "Command.h"
#include <stdio.h>
#include <fstream>
#include <stdlib.h>
#include <ncurses.h>

void commandHandler(const rirc::Message& msg,rirc::Socket* socket);
int main(int argc, char const *argv[])
{
	initscr();
	printw("Hello World !!!");
	refresh();
	rirc::Socket* socket = new rirc::Socket("irc.freenode.net",6667,"rst0aic",commandHandler);
	socket->connect();
	while(1);
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
			exit(0);
		} else if (msg.command() == str_t("376")) {
			__LOG("%s\n",msg.msg().data());
			rirc::Command* cmd = rirc::CommandBulider::bulidJoinCommand("#ubuntu");
			socket->sendCommand(*cmd);
			cmd->release();
		} else if (msg.command() == str_t("JOIN")||
			msg.command() == str_t("QUIT")||
			msg.command() == str_t("353")||
			msg.command() == str_t("366")) {
			//DO NOT NOTHING.
		}
		else if (msg.command() == str_t("372")||
			msg.command() == str_t("NOTICE")||
			msg.command() == str_t("001")||
			msg.command() == str_t("002")||
			msg.command() == str_t("003")||
			msg.command() == str_t("004")||
			msg.command() == str_t("005")||
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
			msg.command() == str_t("332")||
			msg.command() == str_t("333")) {
			__LOG("%s\n",msg.trail().data());
		}
		else if (msg.command() == str_t("PRIVMSG")) {
			__LOG("%s<%s>%s%s %s\n",KGRN,msg.prefix().data(),RESET,KMAG,msg.trail().data());
			std::ofstream file("log.txt",std::ios_base::app);
			char buff[1024];
  			snprintf(buff, sizeof(buff), "<%s> %s\n",msg.prefix().data(),msg.trail().data());
  			std::string buffAsStdStr = buff;
			file << buffAsStdStr;
			file.close();
		} else {
			__LOG("%s%s\n",KRED,msg.msg().data());
		}
}
