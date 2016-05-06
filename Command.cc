#include "Command.h"
#include "CommandBulider.h"
#include "macro.h"

namespace rirc
{
	Command::Command()
	:self(*this)
	,m_commandName("")
	,m_retainCount(1)
	{
	}

	Command::~Command()
	{
		m_params.clear();
	}

	void Command::release()
	{
		--m_retainCount;
		__IF_DO(0 == m_retainCount,CommandBulider::releaseCommand(this););
	}

	void Command::retain()
	{
		++m_retainCount;
	}

	const str_t& Command::commandName() const
	{
		return m_commandName;
	}

	const std::vector< str_t > Command::params() const
	{
		return m_params;
	}

	str_t* Command::mutable_commandName()
	{
		return &m_commandName;
	}

	std::vector< str_t >* Command::mutable_params()
	{
		return &m_params;
	}
	
	size_t Command::length() const
	{
		size_t retval = 0;
		retval += self.commandName().size();
		++retval;
		for(int i = 0;i<self.params().size();i++) {
			const str_t &param = self.params().at(i);
			retval += param.size();
			++retval;
		}
		retval += 2;
		return retval;
	}

	void Command::serialization(uint8_t *buffer,const size_t len) const
	{
		if (len < this->length()) { return; }
		static const uint8_t byte=0x20;
		static const uint8_t end[]={0x0d,0x0a};
		uint8_t *buf = buffer;
		memset(buf,0,len);
		for(int i = 0;i	< self.commandName().length();i++ ){
			*buf = self.commandName().at(i);
			buf++;
		}
		*(buf++) = byte;
		for (int i = 0;i < self.params().size();i++) {
			const str_t param = self.params().at(i);
			for(int i = 0;i	< param.length();i++ ){
				*buf = param.at(i);
				buf++;
			}
			*(buf++) = byte;
		}
		for(int i = 0;i<2;i++) {
			*(buf++) = end[i];
		}
	}
}

