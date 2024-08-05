cmake_minimum_required(VERSION 3.10)
project(MyMQTTProject C)

# 指定C标准
set(CMAKE_C_STANDARD 99)

# 设置编译器路径和系统信息
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

# 指定交叉编译器
set(CMAKE_C_COMPILER aarch64-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER aarch64-linux-gnu-g++)

# 指定查找库文件的根路径
set(CMAKE_FIND_ROOT_PATH /usr/aarch64-linux-gnu)

# 使查找程序只在交叉编译器目录中进行
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# 包含的头文件目录
include_directories(include)

# 库文件目录
link_directories(/usr/aarch64-linux-gun/lib)

# 添加源文件
file(GLOB SRC_FILES "*.c")
add_executable(asyn_sub_arm ${SRC_FILES})

# 链接到必要的库
target_link_libraries(asyn_sub_arm /usr/aarch64-linux-gnu/lib/libpaho-mqtt3as.so.1 pthread dl)

# 启用详细的调试信息
set(CMAKE_VERBOSE_MAKEFILE ON)

# 添加 RPATH 设置
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_INSTALL_RPATH /usr/aarch64-linux-gnu)

