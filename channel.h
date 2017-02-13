#ifndef __CHANNEL_H
#define __CHANNEL_H

#include "types.h"
#include <vector>
#include <map>

namespace rirc
{
	class PrivateMessage;
}

typedef std::vector<rirc::PrivateMessage*> message_list_t;

namespace rirc
{
	class Channel
	{
	public:
		Channel();
		Channel(const str_t& name);
		~Channel();
		
		str_t name(void) const { return m_name; }
		void addMessage(rirc::PrivateMessage* msg);
		const message_list_t& messageList() const { return m_messageList;}
		int messageListSize() const { return m_messageList.size(); }
	private:
		const str_t m_name;	
		message_list_t m_messageList;
	};

	Channel* channel4Name(const str_t& name);
	void clearChannelMap(void);
}


#endif
