#由所有的源码生成可执行文件 .只会加载当前目录 不会递归加载子目录
aux_source_directory(. SRC_LIST)
aux_source_directory(./db DB_LIST)

# 指定可执行文件的名称 
add_executable(Chatserver ${SRC_LIST} ${DB_LIST})

#配置链接库
target_link_libraries(Chatserver muduo_net muduo_base mysqlclient pthread)