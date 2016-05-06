#ifndef __COMMAND_H_
#define __COMMAND_H_
#pragma once
#include "types.h"

namespace rirc
{
	class Command
	{
	public:
		Command();
		virtual ~Command();
		const std::string& commandName() const;
		const std::vector< std::string > params() const;
		std::string* mutable_commandName();
		std::vector< std::string >* mutable_params();
		void serialization(uint8_t *buffer,const size_t len) const;
		size_t length() const;
		void release();
		void retain();
	private:
		const Command& self;
		std::string m_commandName;
		std::vector< std::string > m_params;
		int m_retainCount;
	};
}

#endif
