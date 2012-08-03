
#include "libJPSocket.hpp"
#include <string.h>
#include <errno.h>

using namespace cppLibs;
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
 * Constructor used when no message needs to be passed
 * @param showErrno Indicates if the ERRNO should be or not written
 */
SocketExceptions::SocketExceptions( const char* error, bool showErrno ) throw(){
	this->showErrno = showErrno;
	setMsg(error);

}
/**
 * Constructor used when no message needs to be passed
 * @param error Error message
 * @param showErrno Indicates if the ERRNO should be or not written
 */
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
	if( showErrno && ( errno != 0 ) ){
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
/**
 * END }
 */



