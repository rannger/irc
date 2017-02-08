#ifndef __PRIVATE_MSG_H
#define __PRIVATE_MSG_H
#include "types.h"
#include <iostream>

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

		str_t msg() const { return m_msg; }
	protected:
		str_t m_msg;
	};

	class PrivateMessage : public BaseMessage
	{
	public:
		PrivateMessage() {}
		PrivateMessage(const str_t speaker,const str_t msg);
		~PrivateMessage();
		str_t speaker() const;
	private:
		str_t m_speaker;
	};
}

#endif
