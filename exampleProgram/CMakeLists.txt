CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(clitcp_src clientTCP.cpp) 
SET(srvtcp_src serverTCP.cpp) 
SET(cliudp_src clientUDP.cpp) 
SET(srvudp_src serverUDP.cpp) 
ADD_EXECUTABLE( cliTcp ${clitcp_src})
ADD_EXECUTABLE( srvTcp ${srvtcp_src})
ADD_EXECUTABLE( cliUdp ${cliudp_src})
ADD_EXECUTABLE( srvUdp ${srvudp_src})

TARGET_LINK_LIBRARIES(cliTcp JPSockets ${LOGGER_LIBRARY})
TARGET_LINK_LIBRARIES(srvTcp JPSockets ${LOGGER_LIBRARY} )
TARGET_LINK_LIBRARIES(cliUdp JPSockets ${LOGGER_LIBRARY} )
TARGET_LINK_LIBRARIES(srvUdp JPSockets ${LOGGER_LIBRARY} )

