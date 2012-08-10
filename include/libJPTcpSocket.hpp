#ifndef __LIBJPTCPSOCKET_HPP__
#define __LIBJPTCPSOCKET_HPP__

#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <extlibs/libJPLogger.h>

#include <libJPSocket.hpp>

namespace cppLibs{
	
	class JPTcpSocket: public JPSocket{
		public:
		/**
		 * Class constructor
		 *
		 * @param log Pointer to a logger
		 * @param address String with the ip address
		 * @param port Integer with the number of the port
		 */
		JPTcpSocket(Logger * log, std::string * address, int port);
		/**
		 * Empty constructor
		 * @param log Pointer to a logger
		 */
		JPTcpSocket(Logger * log);
		/**
		 * Copy class constructor
		 * This constructor should be used if socket is already
		 * created
		 * @param copyFrom Object from wish we want to copy
		 */
		JPTcpSocket( const JPTcpSocket& copyFrom );
		/**
		 * Constructor
		 */
		~JPTcpSocket();
		/**
		 * Creates the socket
		 * @return Integer 0 in case of success
		 */
		virtual int create();
		/**
		 * Listen on port IP
		 * @return Integer 0 in case of success
		 */
		int listen();
		/**
		 * Listen on port IP
		 * @return Integer 0 in case of success
		 */
		 int accept(JPSocket*);

	};

	class JPNonBlockSocket: public JPTcpSocket{
		public: 
			/**
			 * Class constructor
			 *
			 * @param log Pointer to a logger
			 * @param address String with the ip address
			 * @param port Integer with the number of the port
			 */
			JPNonBlockSocket(Logger * log, std::string * address, int port);
			/**
			 * Empty constructor
			 * @param log Pointer to a logger
			 */
			JPNonBlockSocket(Logger * log);
			/**
			 * Copy class constructor
			 * This constructor should be used if socket is already
			 * created
			 * @param copyFrom Object from wish we want to copy
			 */
			JPNonBlockSocket( const JPNonBlockSocket& copyFrom );
			/**
			 * Class destructor
			 */
			~JPNonBlockSocket();
			/**
			 * See if the socket as anything to read
			 * @param waitd Time structure to set the wait
			 *               time on the socket
			 * @return Integer 0 in case of success
			 */
			int selectRead( struct timeval * waitd );
			/**
			 * See if the socket as anything to write
			 * @param waitd Time structure to set the wait
			 *               time on the socket
			 * @return Integer 0 in case of success
			 */
			int selectWrite( struct timeval * waitd );
			/**
			 * Overload function from JPSocket to call
			 * the selectRead
			 * @param strsize	Size of the message that want to be readed
			 * @param msg		String where the message will be placed
			 * @return Integer 0 in case of success
			 */
			int receive(int strsize, std::string **msg);
			/**
			 * Overload function from JPSocket to call
			 * the selectWrite
			 * @param msg Message to be sent through the socket
			 * @return Integer 0 in case of success
			 */
			int send( std::string * msg );
		private:
			fd_set read_flags,write_flags;
			/**
			 * Select on the socket
			 * @param waitd Time structure to set the wait
			 *               time on the socket
			 */
			int select( struct timeval * waitd );
			/**
			 * Change a socket to non blocking
			 * @return Integer 0 in case of success
			 */
			int nonblock();
	};



};
#endif
