#ifndef __MSOCKET_HPP__
#define __MSOCKET_HPP__

#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include "Marta.hpp"
#include "MartaMem.hpp"
#include "MExceptions.hpp"
#include "MLog.hpp"

#define M_DISC_GRACE 0
#define M_DISC_FORCE 1

class MSocket{
	public:
	MSocket( std::string address, int port );
	MSocket( int sockId );
	MSocket();
	~MSocket();

	int send( std::string * msg );
	int setSoc( int sockId );
	int receive(int strsize, std::string **msg);
	int received();
	int disconnect( int type = M_DISC_GRACE );

	private:
	protected:
		static const std::string moduleName;
		int socketfd;
		struct sockaddr_in sockAddr;
		m_address addr;
		void log( std::string message, int lvl = M_LOG_NRM , int type = M_LOG_TRC );
		int send_int( std::string * msg );
		int receive_int(int strsize, std::string **msg);
		int setSoc_int( int sockId );


};
class MNonBlockSocket: public MSocket{
	public: 
		MNonBlockSocket(std::string address, int port);
		MNonBlockSocket( int sockId );
		MNonBlockSocket();
		int selectRead( struct timeval * waitd );
		int selectWrite( struct timeval * waitd );
		int receive(int strsize, std::string **msg);
		int send( std::string * msg );
		int setSoc( int sockId );
	private:
		fd_set read_flags,write_flags;
		int select( struct timeval * waitd );
		int nonblock();
};
/*class MExpSocket: public MExceptions
{
public:
        MExpSocket(const char* error);
	MExpSocket(std::string error);
	virtual ~MExpSocket()throw();
	MExpSocket();
private:
	std::string myerror;
}MExpSocket;
*/
#endif
