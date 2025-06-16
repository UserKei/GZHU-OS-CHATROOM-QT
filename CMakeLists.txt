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

# 核心模块源文件
set(CORE_SOURCES
    src/core/database.cpp
    src/core/userinfo.cpp
    src/core/messagefilter.cpp
)

# 网络模块源文件
set(NETWORK_SOURCES
    src/network/chatclient.cpp
    src/network/chatserver.cpp
)

# UI模块源文件
set(UI_SOURCES
    src/ui/windows/loginwindow.cpp
    src/ui/windows/mainwindow.cpp
    src/ui/dialogs/registerdialog.cpp
    src/ui/dialogs/usermanagerdialog.cpp
    src/ui/dialogs/settingsdialog.cpp
)

# 主程序源文件
set(MAIN_SOURCES
    src/main.cpp
)

# 所有源文件
set(ALL_SOURCES
    ${MAIN_SOURCES}
    ${CORE_SOURCES}
    ${NETWORK_SOURCES}
    ${UI_SOURCES}
)

# 创建可执行文件
add_executable(ChatRoom ${ALL_SOURCES})

# 设置包含目录
target_include_directories(ChatRoom PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

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

# 自定义目标：清理
add_custom_target(clean-all
    COMMAND ${CMAKE_BUILD_TOOL} clean
    COMMENT "Cleaning all build files"
)
