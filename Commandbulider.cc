#include "CommandBulider.h"
#include "Command.h"

namespace rirc {

	Command* CommandBulider::bulidNickCommand(const str_t& nickName)
	{
		Command* cmd = new Command();
		*(cmd->mutable_commandName()) = "NICK";
		cmd->mutable_params()->push_back(nickName);
		return cmd;
	}

	Command* CommandBulider::bulidNickCommand(const char* nickName,const size_t len)
	{
		const str_t nick(nickName,len);
		return bulidNickCommand(nick);
	}

	Command* CommandBulider::bulidNickCommand(const char* nickName)
	{
		const str_t nick(nickName);
		return bulidNickCommand(nick);
	}

	Command* CommandBulider::bulidPassCommand(const str_t& pwd)
	{
		Command* cmd = new Command();
		*(cmd->mutable_commandName()) = "PASS";
		cmd->mutable_params()->push_back(pwd);
		return cmd;
	}

	Command* CommandBulider::bulidPassCommand(const char* pwd,const size_t len)
	{
		const str_t password(pwd,len);
		return bulidPassCommand(password);
	}

	Command* CommandBulider::bulidPassCommand(const char* pwd)
	{
		const str_t password(pwd);
		return bulidPassCommand(password);
	}

	Command* CommandBulider::bulidUserCommand(const str_t& userName,
							 const str_t& hostName,
							 const str_t& serverName,
							 const str_t& realName)
	{
		Command* cmd = new Command();
		*(cmd->mutable_commandName()) = "USER";
		cmd->mutable_params()->push_back(userName);
		cmd->mutable_params()->push_back(hostName);
		cmd->mutable_params()->push_back(serverName);
		cmd->mutable_params()->push_back(realName);	
		return cmd;
	}

	Command* CommandBulider::bulidUserCommand(const char* userName,
				   		 	const char* hostName,
						 	const char* serverName,
						 	const char* realName)
	{
		const str_t uname(userName);
		const str_t hname(hostName);
		const str_t sname(serverName);
		const str_t rname(realName);

		return bulidUserCommand(uname,hname,sname,rname);
	}

	Command* CommandBulider::bulidUserCommand(const char* userName,const size_t ulen,
						 const char* hostName,const size_t hlen,
						 const char* serverName,const size_t slen,
						 const char* realName,const size_t rlen)
	{
		const str_t uname(uname,ulen);
		const str_t hname(hostName,hlen);
		const str_t sname(serverName,slen);
		const str_t rname(realName,rlen);

		return bulidUserCommand(uname,hname,sname,rname);
	}


	Command* CommandBulider::bulidWhoisCommand(const str_t& nickName)
	{
		Command* cmd = new Command();
		*(cmd->mutable_commandName()) = "WHOIS";
		cmd->mutable_params()->push_back(nickName);
		return cmd;
	}

	Command* CommandBulider::bulidWhoisCommand(const char* nickName)
	{
		const str_t nname(nickName);
		return bulidWhoisCommand(nname);
	}

	Command* CommandBulider::bulidWhoisCommand(const char* nickName,const size_t len)
	{
		const str_t nname(nickName,len);
		return bulidWhoisCommand(nname);
	}


	Command* CommandBulider::bulidQuitCommand(const str_t& msg)
	{
		Command* cmd = new Command();
		*(cmd->mutable_commandName()) = "QUIT";
		cmd->mutable_params()->push_back(msg);
		return cmd;
	}

	Command* CommandBulider::bulidQuitCommand(const char* msg)
	{
		const str_t mesg(msg);
		return bulidQuitCommand(msg);
	}

	Command* CommandBulider::bulidQuitCommand(const char* msg,const size_t len)
	{
		const str_t mesg(msg,len);
		return bulidQuitCommand(msg);
	}

		
	Command* CommandBulider::bulidJoinCommand(const str_t& channel)
	{
		Command* cmd = new Command();
		*(cmd->mutable_commandName()) = "JOIN";
		cmd->mutable_params()->push_back(channel);
		return cmd;
	}

	Command* CommandBulider::bulidJoinCommand(const char* channel)
	{
		const str_t ch(channel);
		return bulidJoinCommand(ch);
	}

