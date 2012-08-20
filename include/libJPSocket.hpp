#ifndef __LIBJPSOCKET_HPP__
#define __LIBJPSOCKET_HPP__

#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <libJPLogger.hpp>
#include <sstream>

#include <JPSockExceptions.hpp>

#include <JPIpClasses.hpp>

#include <JPSocket.hpp>

namespace jpCppLibs{
	template <class T>
	inline std::string to_string (const T& t)
	{
		std::stringstream ss;
		ss << t;
		return ss.str();
	}

	#define M_DISC_GRACE 0
	#define M_DISC_FORCE 1
	

};
#endif
