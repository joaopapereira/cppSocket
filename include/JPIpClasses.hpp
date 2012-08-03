#ifndef __JPIPCLASSES_HPP__
#define __JPIPCLASSES_HPP__

#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <extlibs/libJPLogger.h>
#include <JPSockExceptions.hpp>

namespace cppLibs{
	class JPIpAddress{
	public:
		/**
		 * Class constructor
		 */
		JPIpAddress();
		/**
		 * Class Copy constructor
		 */
		JPIpAddress(const JPIpAddress& copyFrom);
		/**
		 * Set Ip address of the socket
		 * @param addr IP Address of the socket
		 * @return Integer 0 in case of error
		 */
		int setIp( std::string ip );
		/**
		 * Set Ip address of the socket
		 * @param addr IP Address of the socket
		 * @return Integer 0 in case of error
		 */
		int setIp( struct sockaddr address, socklen_t size );
		/**
		 * function to retrieve
		 * the IP
		 * @return Ip address in readable format
		 */
		virtual std::string getCharIp() = 0;
		/**
		 * function to retrieve
		 * the IP
		 * @return Socket address structure
		 */
		const struct sockaddr* getIp();
		/**
		 * Check IP version
		 * @param ip String with the IP
		 * @return Version
		 */
		static int checkIpVersion( std::string ip );
	protected:
		/**
		 * function to retrieve
		 * the IP
		 * @return Socket address structure
		 */
		void int_getIp(char * result,int family);
		sockaddr_storage address;
	};

	class JPSocketIPv6: public JPIpAddress{
	public:
			/**
			 * Class constructor
			 */
			JPSocketIPv6();
			/**
			 * Class constructor
			 */
			~JPSocketIPv6();

			/**
			 * Pure virtual function to retrieve
			 * the IP
			 * @return Socket address structure
			 */
			virtual std::string getCharIp();


	};
	class JPSocketIPv4: public JPIpAddress{
	public:
			/**
			 * Class constructor
			 */
			JPSocketIPv4();
			/**
			 * Class constructor
			 */
			~JPSocketIPv4();

			/**
			 * Pure virtual function to retrieve
			 * the IP
			 * @return Socket address structure
			 */
			virtual std::string getCharIp();

	};


};
#endif
