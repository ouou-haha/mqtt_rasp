cmake_minimum_required(VERSION 3.10)
project(myapp)

# 指定C标准
set(CMAKE_C_STANDARD 99)

# 包含的头文件目录
include_directories(inc)

# 指定本地系统库文件目录（根据你的系统和安装路径进行调整）
link_directories(/usr/local/lib)

# 添加源文件
file(GLOB SRC_FILES "src/*.c")
add_executable(myapp ${SRC_FILES})

# 链接到必要的库
# 确保 paho-mqtt3c 库已经安装在 /usr/local/lib 或其他标准库目录
target_link_libraries(myapp paho-mqtt3c pthread dl)

# 启用详细的调试信息
set(CMAKE_VERBOSE_MAKEFILE ON)

# 添加 RPATH 设置
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_INSTALL_RPATH /usr/local/lib)

