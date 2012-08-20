#ifndef __LIBJPUDPSOCKET_HPP__
#define __LIBJPUDPSOCKET_HPP__

#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <libJPLogger.hpp>

#include <JPSocket.hpp>

namespace jpCppLibs{

	class JPUdpSocket: public JPSocket{
		public:
		/**
		 * Class constructor
		 *
		 * @param log Pointer to a logger
		 * @param address String with the ip address
		 * @param port Integer with the number of the port
		 */
		JPUdpSocket(Logger * log, std::string * address, int port);
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
		/**
		 * Bind an the address with the socket
		 * @return Integer 0 in case of success
		 */
		int bind();
		/**
		 * Bind an the address with the socket
		 * @param address String with the ip address
		 * @param port Integer with the number of the port
		 * @return Integer 0 in case of success
		 */
		int bind(std::string * address, int port);
		/**
		 * Bind an the address with the socket
		 * @param address String with the ip address
		 * @param port Integer with the number of the port
		 * @return Integer 0 in case of success
		 */
		int send( std::string * msg );
		/**
		 * Read a message from the socket
		 * @param strsize	Size of the message that want to be readed
		 * @param msg		String where the message will be placed
		 * @return Integer 0 in case of success
		 */
		int receive(int strsize, std::string **msg);
		private:
		/**
		 * Ip address
		 */
		JPIpAddress * foreignAddress;
	};


};
#endif
