#ifndef __COMMAND_BULIDER_
#define __COMMAND_BULIDER_
#pragma once
#include "types.h"

namespace rirc {
	class Command;
	class CommandBulider
	{
	public:
		static Command* bulidNickCommand(const str_t& nickName);
		static Command* bulidNickCommand(const char* nickName,const size_t len);
		static Command* bulidNickCommand(const char* nickName);

		static Command* bulidPassCommand(const str_t& pwd);
		static Command* bulidPassCommand(const char* pwd,const size_t len);
		static Command* bulidPassCommand(const char* pwd);

		static Command* bulidUserCommand(const str_t& userName,
						 const str_t& hostName,
						 const str_t& serverName,
						 const str_t& realName);
		static Command* bulidUserCommand(const char* userName,
						 const char* hostName,
						 const char* serverName,
						 const char* realName);
		static Command* bulidUserCommand(const char* userName,const size_t ulen,
						 const char* hostName,const size_t hlen,
						 const char* serverName,const size_t slen,
						 const char* realName,const size_t rlen);

		static Command* bulidWhoisCommand(const str_t& nickName);
		static Command* bulidWhoisCommand(const char* nickName);
		static Command* bulidWhoisCommand(const char* nickName,const size_t len);

		static Command* bulidQuitCommand(const str_t& msg);
		static Command* bulidQuitCommand(const char* msg);
		
		static Command* bulidJoinCommand(const str_t& channel);
		static Command* bulidJoinCommand(const char* channel);
		static Command* bulidJoinCommand(const char* channel,const size_t len);

		static Command* bulidPrivateMsgCommand(const str_t& msg,const str_t& receiver);
		static Command* bulidPrivateMsgCommand(const char* msg,
						       const char* receiver);
		static Command* bulidPrivateMsgCommand(const char* msg,const size_t mlen,
						       const char* receiver,const size_t rlen);

		static Command* bulidListCommand(const str_t& channel);
		static Command* bulidListCommand(const char* channel);
		static Command* bulidListCommand(const char* channel,const size_t len);

		static Command* bulidPingCommand(const str_t& receiver);
		static Command* bulidPingCommand(const char* receiver);
		static Command* bulidPingCommand(const char* receiver,const size_t len);

		static Command* bulidPongCommand(const str_t& receiver);
		static Command* bulidPongCommand(const char* receiver);
		static Command* bulidPongCommand(const char* receiver,const size_t len);

		static Command* bulidPartCommand(const str_t& channel);
		static Command* bulidPartCommand(const char* channel);
		static Command* bulidPartCommand(const char* channel,const size_t len);

		static Command* bulidNoticeCommand(const str_t& nickName,const str_t& msg);
		static Command* bulidNoticeCommand(const char* nickName,const size_t nlen,
						   const char* msg,const size_t mlen);
		static Command* bulidNoticeCommand(const char* nickName,const char* msg);

		static void releaseCommand(Command* cmd);
	private:
		CommandBulider() {}
		~CommandBulider() {}
	};
}

#endif

