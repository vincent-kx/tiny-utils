###################################################################
#
#   cmake smsGateway 配置文件，add by jie.wang 2016-07-17
#
#
###################################################################

# cmake最低版本号
cmake_minimum_required(VERSION 2.8)

# 设置GXX编译参数
add_compile_options(-std=c++0x -g -Wall -o2)

#设置项目名称
project(LOAN_SKU_VIEW_SRV)

SET(CMAKE_SKIP_RULE_DEPENDENCY TRUE)
SET(PYTHON python)
SET(PROJ_ROOT_PATH $ENV{PROJECT_ROOT}/LoanSkuServer)
message("proj_proto_install.py running")
EXECUTE_PROCESS(COMMAND ${PYTHON} $ENV{HOME}/proj_proto_install.py ${PROJ_ROOT_PATH})
message("proj_proto_install.py done")
INCLUDE(${PROJ_ROOT_PATH}/tmp_proto/config.proto.cmake)
INCLUDE_DIRECTORIES($ENV{BASE_PROTOLIB_PATH}/all/inc)

#设置cpp文件
AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}  SOURCE_CPP)
AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}/../../common/ COMM_CPP)
list(APPEND SOURCE_CPP ${COMM_CPP})

#设置头文件路径
SET(INC_PATH ${PROJECT_SOURCE_DIR})


INCLUDE($ENV{BASE_3PARTY_PATH}/zmq/config.zmq.cmake)
INCLUDE($ENV{BASE_3PARTY_PATH}/uuid/config.uuid.cmake)
INCLUDE($ENV{BASE_3PARTY_PATH}/jsoncpp/config.json.cmake)
# INCLUDE($ENV{BASE_3PARTY_PATH}/curl/config.curl.cmake)
INCLUDE($ENV{BASE_3PARTY_PATH}/protobuf/config.protobuf.cmake)
INCLUDE($ENV{BASE_3PARTY_PATH}/libevent/config.libevent.cmake)
INCLUDE($ENV{BASE_3PARTY_PATH}/mysql/config.mysql.cmake)
INCLUDE($ENV{BASE_3PARTY_PATH}/hiredis/config.hiredis.cmake)
INCLUDE($ENV{BASE_MYAPI_PATH}/config.myapi.cmake)

INCLUDE_DIRECTORIES($ENV{BASE_3PARTY_PATH}/boost/include)
LINK_DIRECTORIES($ENV{BASE_3PARTY_PATH}/boost/lib)
INCLUDE_DIRECTORIES($ENV{BASE_PROTOLIB_PATH}/all/inc)
#INCLUDE_DIRECTORIES($ENV{BASE_PROTOLIB_PATH}/tbl/)
# LINK_DIRECTORIES($ENV{BASE_PROTOLIB_PATH}/lib)

INCLUDE_DIRECTORIES(${INC_PATH})
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/../../common/)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/../../tbl/)
INCLUDE_DIRECTORIES($ENV{BASE_3PARTY_PATH}/hiredis)
INCLUDE_DIRECTORIES($ENV{BASE_3PARTY_PATH}/hiredis/adapters)

add_executable(loan_sku_detail ${SOURCE_CPP})

target_link_libraries(loan_sku_detail libboost_regex.a dc_api json zmq sms_api apollo_api qos_client trace_v2 uuid proto protobuf mysql curl hiredis libevent dl crypto z rt pthread)

SET_TARGET_PROPERTIES(loan_sku_detail PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/../bin/")
