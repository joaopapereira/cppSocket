
#include <JPSocket.hpp>
#include <string.h>
#include <netdb.h>
#include <openssl/md5.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <JPSockExceptions.hpp>

using namespace cppLibs;

#define BUFFER_READ 50


const std::string JPSocket::moduleName = "SOC";


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
JPSocket::JPSocket(Logger * log, std::string address, int port ){
	logger = log;

	logger->log(JPSocket::moduleName,M_LOG_NRM,M_LOG_TRC,"JPSocket(%s,%d)", address.c_str(), port);

	struct hostent *server;
	server = gethostbyname(address.c_str());
	if (server == NULL) {
		logger->log(JPSocket::moduleName,M_LOG_HGH,M_LOG_WRN,"Address[%s] is not valid", address.c_str());
		throw JPInvHostname();
	}
	std::string ip(server->h_addr);
	this->addressVersion = JPIpAddress::checkIpVersion(server->h_addr);
	if( AF_INET == addressVersion ){
		this->address = new JPSocketIPv4();
		this->address->setIp(ip);
	}else{
		this->address = new JPSocketIPv6();
		this->address->setIp(ip);
	}

	port = htons(port);



}
/**
 * Empty constructor
 * @param log Pointer to a logger
 */
JPSocket::JPSocket(Logger * log){
	logger = log;
	logger->log(JPSocket::moduleName,M_LOG_NRM,M_LOG_TRC,"JPSocket()");
	socketfd = -1;
	addressVersion = -1;
	port = -1;
	address = NULL;
}
/**
 * Copy class constructor
 * This constructor should be used if socket is already
 * created
 * @param copyFrom Object from wish we want to copy
 */
JPSocket::JPSocket( const JPSocket& copyFrom ){
	logger = copyFrom.logger;
	socketfd = copyFrom.socketfd;
	addressVersion = copyFrom.addressVersion;
	port = copyFrom.port;
	*address = *(copyFrom.address);
}
/**
 * Class destructor
 */
JPSocket::~JPSocket(){}

/**
 * Send a message to the socket
 * @param msg Message to be sent through the socket
 * @return Integer 0 in case of success
 */
int 
JPSocket::send(std::string * msg){
	logger->log(JPSocket::moduleName,M_LOG_LOW,M_LOG_TRC,"JPSocket::send(%s)",msg->c_str());
	return send_int( msg );
}
/**
 * Internal function used to write a message into
 * the socket
 * @param msg Message to be sent through the socket
 * @return Integer 0 in case of success
 */
int 
JPSocket::send_int(std::string * msg)
{
	logger->log(JPSocket::moduleName,M_LOG_NRM,M_LOG_TRC,"send_int(%p) on socket[%d]", msg, socketfd);

	int n = ::send(socketfd,msg->data(),msg->size(),0);
	if (n < 0){
		logger->log("ERROR writing to socket",JPSocket::moduleName,M_LOG_HGH,M_LOG_WRN);
		throw JPErrSend(true);
	}

	logger->log(JPSocket::moduleName,M_LOG_LOW,M_LOG_DBG,"Sent following message %s",msg->c_str());
	logger->log(JPSocket::moduleName,M_LOG_NRM,M_LOG_DBG,"Message sent size was %d",n);

	return 0;
}
/**
 * Read a message from the socket
 * @param strsize	Size of the message that want to be readed
 * @param msg		String where the message will be placed
 * @return Integer 0 in case of success
 */
int 
JPSocket::receive(int strsize, std::string **msg){
	if( NULL != msg )
		logger->log(JPSocket::moduleName,M_LOG_LOW,M_LOG_TRC,"JPSocket::receive(%d,%s)",strsize,(*msg)->c_str());
	else
		logger->log(JPSocket::moduleName,M_LOG_LOW,M_LOG_TRC,"JPSocket::receive(%d,NULL)",strsize);
	return receive_int( strsize , msg );
}
/**
 * Internal function used to read a message from the socket
 * @param strsize	Size of the message that want to be read
 * @param msg		String where the message will be placed
 * @return Integer 0  in case of success
 *                 >0 Length read
 */
