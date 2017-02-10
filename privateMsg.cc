#include "privateMsg.h"

namespace rirc
{
	PrivateMessage::PrivateMessage(const str_t speaker,const str_t msg,const str_t channel)
	:BaseMessage(msg)
	,m_channel(channel)
	,m_speaker(speaker)
	{
	}

	PrivateMessage::~PrivateMessage() {}

	str_t PrivateMessage::speaker() const 
	{
		return m_speaker;
	}

	str_t PrivateMessage::channel() const 
	{
		return m_channel;
	}
}


