project(sdr_rf_interface VERSION 1.0)

CMAKE_MINIMUM_REQUIRED(VERSION 3.8)

ADD_LIBRARY(sdr_rf_interface INTERFACE)

target_include_directories(sdr_rf_interface INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
target_compile_features(sdr_rf_interface INTERFACE cxx_std_20)

