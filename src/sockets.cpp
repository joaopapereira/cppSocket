
#include "libJPSocket.hpp"
#include <string.h>
#include <netdb.h>
#include <openssl/md5.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <cstdlib>
#include <errno.h>

using namespace cppLibs;

#define BUFFER_READ 50




SocketExceptions::SocketExceptions(char* error, bool showErrno ) throw(){
	setMsg(error);
	this->showErrno = showErrno;
}
SocketExceptions::SocketExceptions(std::string error, bool showErrno ) throw(){
	setMsg(error.c_str());
	this->showErrno = showErrno;
}
SocketExceptions::SocketExceptions( bool showErrno ) throw(){
	setMsg("Error happened on logger file");
	this->showErrno = showErrno;
}
void
SocketExceptions::setMsg(const char * error) throw(){
	myerror = error;
	if( showErrno && errno != 0 ){
		myerror += "[" ;
		myerror += strerror(errno);
		myerror += "]";
	}
}
SocketExceptions::~SocketExceptions() throw(){
}

const char *
SocketExceptions::what() const throw(){
	return myerror.c_str();
}





const std::string JPSocket::moduleName = "SOC";


JPSocket::JPSocket(Logger * log, std::string address, int port ){
	//logger->log("start socket",JPSocket::moduleName,M_LOG_NRM,M_LOG_TRC);

	logger = log;

	logger->log(JPSocket::moduleName,M_LOG_NRM,M_LOG_TRC,"JPSocket(%s,%d)", address.c_str(), port);



	struct hostent *server;
	socketfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if( socketfd < 0 ){
		logger->log("Error starting socket");
		//throw MExpSocket((std::string)"ERROR opening socket!!");
		//throw MExpSocket;
		throw MComGeneric();
	}
	server = gethostbyname(address.c_str());
	if (server == NULL) {
		log("invalid hostname");
		throw MComHostname();
	}

	bzero((char *) &sockAddr, sizeof(sockAddr));
	sockAddr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, 
	        (char *)&sockAddr.sin_addr.s_addr,
		server->h_length);

	//sockAddr.sin_addr.s_addr = INADDR_ANY;
	sockAddr.sin_port = htons(port);

	addr.address = address;
	addr.port = port;
	char aux[500];
	sprintf(aux,"val:%s:port:%d",server->h_addr,sockAddr.sin_port);
	log(aux);

}
JPSocket::JPSocket( int sockId ){
	logger->log(JPSocket::moduleName,M_LOG_NRM,M_LOG_TRC,"JPSocket(%d)", sockId);
	log("started socket");
	socketfd = sockId;
}
JPSocket::JPSocket(){
	logger->log(JPSocket::moduleName,M_LOG_NRM,M_LOG_TRC,"JPSocket()");
	socketfd = -1;
}

JPSocket::~JPSocket(){}

int
JPSocket::setSoc( int sockId ){
	return setSoc_int( sockId );
}
int
JPSocket::setSoc_int( int sockId ){
	logger->log(JPSocket::moduleName,M_LOG_NRM,M_LOG_TRC,"setSoc_int(%d)", sockId);
	socketfd = sockId;
	return 0;
}

