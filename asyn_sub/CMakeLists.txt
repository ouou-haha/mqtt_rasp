cmake_minimum_required(VERSION 3.10)
project(MQTTAsyncPublisher C)


# 指定C标准
set(CMAKE_C_STANDARD 99)

# 包含头文件目录
include_directories(include)

# 添加源文件
file(GLOB SRC_FILES "src/*.c")

# 添加可执行文件
add_executable(asyn_subscribe ${SRC_FILES})

# 链接到必要的库
target_link_libraries(asyn_subscribe paho-mqtt3as pthread rdkafka)