int 
JPSocket::receive_int(int strsize, std::string **msg){
	logger->log(JPSocket::moduleName,M_LOG_NRM,M_LOG_TRC,"receive_int(%d,%p) on socket[%d]", strsize,msg, socketfd);
	if( NULL != *msg )
		free(*msg);

	*msg = new std::string();
	char out[BUFFER_READ];
	int n, length=0;
	bzero(out,BUFFER_READ);
	logger->log(JPSocket::moduleName,M_LOG_NRM,M_LOG_DBG,"Starting the read loop");
	while( 0 < (n = recv(socketfd,out,BUFFER_READ,0) )){
		if( n > 0 ){
			length += n;
			(*msg)->append( out , n);
			if( n < BUFFER_READ )
				break;
			bzero(out,BUFFER_READ);
		}
		logger->log(JPSocket::moduleName,M_LOG_LOW,M_LOG_DBG,"actually received: %s",(*msg)->c_str());
	}
	if( 0 == length ){
		logger->log(JPSocket::moduleName,M_LOG_NRM,M_LOG_DBG,"Error nothing from socket");
		throw JPErrEmptySocket();
	}
	logger->log(JPSocket::moduleName,M_LOG_LOW,M_LOG_DBG,"Received following message %s",(*msg)->c_str());
	logger->log(JPSocket::moduleName,M_LOG_NRM,M_LOG_DBG,"Message received size was %d",length);
	return length;
}

/**
 * End the socket connectiorn
 * @param type Type of connection end
 * 				M_DISC_GRACE If want to wait for acknowledge
 * 				M_DISC_FORCE If want a force end
 * @return Integer 0 in case of success
 */
int 
JPSocket::disconnect( int type ){
	logger->log(JPSocket::moduleName,M_LOG_NRM,M_LOG_WRN,"disconnect(%d)",type);

	int n;
	switch( type ){
		case M_DISC_GRACE:
			logger->log(JPSocket::moduleName,M_LOG_HGH,M_LOG_DBG,"Graceful disconnect on socket[%d]",type,socketfd);
			n = shutdown(socketfd, SHUT_RD);
		break;
		case M_DISC_FORCE:
			logger->log(JPSocket::moduleName,M_LOG_HGH,M_LOG_DBG,"Force disconnect on socket[%d]",socketfd);
			n = shutdown(socketfd, SHUT_RDWR);
		break;
		default:
			logger->log(JPSocket::moduleName,M_LOG_HGH,M_LOG_DBG,"Invalid disconnect option [%d] on socket[%d]",type,socketfd);
			return -1;
	}
	return 0;
}
/**
 * Get the Ip where the socket is binded
 * or the Ip where the socket is connected to
 * depending if this socket is being used as a
 * server or a client
 * @return String with the IP address
 */
std::string
JPSocket::getIp(){
	logger->log(JPSocket::moduleName,M_LOG_LOW,M_LOG_TRC,"getIp()");
	if( this->addressVersion != -1 )
		return address->getIp();

	logger->log(JPSocket::moduleName,M_LOG_HGH,M_LOG_DBG,"Invalid ip address type");
	throw JPInvIpAddress();
}
#if 0
/**
 * Check IP version
 * @param ip String with the IP
 * @return Version
 */
int
JPSocket::checkIpVersion( std::string ip ){
	logger->log(JPSocket::moduleName,M_LOG_MIN,M_LOG_TRC,"checkIpVersion(%s)",ip.c_str());
	struct addrinfo hint, *res = NULL;
	int ret;

	memset(&hint, '\0', sizeof hint);

	hint.ai_family = PF_UNSPEC;
	hint.ai_flags = AI_NUMERICHOST;

	ret = getaddrinfo(ip.c_str(), NULL, &hint, &res);
	if (ret) {
		std::string err("Invalid address![");
		err.append(gai_strerror(ret));
		err.append("]");
		logger->log(JPSocket::moduleName,M_LOG_NRM,M_LOG_WRN,"Error on ip[%s] - [%s]",ip.c_str(),err.c_str());
		throw SocketExceptions(err);
	}
	if(res->ai_family == AF_INET) {
		logger->log(JPSocket::moduleName,M_LOG_LOW,M_LOG_DBG,"IPv4 found[%s]",ip.c_str());
		ret = AF_INET;
	} else if (res->ai_family == AF_INET6) {
		logger->log(JPSocket::moduleName,M_LOG_LOW,M_LOG_DBG,"IPv6 found[%s]",ip.c_str());
		ret = AF_INET6;
	} else {
		logger->log(JPSocket::moduleName,M_LOG_LOW,M_LOG_DBG,"Unknown IP format[%s] format id:%d",ip.c_str(),res->ai_family);
		throw SocketExceptions("Unknown address format");
	}

   freeaddrinfo(res);
   return ret;
}
#endif

/**
 * END }
 */



