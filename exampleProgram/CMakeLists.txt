CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(clitcp_src clientTCP.cpp) 
SET(srvtcp_src serverTCP.cpp) 
SET(cliudp_src clientUDP.cpp) 
SET(srvudp_src serverUDP.cpp) 
ADD_EXECUTABLE( cliTcp ${clitcp_src})
ADD_EXECUTABLE( srvTcp ${srvtcp_src})
ADD_EXECUTABLE( cliUdp ${cliudp_src})
ADD_EXECUTABLE( srvUdp ${srvudp_src})

TARGET_LINK_LIBRARIES(cliTcp JPSockets ${INSTALL_LIBS}/libJPLoggerStatic.a )
TARGET_LINK_LIBRARIES(srvTcp JPSockets ${INSTALL_LIBS}/libJPLoggerStatic.a )
TARGET_LINK_LIBRARIES(cliUdp JPSockets ${INSTALL_LIBS}/libJPLoggerStatic.a )
TARGET_LINK_LIBRARIES(srvUdp JPSockets ${INSTALL_LIBS}/libJPLoggerStatic.a )

