cmake_minimum_required(VERSION 3.10)
project(main)

#配置编译选项 
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")


#配置头文件搜索路径
# include_directories(/usr/local/muduo/include)

#配合库文件搜索路径
# link_directories(/usr/local/muduo/lib)

# 设置需要编译的源文件列表
set(SRC_FILES muduo_server.cpp)

# 使用 aux_source_directory 命令将当前目录下的所有源文件添加到 SRC_FILES 列表中
# aux_source_directory(. SRC_FILES)

# 配置可执行文件的输出目录
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# 配置可执行文件
add_executable(server ${SRC_FILES})

#配置链接库
target_link_libraries(server muduo_net muduo_base)
