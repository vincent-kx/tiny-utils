#
# include all header directories
#

set(MY_PROJ_INCLUDE_PATH $ENV{MY_INCLUED_PARH})

include_directories(${MY_PROJ_INCLUDE_PATH}/include/redis)
include_directories(${MY_PROJ_INCLUDE_PATH}/include/baofoo)
include_directories(${MY_PROJ_INCLUDE_PATH}/include/mytools)
include_directories(${MY_PROJ_INCLUDE_PATH}/include/zmq)
include_directories(${MY_PROJ_INCLUDE_PATH}/include/server_frame)
include_directories(${MY_PROJ_INCLUDE_PATH}/include/)

#
# library search directories
#

link_directories(${MY_PROJ_INCLUDE_PATH}/lib)

