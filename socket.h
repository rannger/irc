#ifndef __SOCKET_H_
#define __SOCKET_H_ 
#pragma once
#include <pthread.h>
#include "types.h"
#include "CommandBulider.h"
#include "Command.h"

namespace rirc {
	class Socket
	{
	friend void* handleSocketStream(void* param);
	public:
		Socket(str_t ip,uint32_t port,str_t username,command_handler_t commandHandler);
		~Socket();
		void connect();
		void disconnect();
		void sendCommand(const Command& cmd) const;
		const str_t& username(void) const { return m_username; }
	private:
		Socket();
		const Socket& self;
		const str_t m_ip;
		const str_t m_username;
		const uint32_t m_port;
		const command_handler_t m_commandHandler;

		int m_socketfd;
		pthread_t m_thread;
	};
}


#endif
