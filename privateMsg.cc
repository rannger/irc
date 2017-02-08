#include "privateMsg.h"

namespace rirc
{
	PrivateMessage::PrivateMessage(const str_t speaker,const str_t msg)
	:BaseMessage(msg)
	,m_speaker(speaker)
	{
	}

	PrivateMessage::~PrivateMessage() {}

	str_t PrivateMessage::speaker() const 
	{
		return m_speaker;
	}
}


