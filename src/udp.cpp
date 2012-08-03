
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
JPUdpSocket::JPUdpSocket(Logger * log, std::string address, int port)
	: JPSocket(log, address, port){
	logger->log(JPSocket::moduleName,M_LOG_NRM,M_LOG_TRC,"JPTcpSocket(%s,%d)", address.c_str(), port );
}
/**
 * Empty constructor
 * @param log Pointer to a logger
 */
JPUdpSocket::JPUdpSocket(Logger * log )
	:JPSocket( log ){
	logger->log(JPSocket::moduleName,M_LOG_NRM,M_LOG_TRC,"JPTcpSocket()");
}
/**
 * Copy class constructor
 * This constructor should be used if socket is already
 * created
 * @param copyFrom Object from wish we want to copy
 */
JPUdpSocket::JPUdpSocket( const JPUdpSocket& copyFrom )
	:JPSocket(copyFrom){

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
	logger->log(JPSocket::moduleName,M_LOG_NRM,M_LOG_TRC,"JPTcpSocket::create()");
	return create_int(SOCK_STREAM, IPPROTO_TCP );
}
/**
 * END }
 */
