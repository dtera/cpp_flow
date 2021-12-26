cmake_minimum_required(VERSION 3.21)
project(cpp_flow)

set(CMAKE_CXX_STANDARD 17)

set(INC_DIR /usr/local/opt/openssl/include)
set(LINK_DIR /usr/local/opt/openssl/lib)
set(LIB_SUFFIX dylib)
#set(LIB_SUFFIX so)
set(LINK_LIB libcrypto.${LIB_SUFFIX} libssl.${LIB_SUFFIX})

set(SOURCE_FILES main.cpp)

include_directories(${INC_DIR})
link_directories(${LINK_DIR})
link_libraries(${LINK_LIB})

add_library(${PROJECT_NAME}-lib
        common/util/Utils.h common/util/Utils.cpp common/util/Func.h common/util/BitOp.hpp
        ot/BaseOTSender.hpp ot/BaseOTReceiver.hpp ot/KOutOfNForOTSender.hpp ot/KOutOfNForOTReceiver.hpp
        ot/Constant.h ot/Message.hpp tests.h
        )

find_package(oatpp REQUIRED)
target_link_libraries(${PROJECT_NAME}-lib)

add_executable(${PROJECT_NAME}-exe ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME}-exe ${PROJECT_NAME}-lib)
add_dependencies(${PROJECT_NAME}-exe ${PROJECT_NAME}-lib)

target_include_directories(${PROJECT_NAME}-lib PUBLIC ot_server)




