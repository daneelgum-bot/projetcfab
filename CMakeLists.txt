cmake_minimum_required(VERSION 3.8)

project(ProjectFA LANGUAGES CXX)

set(UNITTEST ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME}/binaries)

if(UNIX)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/${PROJECT_NAME}/binaries/lib)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/${PROJECT_NAME}/binaries/lib)
elseif(WIN32)
    set(TEST_OUTPUT_PATH ${CMAKE_BINARY_DIR}/${PROJECT_NAME}/binaries)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME}/binaries)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME}/binaries)
endif()

# Установка стандарта C++20 для всего проекта
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(UNITTEST)
    add_subdirectory(googletests/)
endif()

# Добавляем поддиректории для библиотек в правильном порядке
add_subdirectory(TestTask/libs/sdr_rf_interface)
add_subdirectory(TestTask/libs/transfer_interface)
add_subdirectory(TestTask/libs/rtlsdr_control)
add_subdirectory(TestTask/libs/hackrf_control)
add_subdirectory(TestTask/libs/factory)





if(UNITTEST)
    message(" output lib dir " ${CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG})
    enable_testing()
endif()

# Основное приложение
add_executable(${PROJECT_NAME} main.cpp)

# Линкуем библиотеки
target_link_libraries(${PROJECT_NAME}
    factory
    hackrf_control
    rtlsdr_control

)

# Добавляем include директории для заголовков
target_include_directories(${PROJECT_NAME} PRIVATE
    TestTask/libs/factory/include
    TestTask/libs/hackrf_control/include
    TestTask/libs/rtlsdr_control/include
    TestTask/libs/sdr_rf_interface/include
    TestTask/libs/transfer_interface/include

)

if(UNIX)
    add_custom_target(distclean COMMAND find . -name *_automoc.cpp -type f -delete
            COMMAND find . -name moc_*cpp -type f -delete
            COMMAND find . -name *_t -type f -delete
            COMMAND rm -rf ./binaries/lib/*
            COMMAND rm -rf ./binaries/plugins/*
            COMMAND rm -rf ./binaries/tasks/*
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})
endif()
