#由所有的源码生成可执行文件

aux_source_directory(. SRC_LIST)

add_executable(Chatserver ${SRC_LIST})

#配置链接库
target_link_libraries(Chatserver muduo_net muduo_base pthread)