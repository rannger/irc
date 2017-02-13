#ifndef __PRIVATE_MSG_H
#define __PRIVATE_MSG_H
#include "types.h"
#include <iostream>
#include <ncurses.h>
#include <assert.h>

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
		PrivateMessage() { assert(0); }
		PrivateMessage(const str_t speaker,const str_t msg,const str_t channel);
		~PrivateMessage();
		virtual void printInWin(WINDOW* win);
		const str_t& speaker() const;
		const str_t& channel() const;
	private:
		str_t m_channel;
		str_t m_speaker;
	};

	class JoinMessage: public BaseMessage
	{
	public:
		JoinMessage() { assert(0); }
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
		QuitMessage() { assert(0); }
		QuitMessage(const str_t& username)
		:m_username(username)
		{
		}
		~QuitMessage() {}
		const str_t& username(void) const { return m_username; }
	private:
		const str_t m_username;
	};

	class PartMessage: public BaseMessage
	{
	public:
		PartMessage() { assert(0); }
		PartMessage(const str_t &username,const str_t channelname)
		:m_username(username)
		,m_channelname(channelname)
		{
		}
		~PartMessage() {}
		const str_t& username(void) const { return m_username; }
		const str_t channelname(void) const { return m_channelname; }
	private:
		const str_t m_username;
		const str_t m_channelname;
	};
}

#endif
