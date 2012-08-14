
#include "libJPUdpSocket.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>

#include <JPSocket.hpp>

using namespace cppLibs;

#define BUFFER_READ 50



/**
 * CLASS: SocketExceptions
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
 *
 * @param log Pointer to a logger
 * @param address String with the ip address
 * @param port Integer with the number of the port
 */
JPUdpSocket::JPUdpSocket(Logger * log, std::string* address, int port)
	: JPSocket(log, address, port){
	logger->log(JPSocket::moduleName,M_LOG_NRM,M_LOG_TRC,"JPUdpSocket(%s,%d)", address->c_str(), port );
	foreignAddress = NULL;
}
/**
 * Empty constructor
 * @param log Pointer to a logger
 */
JPUdpSocket::JPUdpSocket(Logger * log )
	:JPSocket( log ){
	logger->log(JPSocket::moduleName,M_LOG_NRM,M_LOG_TRC,"JPUdpSocket()");
	foreignAddress = NULL;
}
/**
 * Copy class constructor
 * This constructor should be used if socket is already
 * created
 * @param copyFrom Object from wish we want to copy
 */
JPUdpSocket::JPUdpSocket( const JPUdpSocket& copyFrom )
	:JPSocket(copyFrom){
	*foreignAddress = *(copyFrom.foreignAddress);

}
/**
 * Class destructor
 */
JPUdpSocket::~JPUdpSocket(){
}
/**
 * Creates the socket
 * @return Integer 0 in case of success
 */
int
JPUdpSocket::create(){
	logger->log(JPSocket::moduleName,M_LOG_NRM,M_LOG_TRC,"JPUdpSocket::create()");
	return create_int(SOCK_DGRAM, IPPROTO_UDP );
}
/**
 * Bind an the address with the socket
 * @return Integer 0 in case of success
 */
int
JPUdpSocket::bind(){
	int result = JPSocket::bind();
	if( 0 == result )
		connStab = 1;
	return result;
}
/**
 * Bind an the address with the socket
 * @param address String with the ip address
 * @param port Integer with the number of the port
 * @return Integer 0 in case of success
 */
int
JPUdpSocket::bind(std::string * address, int port){
	int result = JPSocket::bind(address,port);
	if( 0 == result )
		connStab = 1;
	return result;
}
/**
 * Bind an the address with the socket
 * @param address String with the ip address
 * @param port Integer with the number of the port
 * @return Integer 0 in case of success
 */
int
JPUdpSocket::send( std::string * msg ){
	connStab = 1;
	if( NULL == foreignAddress )
		return this->sendTo(msg,this->address);
	else
		return this->sendTo(msg,foreignAddress);
}
/**
 * Read a message from the socket
 * @param strsize	Size of the message that want to be readed
 * @param msg		String where the message will be placed
 * @return Integer 0 in case of success
 */
int
JPUdpSocket::receive(int strsize, std::string **msg){
	connStab = 1;
	return this->receiveFrom(strsize,msg,&foreignAddress);
}

/**
 * END }
 */
