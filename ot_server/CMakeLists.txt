cmake_minimum_required(VERSION 3.21)
project(ot_server)

set(CMAKE_CXX_STANDARD 17)

#set(LIB_SUFFIX dylib)
set(LIB_SUFFIX so)
set(INC_DIR /usr/local/include)
set(LINK_DIR /usr/local/lib)
set(LINK_LIB libhiredis.${LIB_SUFFIX})
set(OPENSSL_INC_DIR /usr/local/opt/openssl/include)
set(OPENSSL_LINK_DIR /usr/local/opt/openssl/lib)
set(OPENSSL_LINK_LIB libcrypto.${LIB_SUFFIX} libssl.${LIB_SUFFIX})

set(SOURCE_FILES App.cpp)

include_directories(${INC_DIR})
link_directories(${LINK_DIR})
link_libraries(${LINK_LIB})
include_directories(${OPENSSL_INC_DIR})
link_directories(${OPENSSL_LINK_DIR})
link_libraries(${OPENSSL_LINK_LIB})

add_library(${PROJECT_NAME}-lib
        ../common/util/Utils.h ../common/util/Utils.cpp ../common/util/Func.h ../common/util/BitOp.hpp
        ../common/dto/DTOs.hpp ../common/dto/OTServerDTOs.hpp ../common/RedisCli.hpp
        ../ot/BaseOTSender.hpp ../ot/BaseOTReceiver.hpp ../ot/KOutOfNForOTSender.hpp ../ot/KOutOfNForOTReceiver.hpp
        ../ot/Constant.h ../ot/Message.hpp
        AppComponent.hpp
        controller/OTServerController.hpp
        service/OTServerService.hpp
        )

find_package(oatpp REQUIRED)
target_link_libraries(${PROJECT_NAME}-lib
        PUBLIC oatpp::oatpp
        PUBLIC oatpp::oatpp-test)

add_executable(${PROJECT_NAME}-exe ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME}-exe ${PROJECT_NAME}-lib)
add_dependencies(${PROJECT_NAME}-exe ${PROJECT_NAME}-lib)

target_include_directories(${PROJECT_NAME}-lib PUBLIC ot-server)

set_target_properties(${PROJECT_NAME}-lib ${PROJECT_NAME}-exe PROPERTIES
        CXX_STANDARD 17
        CXX_EXTENSIONS OFF
        CXX_STANDARD_REQUIRED ON
        )