int 
JPSocket::send(std::string * msg){
	logger->log(JPSocket::moduleName,M_LOG_LOW,M_LOG_TRC,"JPSocket::send(%s)",msg->c_str());
	return send_int( msg );
}
int 
JPSocket::send_int(std::string * msg)
{
	logger->log(JPSocket::moduleName,M_LOG_NRM,M_LOG_TRC,"send_int(%p) on socket[%d]", msg, socketfd);
	log("Send data");

	//int n = write(socketfd,msg->data(),msg->size());
	int n = ::send(socketfd,msg->data(),msg->size(),0);
	if (n < 0)
		logger->log("ERROR writing to socket",JPSocket::moduleName,M_LOG_NRM,M_LOG_WRN);
	char aux[5000];
	sprintf(aux,"sent %d/%d chars\n",n,msg->size());
	log(aux);

	return marta::m_success;
}
int 
JPSocket::receive(int strsize, std::string **msg){
	if( NULL != msg )
		logger->log(JPSocket::moduleName,M_LOG_LOW,M_LOG_TRC,"JPSocket::receive(%d,%s)",strsize,(*msg)->c_str());
	else
		logger->log(JPSocket::moduleName,M_LOG_LOW,M_LOG_TRC,"JPSocket::receive(%d,NULL)",strsize);
	return receive_int( strsize , msg );
}
int 
JPSocket::receive_int(int strsize, std::string **msg){
	logger->log(JPSocket::moduleName,M_LOG_NRM,M_LOG_TRC,"receive_int(%d,%p) on socket[%d]", strsize,msg, socketfd);
	if( NULL != *msg )
		free(*msg);
//	msg =(char *) malloc(strsize);
	*msg = new std::string();
	char out[BUFFER_READ];
	int n, length=0;
	bzero(out,BUFFER_READ);
	logger->log(JPSocket::moduleName,M_LOG_NRM,M_LOG_DBG,"Starting the loop");
	//while( 0 < (n = read(socketfd,out,BUFFER_READ) )){
	while( 0 < (n = recv(socketfd,out,BUFFER_READ,0) )){
		if( n > 0 ){
			length += n;
			(*msg)->append( out , n);
			/*msg = (char*)realloc(msg,length);
			if( NULL == memcpy( (msg+(length-n)) , out, n ) )
				logger->log(JPSocket::moduleName,M_LOG_NRM,M_LOG_WRN,"error moving data");*/
			if( n < BUFFER_READ )
				break;
			bzero(out,BUFFER_READ);
		}
		logger->log(JPSocket::moduleName,M_LOG_NRM,M_LOG_DBG,"actually received: %s",(*msg)->c_str());
	}
	if( 0 == length ){
		logger->log(JPSocket::moduleName,M_LOG_NRM,M_LOG_DBG,"Error nothing from socket");
		throw MExpSocketEmpty();
		return -1;	
	}
	logger->log(JPSocket::moduleName,M_LOG_LOW,M_LOG_TRC,"receied %d chars",length);
	return length;
}

int 
JPSocket::received(){
	log("Received");
	return marta::m_success;
}
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












int JPNonBlockSocket::selectRead(  struct timeval * waitd  ){
	logger->log(JPSocket::moduleName,M_LOG_NRM,M_LOG_TRC,"selectRead(%p) on socket[%d]", waitd, socketfd);
	int res;
	if( -1 == socketfd )
		return marta::m_sockError;
	FD_ZERO(&read_flags);
	FD_ZERO(&write_flags);
	FD_SET(socketfd, &read_flags);

//	res = select( waitd );
	res = ::select( socketfd+1, &read_flags,NULL,NULL,waitd);


	if( res <= 0 ){
		return marta::m_sockError;
	}else if( FD_ISSET(socketfd , &read_flags )  ){
		return marta::m_success;
	}
	return marta::m_sockError;
}
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
int JPNonBlockSocket::select(  struct timeval * waitd ){
	return ::select( socketfd+1, &read_flags,&write_flags, (fd_set*)0,waitd);
}

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
JPNonBlockSocket::JPNonBlockSocket(std::string address, int port)
	: JPSocket(address, port){
	nonblock();
}
JPNonBlockSocket::JPNonBlockSocket( int sockId )
	:JPSocket( sockId ){
	nonblock();
}
JPNonBlockSocket::JPNonBlockSocket()
	:JPSocket(){
}
int
JPNonBlockSocket::setSoc( int sockId ){
	int res = setSoc( sockId );
	nonblock();
}
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
/*
MExpSocket::MExpSocket(const char* error):MExceptions(error){};
MExpSocket::MExpSocket(std::string error):MExceptions(error){};
MExpSocket::~MExpSocket()throw(){};
MExpSocket::MExpSocket():MExceptions((char*)"No message on MExpSocket!!"){};*/
