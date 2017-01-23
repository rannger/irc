#ifdef __WIN32__
# include <winsock2.h>
#else
# include <sys/socket.h>
# include <netdb.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <sys/uio.h>
#endif
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sstream>
#include <istream>
#include <stdlib.h>
#include "socket.h"
#include "Command.h"
#include "CommandBulider.h"
#include "macro.h"
#include "Message.h"
#include <ncurses.h>

inline slist_t splitString(const std::string &s) 
{
    slist_t elems;
    std::istringstream stream(s);
    str_t str;

	while (std::getline(stream, str)) {
        elems.push_back(str);
    }

    return elems;
}

namespace rirc {
	Socket::Socket(str_t ip,uint32_t port,str_t username,command_handler_t commandHandler)
	:self(*this)
	,m_ip(ip)
	,m_username(username)
	,m_port(port)
	,m_commandHandler(commandHandler)
	,m_socketfd(-1)
	{
		if (pthread_mutex_init(&m_lock, NULL) != 0) {
	        	__LOGS("\n mutex init failed\n");
	        	exit(-1);
	    	}
	}

	Socket::~Socket()
	{
		pthread_mutex_destroy(&m_lock);
	}

	void Socket::connect() 
	{
		struct sockaddr_in server;
		struct hostent *hserver = NULL;
		int socket = ::socket(AF_INET , SOCK_STREAM , 0);
  		__CHECK(socket > 0);

  		bzero(&server,sizeof(struct sockaddr_in));
  		hserver = gethostbyname(m_ip.data());

  		bcopy(hserver->h_addr, (char *)&server.sin_addr.s_addr,hserver->h_length);
  		server.sin_family = AF_INET;
	    	server.sin_port = htons(m_port);
	
	    	__IF_DO(::connect(socket , (struct sockaddr *)&server , sizeof(server)) < 0,
			__LOGS("connect error");
	        	return;);
	    	m_socketfd = socket;
	
	    	Command* cmds[3] = {CommandBulider::bulidPassCommand(m_username),
       				    CommandBulider::bulidNickCommand(m_username),
	    			    CommandBulider::bulidUserCommand(m_username.data(),
	    				       			     "tolmoon",
	    							     "tolsun",
	    							     ":Ronnie Reagan")};
	
	    	for (int i = 0; i < 3; ++i)
	    	{
	    		Command* cmd = cmds[i];
	    		self.sendCommand(*cmd);
	    		delete cmd;
	    		cmds[i] = NULL;
	    	}

   		pthread_attr_t attr;

   		pthread_attr_init(&attr);
   		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

   		int res = pthread_create(&m_thread, &attr, Socket::handleSocketStream, (void *)this); 
   		pthread_attr_destroy(&attr);
		__IF_DO(res<0,exit(-1););
	}

	void Socket::disconnect() 
	{
		__CHECK(m_socketfd > 0);
		Command* cmd = CommandBulider::bulidQuitCommand("");
		self.sendCommand(*cmd);
		delete cmd;
		close(m_socketfd);
		m_socketfd = 0;
		pthread_cancel(m_thread);
	}
	
	void Socket::sendCommand(const Command& cmd) const
	{
		const size_t size = cmd.length();
		uint8_t *buf = new uint8_t[size];
		cmd.serialization(buf,size);
		write(m_socketfd,buf,size);
		delete [] buf;
	}

	void* Socket::handleSocketStream(void* param)
	{
		const Socket& self = *((Socket*)param);
		uint8_t buf[512];
		do {
			bzero(buf,512*sizeof(uint8_t));
			const ssize_t size = ::read(self.m_socketfd,buf,512);
			
			if (size > 0) {
				const str_t msg((char*)buf,size);
				const slist_t array = ::splitString(msg);

				for (const str_t& mesg : array){
					Message msg = Message::parseMessage(mesg);
					pthread_mutex_lock((pthread_mutex_t*)&self.m_lock);
					self.m_commandHandler(msg,(Socket*)param);
					pthread_mutex_unlock((pthread_mutex_t*)&self.m_lock);
				}
			}
		} while (true);	

		return NULL;
	}

}
