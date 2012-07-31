#ifndef __MSOCKET_HPP__
#define __MSOCKET_HPP__

#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <extlibs/libJPLogger.h>
namespace cppLibs{
	/**
	 * Class the implements the exceptions of the logger
	 */
	class SocketExceptions: public std::exception{
	public:
		/**
		 * Constructor used when no message needs to be passed
		 * @param showErrno Indicates if the ERRNO should be or not written
		 */
		SocketExceptions(bool showErrno = false) throw();
		/**
		 * Constructor used when no message needs to be passed
		 * @param error Error message
		 * @param showErrno Indicates if the ERRNO should be or not written
		 */
		SocketExceptions( char* error,bool showErrno = false ) throw();
		/**
		 * Constructor used when no message needs to be passed
		 * @param error Error message
		 * @param showErrno Indicates if the ERRNO should be or not written
		 */
		SocketExceptions(std::string error, bool showErrno = false) throw();
		/**
		 * Function to change error message
		 * @param error Error message
		 */
		void setMsg(const char * error) throw();
		/**
		 * Retrive error reason
		 * @return String with the error
		 */
		const char * what() const throw();
		/**
		 * Class destructor
		 */
		~SocketExceptions() throw();
	private:
		/**
		 * Error Message
		 */
		std::string myerror;
		/**
		 * Indicates if errno should be shown or not
		 */
		bool showErrno;
	};


	class JPSocketIPv6{
		protected:
			/**
			 * Class constructor
			 */
			JPSocketIPv6();
			/**
			 * Class constructor
			 */
			~JPSocketIPv6();

			/**
			 * Set Ip address of the socket
			 * @param addr IPv6 Address of the socket
			 * @return Integer 0 in case of error
			 */
			int setIp( struct sockaddr_in6 addr );

			/**
			 * Pure virtual function to retrieve
			 * the IP
			 * @return Socket address structure
			 */
			virtual std::string getIp() = 0;

			/**
			 * IPv6 address of the socket
			 */
			struct sockaddr_in6 ipv6Address;

	};
	class JPSocketIPv4{
		protected:
			/**
			 * Class constructor
			 */
			JPSocketIPv4();
			/**
			 * Class constructor
			 */
			~JPSocketIPv4();
			/**
			 * Set Ip address of the socket
			 * @param addr IPv6 Address of the socket
			 * @return Integer 0 in case of error
			 */
			int setIp( struct sockaddr_in addr );

			/**
			 * Pure virtual function to retrieve
			 * the IP
			 * @return Socket address structure
			 */
			virtual std::string getIp() = 0;

			/**
			 * IPv4 address of the socket
			 */
			struct sockaddr_in ipv4Address;

	};
	#define M_DISC_GRACE 0
	#define M_DISC_FORCE 1
	
	class JPSocket: public JPSocketIPv6,public JPSocketIPv4{
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
			 * Send a message to the socket
			 * @param msg Message to be sent through the socket
			 * @return Integer 0 in case of success
			 */
			int send( std::string * msg );
			/**
			 *
			 */
			int setSoc( int sockId );
			/**
			 * Read a message from the socket
			 * @param strsize	Size of the message that want to be readed
			 * @param msg		String where the message will be placed
			 * @return Integer 0 in case of success
			 */
			int receive(int strsize, std::string **msg);

			/**
			 * End the socket connectiorn
			 * @param type Type of connection end
			 * 				M_DISC_GRACE If want to wait for acknowledge
			 * 				M_DISC_FORCE If want a force end
			 * @return Integer 0 in case of success
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
			 * @return Integer 0 in case of success
			 */
			int receive_int(int strsize, std::string **msg);
			/**
			 *
			 */
			int setSoc_int( int sockId );

			/**
			 * Pointer to a logger
			 */
			Logger * logger;

			/**
			 * Check IP version
			 * @param ip String with the IP
			 * @return Version
			 */
			int checkIpVersion( std::string ip );
	
	};

	class JPTcpSocket: public JPSocket{
		public:
		/**
		 * Constructor
		 */
		JPTcpSocket();
		/**
		 * Constructor
		 */
		~JPTcpSocket();
	};

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


	class JPNonBlockSocket: public JPTcpSocket{
		public: 
			/**
			 * Class constructor
			 *
			 * @param log Pointer to a logger
			 * @param address String with the ip address
			 * @param port Integer with the number of the port
			 */
			JPNonBlockSocket(Logger * log, std::string address, int port);
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
			int setSoc( int sockId );
		private:
			fd_set read_flags,write_flags;
			/**
			 * Select on the socket
			 * @param waitd Time structure to set the wait
			 *               time on the socket
			 */
			int select( struct timeval * waitd );
			/**
			 *
			 */
			int nonblock();
	};



};
#endif
