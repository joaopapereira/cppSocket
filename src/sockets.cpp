
#include "MSocket.hpp"
#include <string.h>
#include <netdb.h>
#include <openssl/md5.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <cstdlib>
#include "MExceptions.hpp"

#define BUFFER_READ 50

const std::string MSocket::moduleName = "SOC";

void 
MSocket::log( std::string message, int lvl , int type )
{
	MLog::instance()->log(message,MSocket::moduleName,M_LOG_NRM,M_LOG_TRC);
}

MSocket::MSocket(std::string address, int port ){
	//MLog::instance()->log("start socket",MSocket::moduleName,M_LOG_NRM,M_LOG_TRC);
	struct hostent *server;

	MLog::instance()->log(MSocket::moduleName,M_LOG_NRM,M_LOG_TRC,"MSocket(%s,%d)", address.c_str(), port);
	socketfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if( socketfd < 0 ){
		log("Error starting socket");
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
MSocket::MSocket( int sockId ){
	MLog::instance()->log(MSocket::moduleName,M_LOG_NRM,M_LOG_TRC,"MSocket(%d)", sockId);
	log("started socket");
	socketfd = sockId;
}
MSocket::MSocket(){
	MLog::instance()->log(MSocket::moduleName,M_LOG_NRM,M_LOG_TRC,"MSocket()");
	socketfd = -1;
}

MSocket::~MSocket(){}

int
MSocket::setSoc( int sockId ){
	return setSoc_int( sockId );
}
int
MSocket::setSoc_int( int sockId ){
	MLog::instance()->log(MSocket::moduleName,M_LOG_NRM,M_LOG_TRC,"setSoc_int(%d)", sockId);
	socketfd = sockId;
	return 0;
}

int 
MSocket::send(std::string * msg){
	MLog::instance()->log(MSocket::moduleName,M_LOG_LOW,M_LOG_TRC,"MSocket::send(%s)",msg->c_str());
	return send_int( msg );
}
int 
MSocket::send_int(std::string * msg)
{
	MLog::instance()->log(MSocket::moduleName,M_LOG_NRM,M_LOG_TRC,"send_int(%p) on socket[%d]", msg, socketfd);
	log("Send data");

	//int n = write(socketfd,msg->data(),msg->size());
	int n = ::send(socketfd,msg->data(),msg->size(),0);
	if (n < 0)
		MLog::instance()->log("ERROR writing to socket",MSocket::moduleName,M_LOG_NRM,M_LOG_WRN);
	char aux[5000];
	sprintf(aux,"sent %d/%d chars\n",n,msg->size());
	log(aux);

	return marta::m_success;
}
int 
MSocket::receive(int strsize, std::string **msg){
	if( NULL != msg )
		MLog::instance()->log(MSocket::moduleName,M_LOG_LOW,M_LOG_TRC,"MSocket::receive(%d,%s)",strsize,(*msg)->c_str());
	else
		MLog::instance()->log(MSocket::moduleName,M_LOG_LOW,M_LOG_TRC,"MSocket::receive(%d,NULL)",strsize);
	return receive_int( strsize , msg );
}
int 
MSocket::receive_int(int strsize, std::string **msg){
	MLog::instance()->log(MSocket::moduleName,M_LOG_NRM,M_LOG_TRC,"receive_int(%d,%p) on socket[%d]", strsize,msg, socketfd);
	if( NULL != *msg )
		free(*msg);
//	msg =(char *) malloc(strsize);
	*msg = new std::string();
	char out[BUFFER_READ];
	int n, length=0;
	bzero(out,BUFFER_READ);
	MLog::instance()->log(MSocket::moduleName,M_LOG_NRM,M_LOG_DBG,"Starting the loop");
	//while( 0 < (n = read(socketfd,out,BUFFER_READ) )){
	while( 0 < (n = recv(socketfd,out,BUFFER_READ,0) )){
		if( n > 0 ){
			length += n;
			(*msg)->append( out , n);
			/*msg = (char*)realloc(msg,length);
			if( NULL == memcpy( (msg+(length-n)) , out, n ) )
				MLog::instance()->log(MSocket::moduleName,M_LOG_NRM,M_LOG_WRN,"error moving data");*/
			if( n < BUFFER_READ )
				break;
			bzero(out,BUFFER_READ);
		}
		MLog::instance()->log(MSocket::moduleName,M_LOG_NRM,M_LOG_DBG,"actually received: %s",(*msg)->c_str());
	}
	if( 0 == length ){
		MLog::instance()->log(MSocket::moduleName,M_LOG_NRM,M_LOG_DBG,"Error nothing from socket");
		throw MExpSocketEmpty();
		return -1;	
	}
	MLog::instance()->log(MSocket::moduleName,M_LOG_LOW,M_LOG_TRC,"receied %d chars",length);
	return length;
}

int 
MSocket::received(){
	log("Received");
	return marta::m_success;
}
int 
MSocket::disconnect( int type ){
	MLog::instance()->log(MSocket::moduleName,M_LOG_NRM,M_LOG_WRN,"disconnect(%d)",type);

	int n;
	switch( type ){
		case M_DISC_GRACE:
			n = shutdown(socketfd, SHUT_RD);
		break;
		case M_DISC_FORCE:
			n = shutdown(socketfd, SHUT_RDWR);
		break;
		default:
			log("invalid option");
			return marta::m_genError;
	}
	return marta::m_success;
}

int MNonBlockSocket::selectRead(  struct timeval * waitd  ){
	MLog::instance()->log(MSocket::moduleName,M_LOG_NRM,M_LOG_TRC,"selectRead(%p) on socket[%d]", waitd, socketfd);
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
int MNonBlockSocket::selectWrite(  struct timeval * waitd ){
	MLog::instance()->log(MSocket::moduleName,M_LOG_NRM,M_LOG_TRC,"selectWrite(%p) on socket[%d]", waitd, socketfd);
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
int MNonBlockSocket::select(  struct timeval * waitd ){
	return ::select( socketfd+1, &read_flags,&write_flags, (fd_set*)0,waitd);
}

int 
MNonBlockSocket::receive(int strsize, std::string **msg){
	if( NULL != *msg )
		MLog::instance()->log(MSocket::moduleName,M_LOG_LOW,M_LOG_TRC,"MNonBlockSocket::receive(%d,%s)",strsize,(*msg)->c_str());
	else
		MLog::instance()->log(MSocket::moduleName,M_LOG_LOW,M_LOG_TRC,"MNonBlockSocket::receive(%d,NULL)",strsize);
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
MNonBlockSocket::send( std::string * msg ){
	MLog::instance()->log(MSocket::moduleName,M_LOG_LOW,M_LOG_TRC,"MNonBlockSocket::send(%s)",msg->c_str());
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
MNonBlockSocket::MNonBlockSocket(std::string address, int port)
	: MSocket(address, port){
	nonblock();
}
MNonBlockSocket::MNonBlockSocket( int sockId )
	:MSocket( sockId ){
	nonblock();
}
MNonBlockSocket::MNonBlockSocket()
	:MSocket(){
}
int
MNonBlockSocket::setSoc( int sockId ){
	int res = setSoc( sockId );
	nonblock();
}
int 
MNonBlockSocket::nonblock(){
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
