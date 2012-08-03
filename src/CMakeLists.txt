
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET( lib_srcs sockets.cpp exceptions.cpp iplayer.cpp tcp.cpp udp.cpp)
include_directories( include/ ${INSTALL_INCS})
ADD_LIBRARY( JPSockets STATIC ${lib_srcs})

