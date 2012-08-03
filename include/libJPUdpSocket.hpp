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
		 * Class constructor
		 *
		 * @param log Pointer to a logger
		 * @param address String with the ip address
		 * @param port Integer with the number of the port
		 */
		JPUdpSocket(Logger * log, std::string address, int port);
		/**
		 * Empty constructor
		 * @param log Pointer to a logger
		 */
		JPUdpSocket(Logger * log);
		/**
		 * Copy class constructor
		 * This constructor should be used if socket is already
		 * created
		 * @param copyFrom Object from wish we want to copy
		 */
		JPUdpSocket( const JPUdpSocket& copyFrom );
		/**
		 * Constructor
		 */
		~JPUdpSocket();
		/**
		 * Creates the socket
		 * @return Integer 0 in case of success
		 */
		virtual int create();
	};


};
#endif
