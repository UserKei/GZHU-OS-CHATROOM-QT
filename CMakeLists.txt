cmake_minimum_required(VERSION 3.16)
project(ChatRoom VERSION 1.0.0 LANGUAGES CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找Qt组件
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Network Sql)

# 如果Qt6不可用，尝试Qt5
if(NOT Qt6_FOUND)
    find_package(Qt5 REQUIRED COMPONENTS Core Widgets Network Sql)
endif()

# 设置Qt相关选项
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 源文件
set(SOURCES
    main.cpp
    mainwindow.cpp
    chatclient.cpp
    chatserver.cpp
    userinfo.cpp
    messagefilter.cpp
    database.cpp
    loginwindow.cpp
    registerdialog.cpp
    usermanagerdialog.cpp
    settingsdialog.cpp
)

# 头文件
set(HEADERS
    mainwindow.h
    chatclient.h
    chatserver.h
    userinfo.h
    messagefilter.h
    database.h
    loginwindow.h
    registerdialog.h
    usermanagerdialog.h
    settingsdialog.h
)

# 创建可执行文件
add_executable(ChatRoom ${SOURCES} ${HEADERS})

# 链接Qt库
if(Qt6_FOUND)
    target_link_libraries(ChatRoom 
        Qt6::Core 
        Qt6::Widgets 
        Qt6::Network 
        Qt6::Sql
    )
else()
    target_link_libraries(ChatRoom 
        Qt5::Core 
        Qt5::Widgets 
        Qt5::Network 
        Qt5::Sql
    )
endif()

# 设置输出目录
set_target_properties(ChatRoom PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# 安装规则
install(TARGETS ChatRoom
    RUNTIME DESTINATION bin
)

# 创建build目录的快捷脚本
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/build_cmake.sh.in"
    "${CMAKE_CURRENT_SOURCE_DIR}/build_cmake.sh"
    @ONLY
)

# 如果是macOS，设置bundle信息
if(APPLE)
    set_target_properties(ChatRoom PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.chatroom.app"
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE_BUNDLE_NAME "ChatRoom"
    )
endif()

# 添加编译器特定的优化
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(ChatRoom PRIVATE -Wall -Wextra -O2)
endif()

# Debug模式下的额外编译选项
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(ChatRoom PRIVATE DEBUG)
endif()
