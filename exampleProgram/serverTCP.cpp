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

#include <extlibs/libJPLogger.hpp>
#include <libJPTcpSocket.hpp>
#include <JPSockExceptions.hpp>
#include <stdlib.h>
#include <errno.h>
using namespace jpCppLibs;
// With this option will disable the debug to screen and do not allow any default log to be written
#define DEBUG 0
int main(void) {
  Logger log("/tmp/test.log");
  log.setLogLvl("SOC",M_LOG_MIN,M_LOG_ALLLVL);
  log.log("SOC",M_LOG_NRM,M_LOG_DBG,"Starting application");

  JPTcpSocket srv(&log);
  JPTcpSocket client(&log);
  std::string ip("127.0.0.1");
  std::string *msg= new std::string();
  try{
	  srv.setAddress(&ip,9999);
	  srv.create();


	  srv.bind(&ip,9999);
	  sleep(5);
	  srv.listen();
	  int reqNum = 0;
	  while( reqNum < 10 ){
		  if( 0 == srv.accept(&client) )
		  {
			  client.receive(300,&msg);
			  std::cout << "Received: " << msg->c_str();

			  msg->assign("Thanks for coming, now leave please!");

			  client.send(msg);
			  sleep(1);
			  client.disconnect();
		  }
		  std::cout << "request number: " << reqNum++;
	  }
	  srv.disconnect();

  }catch( SocketExceptions &e){
    std::cerr << e.what()<< std::endl;
    std::cerr << errno << std::endl;
    exit(-1);
  }

  return 0;
}
