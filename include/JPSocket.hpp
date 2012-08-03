#ifndef __JPSOCKET_HPP__
#define __JPSOCKET_HPP__

#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <extlibs/libJPLogger.h>

#include <JPIpClasses.hpp>

namespace cppLibs{

	#define M_DISC_GRACE 0
	#define M_DISC_FORCE 1
	
	class JPSocket{
		public:
			/**
			 * Class constructor
			 *
			 * @param log Pointer to a logger
			 * @param address String with the ip address
			 * @param port Integer with the number of the port
			 */
			JPSocket( Logger * log , std::string address, int port );
			/**
			 * Empty constructor
			 * @param log Pointer to a logger
			 */
			JPSocket( Logger * log );
			/**
			 * Copy class constructor
			 * This constructor should be used if socket is already
			 * created
			 * @param copyFrom Object from wish we want to copy
			 */
			JPSocket( const JPSocket& copyFrom );
			/**
			 * Class destructor
			 */
			~JPSocket();
			/**
			 * Creates the socket
			 * @return Integer 0 in case of success
			 */
			virtual int create() = 0;

			/**
			 * Send a message to the socket
			 * @param msg Message to be sent through the socket
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
			/**
			 * Connects the socket to the address defined
			 * @return Integer 0 in case of success
			 */
			int connect();
			/**
			 * Connects the socket to the address defined
			 * @param address String with the ip address
			 * @param port Integer with the number of the port
			 * @return Integer 0 in case of success
			 */
			int connect(std::string address, int port);
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
			int bind(std::string address, int port);
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
			/**
			 * End the socket connectiorn
			 * @param type Type of connection end
			 * 				M_DISC_GRACE If want to wait for acknowledge
			 * 				M_DISC_FORCE If want a force end
			 * @return Integer
			 *                 >0 Length read
			 */
			int disconnect( int type = M_DISC_GRACE );

			/**
			 * Get the Ip where the socket is binded
			 * or the Ip where the socket is connected to
			 * depending if this socket is being used as a
			 * server or a client
			 * @return String with the IP address
			 */
			std::string getIp();
			/**
			 * Set the address to be used
			 * @param address String with the ip address
			 * @param port Integer with the number of the port
			 * @return Integer 0 in case of success
			 */
			int setAddress( std::string address, int port );
	
		protected:
			static const std::string moduleName;
			/**
			 * File descriptor of the socket
			 */
			int socketfd;
			/**
			 * Address version can have 2 values:
			 * AF_INET  if ipv4
			 * AF_INTE6 if ipv6
			 */
			int addressVersion;
			/**
			 * Port used on this socket
			 */
			int port;

			/**
			 * Address version can have 5 values:
			 * SOCK_STREAM     Provides sequenced, reliable, two-way, connection-based byte streams.  An out-of-band data transmission mechanism may be supported.
			 *
       	   	 * SOCK_DGRAM      Supports datagrams (connectionless, unreliable messages of a fixed maximum length).
       	   	 * SOCK_SEQPACKET  Provides a sequenced, reliable, two-way connection-based data transmission path for datagrams of fixed maximum length; a consumer is required to read  an
             *                 entire packet with each input system call.
       	   	 * SOCK_RAW        Provides raw network protocol access.
       	   	 * SOCK_RDM	       Provides a reliable datagram layer that does not guarantee ordering.
			 */
			int typeOfSocket;
			/**
			 * Protocol used on the socket
			 */
			int protocol;
			/**
			 * Pointer to a logger
			 */
			Logger * logger;
			/**
			 * Ip address
			 */
			JPIpAddress * address;
			/**
			 * Handle the address
			 * @param address String with the ip address
			 * @param port Integer with the number of the port
			 * @return Integer 0 in case of success
			 */
			int handleAddress( std::string address, int port );

			/**
			 * Internal function used to write a message into
			 * the socket
			 * @param msg Message to be sent through the socket
			 * @return Integer 0 in case of success
			 */
			int send_int( std::string * msg );
			/**
			 * Internal function used to read a message from the socket
			 * @param strsize	Size of the message that want to be readed
			 * @param msg		String where the message will be placed
			 * @return Integer >0 Length read
			 */
			int receive_int(int strsize, std::string **msg);
			/**
			 * Creates the socket
			 * @param type Specifies the communication semantics
			 * @param protocol Specifies a particular protocol to be used with the socket
			 */
			int create_int(int type, int protocol);

	
	};

};
#endif
