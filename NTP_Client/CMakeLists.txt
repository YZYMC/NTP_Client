cmake_minimum_required(VERSION 3.10)
project(NtpSyncClient LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 定义源文件
set(SOURCES
    main.cpp
)

# 添加可执行文件
add_executable(ntp_client ${SOURCES})

# Windows 特有设置
if(WIN32)
    target_link_libraries(ntp_client ws2_32)
endif()

# 设置 UTF-8 编码（Windows 下防乱码）
if (MSVC)
    target_compile_options(ntp_client PRIVATE /source-charset:utf-8)
endif()

# 复制配置文件到输出目录
add_custom_command(TARGET ntp_client POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/config.ini"
        "$<TARGET_FILE_DIR:ntp_client>/config.ini"
)
