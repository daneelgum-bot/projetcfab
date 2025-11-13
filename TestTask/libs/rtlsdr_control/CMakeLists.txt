project(rtlsdr_control VERSION 1.0)

CMAKE_MINIMUM_REQUIRED(VERSION 3.8)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

file(GLOB_RECURSE SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)

ADD_LIBRARY(rtlsdr_control SHARED ${SOURCE_FILES})

target_include_directories(rtlsdr_control PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_include_directories(rtlsdr_control PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/source
)

find_library(RTLSDR_LIB
    NAMES rtlsdr
    PATHS /usr/lib /usr/local/lib /usr/lib/x86_64-linux-gnu
)

if(RTLSDR_LIB)
    message(STATUS "Found librtlsdr: ${RTLSDR_LIB}")
    target_link_libraries(rtlsdr_control ${RTLSDR_LIB})
else()
    message(WARNING "librtlsdr not found - RTL-SDR support will be limited")
endif()

# Проверка заголовочных файлов
find_path(RTLSDR_INCLUDE_DIR
    NAMES rtl-sdr.h
    PATHS /usr/include /usr/local/include /usr/include/librtlsdr
)

if(RTLSDR_INCLUDE_DIR)
    message(STATUS "Found rtl-sdr.h in: ${RTLSDR_INCLUDE_DIR}")
    target_include_directories(rtlsdr_control PRIVATE ${RTLSDR_INCLUDE_DIR})
else()
    message(WARNING "rtl-sdr.h not found - RTL-SDR support will be limited")
endif()

target_compile_features(rtlsdr_control PRIVATE cxx_std_20)

# Флаги компиляции
if(UNIX)
    target_compile_options(rtlsdr_control PRIVATE -Wall -Wextra)
endif()

include("unittest.cmake")
