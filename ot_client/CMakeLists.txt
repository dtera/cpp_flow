cmake_minimum_required(VERSION 3.18)
project(ot_client)

set(CMAKE_CXX_STANDARD 17)

#set(LIB_SUFFIX dylib)
set(LIB_SUFFIX so)
set(INC_DIR /usr/local/include)
set(LINK_DIR /usr/local/lib)
set(LINK_LIB boost_filesystem.${LIB_SUFFIX})

set(PROJECT_INC_DIR . .. ../include ../proto/gen)
set(PROJECT_LINK_DIR ../lib)
set(PROJECT_LINK_LIB )

set(OPENSSL_INC_DIR /usr/local/opt/openssl/include)
set(OPENSSL_LINK_DIR /usr/local/opt/openssl/lib)
set(OPENSSL_LINK_LIB libcrypto.${LIB_SUFFIX} libssl.${LIB_SUFFIX})

include_directories(${INC_DIR})
link_directories(${LINK_DIR})
link_libraries(${LINK_LIB})

include_directories(${PROJECT_INC_DIR})
link_directories(${PROJECT_LINK_DIR})
link_libraries(${PROJECT_LINK_LIB})

include_directories(${OPENSSL_INC_DIR})
link_directories(${OPENSSL_LINK_DIR})
link_libraries(${OPENSSL_LINK_LIB})

set(SOURCE_FILES
        App.cpp
        test/app/MyApiTestClient.hpp ../common/util/OatppUtils.hpp)

set(TEST_SOURCE_FILES
        test/Tests.cpp
        test/app/TestComponent.hpp
        test/app/MyApiTestClient.hpp
        test/MyControllerTest.cpp
        test/MyControllerTest.hpp
        )

include_directories(${INC_DIR})
link_directories(${LINK_DIR})
link_libraries(${LINK_LIB})

add_library(${PROJECT_NAME}-lib
        ../common/util/Utils.cpp
        )

find_package(oatpp REQUIRED)
target_link_libraries(${PROJECT_NAME}-lib
        PUBLIC oatpp::oatpp
        PUBLIC oatpp::oatpp-test)

add_executable(${PROJECT_NAME}-exe ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME}-exe ${PROJECT_NAME}-lib)
add_dependencies(${PROJECT_NAME}-exe ${PROJECT_NAME}-lib)

add_executable(${PROJECT_NAME}-test ${TEST_SOURCE_FILES})
target_link_libraries(${PROJECT_NAME}-test ${PROJECT_NAME}-lib)
add_dependencies(${PROJECT_NAME}-test ${PROJECT_NAME}-lib)

target_include_directories(${PROJECT_NAME}-lib PUBLIC ot-server)

set_target_properties(${PROJECT_NAME}-lib ${PROJECT_NAME}-exe ${PROJECT_NAME}-test PROPERTIES
        CXX_STANDARD 17
        CXX_EXTENSIONS OFF
        CXX_STANDARD_REQUIRED ON
        )

enable_testing()
add_test(project-tests ${PROJECT_NAME}-test)



