#ifndef __LIBJPUDPSOCKET_HPP__
#define __LIBJPUDPSOCKET_HPP__

#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <extlibs/libJPLogger.h>

#include <JPSocket.hpp>

namespace cppLibs{

	class JPUdpSocket: public JPSocket{
		public:
		/**
		 * Constructor
		 */
		JPUdpSocket();
		/**
		 * Constructor
		 */
		~JPUdpSocket();
	};


};
#endif
