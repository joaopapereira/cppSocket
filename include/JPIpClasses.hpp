#ifndef __JPIPCLASSES_HPP__
#define __JPIPCLASSES_HPP__

#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <extlibs/libJPLogger.h>

namespace cppLibs{
	class JPIpAddress{
	public:
		/**
		 * Class constructor
		 */
		JPIpAddress();
		/**
		 * Set Ip address of the socket
		 * @param addr IP Address of the socket
		 * @return Integer 0 in case of error
		 */
		virtual int setIp( std::string ip ) = 0;
		/**
		 * function to retrieve
		 * the IP
		 * @return Socket address structure
		 */
		virtual std::string getIp() = 0;
		/**
		 * Check IP version
		 * @param ip String with the IP
		 * @return Version
		 */
		static int checkIpVersion( std::string ip );
	protected:
		/**
		 * Set Ip address of the socket
		 * @param addr IP Address of the socket
		 * @return Integer 0 in case of error
		 */
		int int_setIp( std::string ip );
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
			 * Set Ip address of the socket
			 * @param addr IPv6 Address of the socket
			 * @return Integer 0 in case of error
			 */
			int setIp( std::string ip );

			/**
			 * Pure virtual function to retrieve
			 * the IP
			 * @return Socket address structure
			 */
			virtual std::string getIp();


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
			 * Set Ip address of the socket
			 * @param addr IPv6 Address of the socket
			 * @return Integer 0 in case of error
			 */
			int setIp( std::string ip );

			/**
			 * Pure virtual function to retrieve
			 * the IP
			 * @return Socket address structure
			 */
			std::string getIp();

	};


};
#endif
