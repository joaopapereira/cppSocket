
//#include "libJPSocket.hpp"
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <JPIpClasses.hpp>

using namespace cppLibs;

#define BUFFER_READ 50


/**
 * CLASS: JPSocketIPv6
 * BEGIN {
 */
/**
 * Private methods
 */
/**
 * Protected methods
 */
/**
 * Set Ip address of the socket
 * @param addr IP Address of the socket
 * @return Integer 0 in case of error
 */
int
JPIpAddress::setIp( std::string ip ){
	struct addrinfo hint, *res = NULL;
	int ret;

	memset(&hint, '\0', sizeof hint);

	hint.ai_family = PF_UNSPEC;

	ret = getaddrinfo(ip.c_str(), NULL, &hint, &res);
	memcpy(&address, res->ai_addr, sizeof(address));
	return 0;
}
/**
 * function to retrieve
 * the IP
 * @return Socket address structure
 */
void
JPIpAddress::int_getIp(char * result, int family){
	inet_ntop(family,& address, result,
			  sizeof(result));
}

/**
 * function to retrieve
 * the IP
 * @return Socket address structure
 */
const struct sockaddr*
JPIpAddress::getIp(){
	return (const struct sockaddr*)&address;
}
/**
 * Class constructor
 */
JPIpAddress::JPIpAddress(){
	memset(&address, 0, sizeof(address));
}
/**
 * Class constructor
 */
JPIpAddress::JPIpAddress(const JPIpAddress& copyFrom){
	address = copyFrom.address;
}
/**
 * Check IP version
 * @param ip String with the IP
 * @return Version
 */
int
JPIpAddress::checkIpVersion( std::string ip ){
	struct addrinfo hint, *res = NULL;
	int ret;

	memset(&hint, '\0', sizeof hint);

	hint.ai_family = PF_UNSPEC;

	ret = getaddrinfo(ip.c_str(), NULL, &hint, &res);
	if (ret) {
		std::string err("Invalid address![");
		err.append(gai_strerror(ret));
		err.append("]");
		throw SocketExceptions(err);
	}
	if(res->ai_family == AF_INET) {
		ret = AF_INET;
	} else if (res->ai_family == AF_INET6) {
		ret = AF_INET6;
	} else {
		throw SocketExceptions("Unknown address format");
	}

   freeaddrinfo(res);
   return ret;
}
/**
 * } End
 */

/**
 * CLASS: JPSocketIPv6
 * BEGIN {
 */
/**
 * Private methods
 */
/**
 * Protected methods
 */
/**
 * Class constructor
 */
JPSocketIPv6::JPSocketIPv6()
:JPIpAddress(){

}
/**
 * Class constructor
 */
JPSocketIPv6::~JPSocketIPv6(){

}

/**
 * Pure virtual function to retrieve
 * the IP
 * @return Socket address structure
 */
std::string
JPSocketIPv6::getCharIp(){
	char straddr[INET6_ADDRSTRLEN];
	int_getIp(straddr,AF_INET6);
	return std::string(straddr);
}
/**
 * Public methods
 */

/**
 * END }
 */



/**
 * CLASS: JPSocketIPv4
 * BEGIN {
 */
/**
 * Private methods
 */
/**
 * Protected methods
 */
/**
 * Public methods
 */
/**
 * Class constructor
 */
JPSocketIPv4::JPSocketIPv4()
:JPIpAddress(){
}
/**
 * Class constructor
 */
JPSocketIPv4::~JPSocketIPv4(){

}
/**
 * Pure virtual function to retrieve
 * the IP
 * @return Socket address structure
 */
std::string
JPSocketIPv4::getCharIp(){
	char straddr[INET_ADDRSTRLEN];
	int_getIp(straddr,AF_INET);
	return std::string(straddr);
}


