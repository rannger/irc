#ifdef __WIN32__
# include <winsock2.h>
#else
# include <sys/socket.h>
# include <netdb.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <sys/uio.h>
#endif
#include <sys/time.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sstream>
#include <istream>
#include <sched.h>
#include "socket.h"
#include "Command.h"
#include "CommandBulider.h"
#include "macro.h"
#include "Message.h"

inline slist_t splitString(const std::string &s) 
{
    	slist_t elems;
    	std::istringstream stream(s);
    	str_t str;

	while (std::getline(stream, str)) {
       		 elems.push_back(str);
		 str.clear();
    	}
    	return elems;
}

namespace rirc {
	void* handleSocketStream(void* param);
	Socket::Socket(str_t ip,uint32_t port,str_t username,command_handler_t commandHandler)
	:self(*this)
	,m_ip(ip)
	,m_username(username)
	,m_port(port)
	,m_commandHandler(commandHandler)
	,m_socketfd(-1)
	{
	}

	Socket::~Socket()
	{
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
			puts("connect error");
	        	return;);
	    	m_socketfd = socket;
	
	    	Command* cmds[3] = {CommandBulider::bulidPassCommand(m_username),
       				    CommandBulider::bulidNickCommand(m_username),
	    			    CommandBulider::bulidUserCommand(m_username.data(),
	    				       			     "tolmoon",
	    							     "tolsun",
	    							     ":Ronnie Reagan")};
	
	    	for (int i = 0; i < 3; ++i) {
	    		Command* cmd = cmds[i];
	    		self.sendCommand(*cmd);
	    		delete cmd;
	    		cmds[i] = NULL;
	    	}
		
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
		int res = pthread_create(&m_thread, 
					&attr, 
					handleSocketStream, 
					static_cast<void*>(this));
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
	}
	
	void Socket::sendCommand(const Command& cmd) const
	{
		const size_t size = cmd.length();
		uint8_t *buf = new uint8_t[size];
		cmd.serialization(buf,size);
		write(m_socketfd,buf,size);
		delete [] buf;
	}

	void* handleSocketStream(void* param)
	{
		fd_set rfds;
		struct timeval tv = {0,0};

		const Socket& self = *static_cast<Socket*>(param);
		static const size_t buf_size = 512;
		uint8_t buf[buf_size];
		do {
			FD_ZERO(&rfds);
			FD_SET(self.m_socketfd, &rfds);

			tv.tv_sec = 0;
			tv.tv_usec = 100;
			const int selectRes = select(self.m_socketfd+1, &rfds, NULL, NULL, &tv);
			if (selectRes < 0) {
				exit(-1);
			} else if (0==selectRes) {
				sched_yield();
				continue;
			}
			bzero(buf,buf_size*sizeof(uint8_t));
			uint8_t *buffer = buf;
			ssize_t rsize = buf_size;
			ssize_t size = 0;
			do {
				ssize_t trsize = rsize;
				size = ::read(self.m_socketfd,buffer,rsize);
				buffer += size;
				rsize -= size;
				__IF_DO(trsize>=size,break;);
			} while (1);

			size = buf_size - rsize;	
			if (size > 0) {
				//__LOG("%s%s %s\n",KYEL,"dispatch msg",__func__);
				str_t msg((char*)buf,size);
				slist_t array = ::splitString(msg);
				for (const str_t& mesg : array){
					Message *msg = Message::parseMessage(mesg);
					if (NULL==msg) continue;
					self.m_commandHandler(*msg,static_cast<Socket*>(param));
					delete msg;
				}
			}	
		} while (1);	

		return NULL;
	}

}
