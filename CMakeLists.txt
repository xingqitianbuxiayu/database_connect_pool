########################## 项目参数 ###############################
# 指定CMake的最小版本要求为3.20
cmake_minimum_required(VERSION 3.20)
# 指定项目名称为DATABASE_CONNECT_POOL，项目支持的语言为C C++
project(DATABASE_CONNECT_POOL C CXX)

################### 指定源文件、头文件所在目录 ######################
# 搭配c_cpp_properties.json中的"compileCommands"属性使用，可以让源文件找到头文件
# SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# 指定项目中自定义的头文件目录
# 项目会自动搜索/usr/include/mysql/mysql.h和/usr/local/include/json/json.h
include_directories(${PROJECT_SOURCE_DIR}/include)
# 构建源文件列表，并存储在变量SRC中，后面生成可执行文件时，直接指定SRC即可
# 也可以使用类似set(SRC sayhello.cpp hello.cpp)
aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC_LIST)

###################### 设置编译参数 #################################
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -O2")
# add_compile_options(-Wall -std=c++11 -O2)

###################### 设置调试参数 #################################
# 设置调试模式：Debug或Release，这里我在设置为Debug时，make指令报错。
# as：无法识别的选项“--gdwarf-5”, 原因是因为as需要2.35.2才支持--gdwarf-5选项
# 而--gdwarf-5是GCC 11默认的调试选项，改为--gdwarf-2即可
SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb -gdwarf-2")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

#################### 生成可执行文件、链接动态库######################
# 编译子文件夹的CMakeLists.txt文件
# add_subdirectory(test)
# 设置可执行文件输出路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
# 指定可执行文件名称，以及使用的源文件
add_executable(noPool_multiThread ${PROJECT_SOURCE_DIR}/main.cpp ${SRC_LIST})
# 将要使用的动态库链接到可执行文件，位于/usr/local/lib64和/usr/lib64/中的会自动搜索
# jsoncpp位于/usr/local/lib64中, mysql/mysqlclient和pthread位于/usr/lib64/中
# 指定自定义的库文件的搜索路径
# target_link_directories(./lib)
target_link_libraries(noPool_multiThread jsoncpp mysqlclient pthread)