project(transfer_interface VERSION 1.0)

CMAKE_MINIMUM_REQUIRED(VERSION 3.8)

ADD_LIBRARY(transfer_interface INTERFACE)

target_include_directories(transfer_interface INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
target_compile_features(transfer_interface INTERFACE cxx_std_20)

