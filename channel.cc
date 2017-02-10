#include "channel.h"
#include "PrivateMsg.h"
#include <assert.h>
#include <map>


typedef std::map<str_t,rirc::Channel*> channel_map_t;
static channel_map_t *g_channelMap = NULL;

namespace rirc
{
	Channel::Channel() 
	{
		assert(0);	
	}

	Channel::Channel(const str_t& name)
	:m_name(name)
	{
	}

	Channel::~Channel()
	{
		for(int i=0;i<m_messageList.size();++i) {
			PrivateMessage* msg = m_messageList.at(i);
			delete msg;
		}
	}


	void Channel::addMessage(rirc::PrivateMessage* msg)
	{
		m_messageList.push_back(msg);
	}

	static channel_map_t* channeMapSingleInstance(void) 
	{
		if (NULL == g_channelMap) {
			g_channelMap = new channel_map_t();
		}
		return g_channelMap;
	}

	Channel* channel4Name(const str_t& name)
	{
		if (channeMapSingleInstance()->count(name)==0) {
			Channel* channel = new Channel(name);
			channeMapSingleInstance()->insert(std::pair<str_t,rirc::Channel*>(name,channel));
		}
		return channeMapSingleInstance()->at(name);
	}
}



