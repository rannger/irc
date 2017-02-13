#ifndef __PRIVATE_MSG_H
#define __PRIVATE_MSG_H
#include "types.h"
#include <iostream>
#include <ncurses.h>

namespace rirc 
{
	class BaseMessage
	{
	public:
		BaseMessage() {}

		BaseMessage(const str_t msg)
		:m_msg(msg)
		{
		}

		virtual ~BaseMessage() {}

		virtual void printInWin(WINDOW* win);
		str_t msg() const { return m_msg; }
	protected:
		str_t m_msg;
	};

	class PrivateMessage : public BaseMessage
	{
	public:
		PrivateMessage() {}
		PrivateMessage(const str_t speaker,const str_t msg,const str_t channel);
		~PrivateMessage();
		virtual void printInWin(WINDOW* win);
		str_t speaker() const;
		str_t channel() const;
	private:
		str_t m_channel;
		str_t m_speaker;
	};

	class JoinMessage: public BaseMessage
	{
	public:
		JoinMessage() {}
		JoinMessage(const str_t& channelName,const str_t userName)
		:m_channelName(channelName)
		,m_userName(userName)
		{
		}
		~JoinMessage() {}
		const str_t& channel() const { return m_channelName; }
		const str_t& user() const { return m_userName; }
	private:
		const str_t m_channelName;
		const str_t m_userName;
	};

	class QuitMessage: public BaseMessage
	{
	public:
		QuitMessage() {}
		QuitMessage(const str_t& username)
		:m_username(username)
		{
		}
		~QuitMessage() {}
		const str_t& username(void) const { return m_username; }
	private:
		const str_t m_username;
	};
}

#endif
