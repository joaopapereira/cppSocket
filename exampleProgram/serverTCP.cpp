/*
 ============================================================================
 Name        : exampleProgram.c
 Author      : Joao Pereira
 Version     :
 Copyright   : This library is creating under the GPL license
 Description : Uses shared library to print greeting
               To run the resulting executable the LD_LIBRARY_PATH must be
               set to ${project_loc}/libJPLogger/.libs
               Alternatively, libtool creates a wrapper shell script in the
               build directory of this program which can be used to run it.
               Here the script will be called exampleProgram.
 ============================================================================
 */

#include <extlibs/libJPLogger.h>
#include <libJPTcpSocket.hpp>
#include <JPSockExceptions.hpp>
#include <stdlib.h>
#include <errno.h>
using namespace cppLibs;
// With this option will disable the debug to screen and do not allow any default log to be written
#define DEBUG 0
int main(void) {
  Logger log("/tmp/test.log");
  log.setLogLvl("SOC",M_LOG_MIN,M_LOG_ALLLVL);
  log.log("SOC",M_LOG_NRM,M_LOG_DBG,"Starting application");

  JPTcpSocket srv(&log);
  std::string ip("127.0.0.1");
  try{
	  srv.setAddress(ip,9999);
	  srv.create();


	  srv.bind(ip,9999);
	  sleep(5);

	  std::string * recv;
	  srv.receive(100,&recv);
	  std::cout << "the message received was: "<< *recv<< std::endl;

	  std::string msg("pao com panado");
	  srv.send(&msg);





	  srv.disconnect();

  }catch( SocketExceptions &e){
    std::cerr << e.what()<< std::endl;
    std::cerr << errno << std::endl;
    exit(-1);
  }

  return 0;
}
