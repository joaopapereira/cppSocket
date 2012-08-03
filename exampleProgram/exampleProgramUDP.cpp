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

#include "libJPLogger.h"

// With this option will disable the debug to screen and do not allow any default log to be written
#define DEBUG 0
int main(void) {
  Logger log("/tmp/test.log");
  log.log("Before first set","Ex0",M_LOG_LOW,M_LOG_INF);
  log.setLogLvl("Ex0",M_LOG_MAX,M_LOG_INF);
  log.log("Before change to low","Ex0",M_LOG_LOW,M_LOG_INF);
  log.setLogLvl("Ex0",M_LOG_LOW,M_LOG_INF);

  log.log("After change to low","",M_LOG_LOW,M_LOG_INF);

  log.setLogLvl("Ex0",M_LOG_NRM,M_LOG_INF);
  OneInstanceLogger::instance()->log("Try to log","Ex0",M_LOG_LOW,M_LOG_INF);

  OneInstanceLogger::instance()->copyLoggerDef( &log );
  OneInstanceLogger::instance()->log("After the copy","Ex0",M_LOG_LOW,M_LOG_INF);


  return 0;
}