	Command* CommandBulider::bulidJoinCommand(const char* channel,const size_t len)
	{
		const str_t ch(channel,len);
		return bulidJoinCommand(ch);
	}


	Command* CommandBulider::bulidPrivateMsgCommand(const str_t& msg,const str_t& receiver)
	{
		Command* cmd = new Command();
		*(cmd->mutable_commandName()) = "PRIVMSG";
		cmd->mutable_params()->push_back(msg);	
		cmd->mutable_params()->push_back(receiver);	

		return cmd;
	}

	Command* CommandBulider::bulidPrivateMsgCommand(const char* msg,
							       const char* receiver)
	{
		const str_t mesg(msg);
		const str_t recv(receiver);
		return bulidPrivateMsgCommand(mesg,recv);
	}

	Command* CommandBulider::bulidPrivateMsgCommand(const char* msg,const size_t mlen,
							       const char* receiver,const size_t rlen)
	{
		const str_t mesg(msg,mlen);
		const str_t recv(receiver,rlen);
		return bulidPrivateMsgCommand(mesg,recv);
	}


	Command* CommandBulider::bulidListCommand(const str_t& channel)
	{
		Command* cmd = new Command();
		*(cmd->mutable_commandName()) = "LIST";
		cmd->mutable_params()->push_back(channel);	
		return cmd;
	}

	Command* CommandBulider::bulidListCommand(const char* channel)
	{
		const str_t ch(channel);
		return bulidListCommand(ch);
	}

	Command* CommandBulider::bulidListCommand(const char* channel,const size_t len)
	{
		const str_t ch(channel,len);
		return bulidListCommand(ch);
	}


	Command* CommandBulider::bulidPingCommand(const str_t& receiver)
	{
		Command* cmd = new Command();
		*(cmd->mutable_commandName()) = "PING";
		cmd->mutable_params()->push_back(receiver);
		return cmd;
	}

	Command* CommandBulider::bulidPingCommand(const char* receiver)
	{
		const str_t ch(receiver);
		return bulidPingCommand(ch);
	}

	Command* CommandBulider::bulidPingCommand(const char* receiver,const size_t len)
	{
		const str_t ch(receiver,len);
		return bulidPingCommand(ch);
	}


	Command* CommandBulider::bulidPongCommand(const str_t& receiver)
	{
		Command* cmd = new Command();
		*(cmd->mutable_commandName()) = "PONG";
		cmd->mutable_params()->push_back(receiver);
		return cmd;
	}

	Command* CommandBulider::bulidPongCommand(const char* receiver)
	{
		const str_t ch(receiver);
		return bulidPongCommand(ch);
	}

	Command* CommandBulider::bulidPongCommand(const char* receiver,const size_t len)
	{
		const str_t ch(receiver,len);
		return bulidPongCommand(ch);
	}


	Command* CommandBulider::bulidPartCommand(const str_t& channel)
	{
		Command* cmd = new Command();
		*(cmd->mutable_commandName()) = "PART";
		cmd->mutable_params()->push_back(channel);	
		return cmd;
	}

	Command* CommandBulider::bulidPartCommand(const char* channel)
	{
		const str_t ch(channel);
		return bulidPartCommand(ch);
	}

	Command* CommandBulider::bulidPartCommand(const char* channel,const size_t len)
	{
		const str_t ch(channel,len);
		return bulidPartCommand(ch);
	}


	Command* CommandBulider::bulidNoticeCommand(const str_t& nickName,const str_t& msg)
	{
		Command* cmd = new Command();
		*(cmd->mutable_commandName()) = "NOTICE";
		cmd->mutable_params()->push_back(nickName);	
		cmd->mutable_params()->push_back(msg);
		return cmd;
	}

	Command* CommandBulider::bulidNoticeCommand(const char* nickName,const size_t nlen,
						   	   const char* msg,const size_t mlen)
	{
		const str_t name(nickName,nlen);
		const str_t mesg(msg,mlen);
		return bulidNoticeCommand(name,mesg);
	}

	Command* CommandBulider::bulidNoticeCommand(const char* nickName,const char* msg)
	{
		const str_t name(nickName);
		const str_t mesg(msg);
		return bulidNoticeCommand(name,mesg);
	}

	void CommandBulider::releaseCommand(Command* cmd)
	{
		delete cmd;
	}
}

