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
using namespace cppLibs;
// With this option will disable the debug to screen and do not allow any default log to be written
#define DEBUG 0
int main(void) {
  Logger log("/tmp/test.log");
  log.setLogLvl("SOC",M_LOG_MAX,M_LOG_ALLLVL);

  JPTcpSocket cli(&log);
  cli.create();
  std::string ip("localhost");
  cli.connect(ip,9999);
  std::string msg("panado com pao");
  cli.send(&msg);

  std::string * recv;
  cli.receive(100,&recv);

  std::cout << "the message received was: "<< *recv<< std::endl;

  cli.disconnect();



  return 0;
}
