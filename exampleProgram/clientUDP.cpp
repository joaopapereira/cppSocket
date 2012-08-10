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
#include <libJPUdpSocket.hpp>
#include <JPSockExceptions.hpp>
#include <stdlib.h>
#include <errno.h>
using namespace cppLibs;
// With this option will disable the debug to screen and do not allow any default log to be written
#define DEBUG 0
int main(void) {
  Logger log("/tmp/test.log");
  log.setLogLvl("SOC",M_LOG_MIN,M_LOG_ALLLVL);

  JPUdpSocket cli(&log);
  try{
  	  std::string ip("127.0.0.1");
      cli.setAddress(&ip,9999);
	  cli.create();
	  cli.connect(&ip,9999);
	  std::string msg("panado com pao");
	  cli.send(&msg);

	  std::string * recv;
	  cli.receive(100,&recv);

	  std::cout << "the message received was: "<< *recv<< std::endl;

	  cli.disconnect();
  }catch( SocketExceptions &e){
      std::cerr << e.what()<< std::endl;
      std::cerr << errno << std::endl;
      exit(-1);
  }



  return 0;
}
