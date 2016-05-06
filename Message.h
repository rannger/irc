#ifndef __MESSAGE_
#define __MESSAGE_
#pragma once
#include "types.h"

namespace rirc {
	extern const str_t kMessagePrefix;
	extern const str_t kMessageCommand;
	extern const str_t kMessageArges;
	extern const str_t kMessageTail;
	extern const str_t kMessageSource;
	class Message
	{
	public:
		static Message parseMessage(const str_t& message);
		static const dict_t& ircFlags();
		Message();
		Message(const str_t& msg,
				const std::string & command, 
				const std::string & prefix = "", 
				const std::vector<str_t>& parameters = {}, 
				const std::string & trail = "");
		virtual ~Message();

		const str_t& msg() const {return m_msg;}
		const str_t& command() const {return m_Command;}
		const str_t& prefix() const {return m_Prefix;}
		const std::vector<str_t>& parameters() const {return m_Parameters;}
		const str_t& trail() const {return m_Trail;}
	private:
		const str_t 		m_msg;
		const str_t			m_Command;
		const str_t			m_Prefix;
		const std::vector<str_t> m_Parameters;
		const str_t			m_Trail;
	};
}

#endif