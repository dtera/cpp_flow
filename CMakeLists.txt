cmake_minimum_required(VERSION 3.21)
project(cpp_flow)

set(CMAKE_CXX_STANDARD 14)

set(INC_DIR /usr/local/opt/openssl/include)
set(LINK_DIR /usr/local/opt/openssl/lib)

include_directories(${INC_DIR})
link_directories(${LINK_DIR})
link_libraries(libcrypto.3.dylib libcrypto.a libcrypto.dylib libssl.3.dylib libssl.a libssl.dylib)

add_executable(cpp_flow main.cpp util/Utils.h util/Utils.cpp Test.h
        ot/BaseOTSender.hpp ot/BaseOTReceiver.hpp
        ot/KOutOfNForOTSender.hpp ot/KOutOfNForOTReceiver.hpp
        ot/Constant.h ot/Message.hpp util/Func.h util/BitOp.hpp)

#target_link_libraries(cpp_flow libcrypto.3.dylib libcrypto.a libcrypto.dylib libssl.3.dylib libssl.a libssl.dylib)


