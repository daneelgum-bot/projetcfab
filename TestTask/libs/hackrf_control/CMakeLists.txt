project(hackrf_control VERSION 1.0)

CMAKE_MINIMUM_REQUIRED(VERSION 3.8)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

file(GLOB_RECURSE SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)

ADD_LIBRARY(hackrf_control SHARED ${SOURCE_FILES})

target_include_directories(hackrf_control PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_include_directories(hackrf_control PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/source
)

find_library(HACKRF_LIB
    NAMES hackrf
    PATHS /usr/lib /usr/local/lib /usr/lib/x86_64-linux-gnu
)

if(HACKRF_LIB)
    message(STATUS "Found libhackrf: ${HACKRF_LIB}")
    target_link_libraries(hackrf_control ${HACKRF_LIB})
else()
    message(WARNING "libhackrf not found - HackRF support will be limited")
endif()

# Проверка заголовочных файлов
find_path(HACKRF_INCLUDE_DIR
    NAMES hackrf.h
    PATHS /usr/include /usr/local/include /usr/include/libhackrf
)

if(HACKRF_INCLUDE_DIR)
    message(STATUS "Found hackrf.h in: ${HACKRF_INCLUDE_DIR}")
    target_include_directories(hackrf_control PRIVATE ${HACKRF_INCLUDE_DIR})
else()
    message(WARNING "hackrf.h not found - HackRF support will be limited")
endif()

target_compile_features(hackrf_control PRIVATE cxx_std_20)

# Флаги компиляции
if(UNIX)
    target_compile_options(hackrf_control PRIVATE -Wall -Wextra)
endif()

include("unittest.cmake")
