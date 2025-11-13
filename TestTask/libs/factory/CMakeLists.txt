project(factory VERSION 1.0)

CMAKE_MINIMUM_REQUIRED(VERSION 3.8)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

file(GLOB_RECURSE SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)

ADD_LIBRARY(factory SHARED ${SOURCE_FILES})

target_include_directories(factory PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_include_directories(factory PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/source
)

# Зависимости factory от других библиотек
target_link_libraries(factory
    rtlsdr_control
    hackrf_control
)

# Также добавляем include директории для интерфейсов
target_include_directories(factory PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../sdr_rf_interface/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../transfer_interface/include>
)

target_compile_features(factory PRIVATE cxx_std_20)

# Флаги компиляции
if(UNIX)
    target_compile_options(factory PRIVATE -Wall -Wextra)
endif()

include("unittest.cmake")
