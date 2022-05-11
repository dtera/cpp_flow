cmake_minimum_required(VERSION 3.18)

project(cpp_flow)

add_library(${PROJECT_NAME}-lib common/util/Utils.cpp proto/gen/mmdcotservicehttpbroker.pb.cc
        common/http/connection.cpp common/http/mime_types.cpp common/http/reply.cpp common/http/request_handler.cpp
        common/http/request_parser.cpp common/http/server.cpp
        )

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

set(SOURCE_FILES main.cpp)

#set(LIB_SUFFIX dylib)
set(LIB_SUFFIX so)
set(LOCAL_OPT_DIR /usr/local/opt)
set(BREW_OPT_DIR /opt/homebrew/opt)
set(OPENSSL_ROOT_DIR ${BREW_OPT_DIR}/openssl)
set(OPENSSL_LIBRARIES ${OPENSSL_ROOT_DIR}/lib)
set(INC_DIR . ./include ./proto/gen ./proto/src /usr/local/include /opt/homebrew/include
        ${OPENSSL_ROOT_DIR}/include ${LOCAL_OPT_DIR}/jsoncpp/include ${BREW_OPT_DIR}/jsoncpp/include
        )
set(LINK_DIR ./lib ${LOCAL_DIR}/lib /opt/homebrew/lib
        ${LOCAL_OPT_DIR}/jsoncpp/lib ${BREW_OPT_DIR}/jsoncpp/lib
        )
set(LINK_LIB libjsoncpp.${LIB_SUFFIX})

include_directories(${INC_DIR})
link_directories(${LINK_DIR})
link_libraries(${LINK_LIB})

find_package(Boost COMPONENTS system filesystem thread REQUIRED)
if(Boost_FOUND)
    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
    message(STATUS "Boost_VERSION: ${Boost_VERSION}")
endif()

find_package(Protobuf REQUIRED)

find_package(Hiredis REQUIRED)

find_package(OpenSSL REQUIRED)

find_package(oatpp REQUIRED)

target_link_libraries(${PROJECT_NAME}-lib
        PUBLIC ${Boost_LIBRARIES}
        PUBLIC ${Protobuf_LIBRARIES}
        PUBLIC ${Hiredis_LIBRARIES}
        PUBLIC ${OPENSSL_LIBRARIES}
        PUBLIC oatpp::oatpp;oatpp::oatpp-test
        )

target_include_directories(${PROJECT_NAME}-lib PUBLIC ot-server)

add_executable(${PROJECT_NAME}-exe ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME}-exe ${PROJECT_NAME}-lib)
#add_dependencies(${PROJECT_NAME}-exe ${PROJECT_NAME}-lib)

set_target_properties(${PROJECT_NAME}-lib ${PROJECT_NAME}-exe PROPERTIES
        CXX_STANDARD 17
        CXX_EXTENSIONS OFF
        CXX_STANDARD_REQUIRED ON
        )
