cmake_minimum_required(VERSION 3.10)
project(NTP_Client VERSION CMTv1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add source files
add_executable(NTP_Client NTP_Client.cpp)

# Windows
if(WIN32)
    target_link_libraries(NTP_Client ws2_32)
endif()

# Set UTF-8 encoding for source files on MSVC
if (MSVC)
    target_compile_options(NTP_Client PRIVATE /source-charset:utf-8)
endif()

# Copy config.ini to the output directory after build
add_custom_command(TARGET NTP_Client POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/config.ini"
        "$<TARGET_FILE_DIR:NTP_Client>/config.ini"
)
