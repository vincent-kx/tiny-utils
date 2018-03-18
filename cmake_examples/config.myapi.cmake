#
# include all header directories
#

set(MYAPI_PATH $ENV{BASE_MYAPI_PATH})

include_directories(${MYAPI_PATH}/include)
include_directories(${MYAPI_PATH}/include/apollo_api)
include_directories(${MYAPI_PATH}/include/account)
include_directories(${MYAPI_PATH}/include/duizhang)
include_directories(${MYAPI_PATH}/include/cmq)
include_directories(${MYAPI_PATH}/include/jiean)
include_directories(${MYAPI_PATH}/include/kqpay)
include_directories(${MYAPI_PATH}/include/msgQ)
include_directories(${MYAPI_PATH}/include/mysrv)
include_directories(${MYAPI_PATH}/include/redis)
include_directories(${MYAPI_PATH}/include/sms_api)
include_directories(${MYAPI_PATH}/include/attr_api)
include_directories(${MYAPI_PATH}/include/mycgi)
include_directories(${MYAPI_PATH}/include/p2p_api)
include_directories(${MYAPI_PATH}/include/repay)
include_directories(${MYAPI_PATH}/include/szfs)
include_directories(${MYAPI_PATH}/include/tracer)
include_directories(${MYAPI_PATH}/include/baofoo)
include_directories(${MYAPI_PATH}/include/loan)
include_directories(${MYAPI_PATH}/include/mytools)
include_directories(${MYAPI_PATH}/include/pay_channel)
include_directories(${MYAPI_PATH}/include/sinapay)
include_directories(${MYAPI_PATH}/include/ucfpay)
include_directories(${MYAPI_PATH}/include/zmq)
include_directories(${MYAPI_PATH}/include/cl5)
include_directories(${MYAPI_PATH}/include/dc_api)
include_directories(${MYAPI_PATH}/include/marketing)
include_directories(${MYAPI_PATH}/include/sms)
include_directories(${MYAPI_PATH}/include/umpay)
include_directories(${MYAPI_PATH}/include/insurance_api)
include_directories(${MYAPI_PATH}/include/trace_v2)
include_directories(${MYAPI_PATH}/include/apollo_api)
include_directories(${MYAPI_PATH}/include/credit)
include_directories(${MYAPI_PATH}/include/user_info)
include_directories(${MYAPI_PATH}/include/server_frame)
include_directories(${MYAPI_PATH}/include/)

#
# library search directories
#

link_directories(${MYAPI_PATH}/lib)

