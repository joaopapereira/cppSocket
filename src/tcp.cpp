
#include "libJPTcpSocket.hpp"
#include <unistd.h>
#include <fcntl.h>

using namespace cppLibs;

#define BUFFER_READ 50



/**
 * CLASS: JPTcpSocket
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
 * Constructor
 */
JPTcpSocket::JPTcpSocket(){

}
/**
 * Constructor
 */
JPTcpSocket::~JPTcpSocket(){

}
/**
 * END }
 */




/**
 * CLASS: JPNonBlockSocket
 * BEGIN {
 */
/**
 * Private methods
 */
/**
 * Protected methods
 */
/**
 * Select on the socket
 * @param waitd Time structure to set the wait
 *               time on the socket
 */
int JPNonBlockSocket::select(  struct timeval * waitd ){
	return ::select( socketfd+1, &read_flags,&write_flags, (fd_set*)0,waitd);
}
/**
 * Change a socket to non blocking
 * @return Integer 0 in case of success
 */
int
JPNonBlockSocket::nonblock(){
	int opts;

	opts = fcntl(socketfd,F_GETFL);
	if (opts < 0) {
		perror("fcntl(F_GETFL)");
		exit(EXIT_FAILURE);
	}
	opts = (opts | O_NONBLOCK);
	if (fcntl(socketfd,F_SETFL,opts) < 0) {
		perror("fcntl(F_SETFL)");
		exit(EXIT_FAILURE);
	}
}
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
JPNonBlockSocket::JPNonBlockSocket(Logger * log, std::string address, int port)
	: JPSocket(log, address, port){
	nonblock();
}
/**
 * Empty constructor
 * @param log Pointer to a logger
 */
JPNonBlockSocket::JPNonBlockSocket(Logger * log )
	:JPSocket( log ){
}
/**
 * Copy class constructor
 * This constructor should be used if socket is already
 * created
 * @param copyFrom Object from wish we want to copy
 */
JPNonBlockSocket::JPNonBlockSocket( const JPNonBlockSocket& copyFrom ){

}
/**
 * Class destructor
 */
JPNonBlockSocket::~JPNonBlockSocket(){

}
/**
 * See if the socket as anything to read
 * @param waitd Time structure to set the wait
 *               time on the socket
 * @return Integer 0 in case of success
 */
int JPNonBlockSocket::selectRead(  struct timeval * waitd  ){
	logger->log(JPSocket::moduleName,M_LOG_NRM,M_LOG_TRC,"selectRead(%p) on socket[%d]", waitd, socketfd);
	int res;
	if( -1 == socketfd ){
		logger->log(JPSocket::moduleName,M_LOG_HGH,M_LOG_WRN,"Function to create socket wasn't called");
		throw JPNoSocketCreated();
	}
	FD_ZERO(&read_flags);
	FD_ZERO(&write_flags);
	FD_SET(socketfd, &read_flags);

//	res = select( waitd );
	res = ::select( socketfd+1, &read_flags,NULL,NULL,waitd);

	std::string err("Error while doing selectRead on socket[");
	if( res <= 0 ){
		err.append(to_string(socketfd));
		err.append("]");
		return JPGenericSocket("Error while doing selectRead on socket");
	}else if( FD_ISSET(socketfd , &read_flags )  ){
		return marta::m_success;
	}
	return marta::m_sockError;
}
/**
 * See if the socket as anything to write
 * @param waitd Time structure to set the wait
 *               time on the socket
 * @return Integer 0 in case of success
 */
int JPNonBlockSocket::selectWrite(  struct timeval * waitd ){
	logger->log(JPSocket::moduleName,M_LOG_NRM,M_LOG_TRC,"selectWrite(%p) on socket[%d]", waitd, socketfd);
	int res;
	if( -1 == socketfd )
		return marta::m_sockError;
	FD_ZERO(&write_flags);
	FD_ZERO(&read_flags);
	FD_SET(socketfd, &write_flags);

	res = select( waitd );

	if( res < 0 ){
		return marta::m_sockError;
	}else if( FD_ISSET(socketfd , &write_flags )  ){
		return marta::m_success;
	}
	return marta::m_sockError;
}

/**
 * Overload function from JPSocket to call
 * the selectRead
 * @param strsize	Size of the message that want to be readed
 * @param msg		String where the message will be placed
 * @return Integer 0 in case of success
 */
int 
JPNonBlockSocket::receive(int strsize, std::string **msg){
	if( NULL != *msg )
		logger->log(JPSocket::moduleName,M_LOG_LOW,M_LOG_TRC,"JPNonBlockSocket::receive(%d,%s)",strsize,(*msg)->c_str());
	else
		logger->log(JPSocket::moduleName,M_LOG_LOW,M_LOG_TRC,"JPNonBlockSocket::receive(%d,NULL)",strsize);
	struct timeval tm;
	int res;
	tm.tv_sec = 0;
	tm.tv_usec = 1;
	if(marta::m_success == selectRead( &tm ) ){
		try{
			res = receive_int( strsize , msg );
		}catch( MExpSocketEmpty &e ){
			FD_CLR(socketfd,&read_flags);
			return -1;
		}
		FD_CLR(socketfd,&read_flags);
	}else
		res = -1;
	return res;
}
/**
 * Overload function from JPSocket to call
 * the selectWrite
 * @param msg Message to be sent through the socket
 * @return Integer 0 in case of success
 */
int 
JPNonBlockSocket::send( std::string * msg ){
	logger->log(JPSocket::moduleName,M_LOG_LOW,M_LOG_TRC,"JPNonBlockSocket::send(%s)",msg->c_str());
	struct timeval tm;
	int res;
	tm.tv_sec = 0;
	tm.tv_usec = 1;
	if(marta::m_success == selectWrite( &tm ) )
		res = send_int( msg );
	FD_CLR(socketfd,&write_flags);
	FD_CLR(socketfd,&read_flags);
	return res;
}


int
JPNonBlockSocket::setSoc( int sockId ){
	int res = setSoc( sockId );
	nonblock();
}

/**
 * END }
 */
