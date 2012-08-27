
#include "libJPTcpSocket.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>

#include <JPSocket.hpp>

using namespace jpCppLibs;

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
 * Class constructor
 *
 * @param log Pointer to a logger
 * @param address String with the ip address
 * @param port Integer with the number of the port
 */
JPTcpSocket::JPTcpSocket(Logger * log, std::string * address, int port)
	: JPSocket(log, address, port){
	logger->log(JPSocket::moduleName,M_LOG_NRM,M_LOG_TRC,"JPTcpSocket(%s,%d)", address->c_str(), port );
}
/**
 * Empty constructor
 * @param log Pointer to a logger
 */
JPTcpSocket::JPTcpSocket(Logger * log )
	:JPSocket( log ){
	logger->log(JPSocket::moduleName,M_LOG_NRM,M_LOG_TRC,"JPTcpSocket()");
}
/**
 * Copy class constructor
 * This constructor should be used if socket is already
 * created
 * @param copyFrom Object from wish we want to copy
 */
JPTcpSocket::JPTcpSocket( const JPTcpSocket& copyFrom )
	:JPSocket(copyFrom){

}
/**
 * Class destructor
 */
JPTcpSocket::~JPTcpSocket(){
}
/**
 * Creates the socket
 * @return Integer 0 in case of success
 */
int
JPTcpSocket::create(){
	logger->log(JPSocket::moduleName,M_LOG_NRM,M_LOG_TRC,"JPTcpSocket::create()");
	return create_int(SOCK_STREAM, IPPROTO_TCP );
}
/**
 * Listen on port IP
 * @return Integer 0 in case of success
 */
int
JPTcpSocket::listen(){
	logger->log(JPSocket::moduleName,M_LOG_LOW,M_LOG_TRC,"JPSocket::listen()");
	if( -1 == socketfd )
		throw JPInvSocket();
	if( 0 > ::listen(socketfd,5) )
		throw JPGenericSocket("Error putting socket to listening mode!",true);

	connStab = 1;

}
/**
 * Listen on port IP
 * @return Integer 0 in case of success
 */
int
JPTcpSocket::accept(JPSocket* socket){
	logger->log(JPSocket::moduleName,M_LOG_LOW,M_LOG_TRC,"accept(%p)",socket);
	int newsockfd,oldsocket;
	struct sockaddr_storage cliAddr;
	socklen_t cliLen = sizeof(cliAddr);
	newsockfd = ::accept(socketfd,
		(struct sockaddr *) &cliAddr,
		&cliLen);
	if (newsockfd < 0)
		throw SocketExceptions("Error accepting new connection!",true);

	logger->log(moduleName,M_LOG_LOW,M_LOG_TRC,"newsocecktid=(%d),listen on=%d",newsockfd,socketfd);
	//JPIpAddress * addr = new JPIpAddress();
	//TODO: This part need to be changed
	oldsocket = socketfd;
	socketfd = newsockfd;
	*socket = *this;
	socketfd = oldsocket;
	socket->setAddress(cliAddr, cliLen);
	logger->log(moduleName,M_LOG_LOW,M_LOG_DBG,"Accepted a connection from[%s]",socket->getIp().c_str());




	return 0;
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
	if (opts < 0)
		throw JPGenericSocket("Error getting flags on socket");
	opts = (opts | O_NONBLOCK);
	if (fcntl(socketfd,F_SETFL,opts) < 0)
		throw JPGenericSocket("Error setting flags on socket");
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
JPNonBlockSocket::JPNonBlockSocket(Logger * log, std::string * address, int port)
	: JPTcpSocket(log, address, port){
	nonblock();
}
/**
 * Empty constructor
 * @param log Pointer to a logger
 */
JPNonBlockSocket::JPNonBlockSocket(Logger * log )
	:JPTcpSocket( log ){
}
/**
 * Copy class constructor
 * This constructor should be used if socket is already
 * created
 * @param copyFrom Object from wish we want to copy
 */
JPNonBlockSocket::JPNonBlockSocket( const JPNonBlockSocket& copyFrom )
	:JPTcpSocket(copyFrom){

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

	res = ::select( socketfd+1, &read_flags,NULL,NULL,waitd);

	std::string err("Error while doing selectRead on socket[");
	err.append(to_string(socketfd));
	err.append("]");
	if( res < 0 ){
		throw JPGenericSocket(err,true);
	}else if( FD_ISSET(socketfd , &read_flags )  ){
		return 0;
	}
	throw JPErrEmptySocket();
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
	if( -1 == socketfd ){
		logger->log(JPSocket::moduleName,M_LOG_HGH,M_LOG_WRN,"Function to create socket wasn't called");
		throw JPNoSocketCreated();
	}
	FD_ZERO(&write_flags);
	FD_ZERO(&read_flags);
	FD_SET(socketfd, &write_flags);

	res = select( waitd );

	std::string err("Error while doing selectRead on socket[");
	err.append(to_string(socketfd));
	err.append("]");
	if( res < 0 ){
		throw JPGenericSocket(err);
	}else if( FD_ISSET(socketfd , &write_flags )  ){
		return 0;
	}
	throw JPErrEmptySocket();
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
	try{
		selectRead( &tm );
		res = receive_int( strsize , msg );
	}catch( JPErrEmptySocket &e ){
		FD_CLR(socketfd,&read_flags);
		throw e;
	}catch(JPGenericSocket &e ){
		throw e;
	}
	FD_CLR(socketfd,&read_flags);

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
	try{
		selectWrite( &tm );
		res = send_int( msg );
	}catch(JPGenericSocket &e ){
		throw e;
	}catch( JPErrEmptySocket &e ){
		//Do nothing
	}
	FD_CLR(socketfd,&write_flags);
	FD_CLR(socketfd,&read_flags);
	return res;
}

/**
 * END }
 */
