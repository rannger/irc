#ifndef __IRC_TYPES_H_
#define __IRC_TYPES_H_ 
#pragma once
#include <map>
#include <vector>
#include <string>
#include <stdint.h>

namespace rirc {
	class Command;
	class Socket;
	class Message;
}

typedef std::string str_t;
typedef std::map<str_t,str_t> dict_t;
typedef std::vector<str_t> slist_t;

typedef void (*command_handler_t)(const rirc::Message&,rirc::Socket*);

#endif