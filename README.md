## 数据库连接池
### 开发环境配置
---
#### 1.CentOS 7
#### 2.GCC
#### 3.CMake
#### 4.MySQL
#### 5.mysql-devel
#### 6.jsoncpp
### 如何运行在VSCode中运行
---
#### 1.MySQL建库和表
#### 2.VSCode配置
#### 3.编译
#### 4.调试
#### 5.运行
#### 6.插入5000条数据的运行结果分析
```
连接池, 多线程, 用时: 8174353854纳秒, 8174毫秒

连接池, 单线程, 用时: 18725899085纳秒, 18725毫秒

非连接池, 多线程, 用时: 52480313253纳秒, 52480毫秒

非连接池, 单线程, 用时: 144784425519纳秒, 144784毫秒
```
### 踩坑
---
#### 踩坑1：在项目中使用MySQL C API还需要安装mysql-devel
1. 刚开始只通过`yum install mysql-community-server`安装了MySQL的服务器和客户端
2. 要想使用MySQL C API，在CentOS中需要通过`yum install mysql-devel`安装。ubuntu中需要安装libmysqlclient-dev
#### 踩坑2：关于mysql-devel和jsoncpp安装之后，指定动态库文件
1. Liunx系统中：存在于/usr/lib64和/usr/local/lib64下的库文件会被自动搜寻，无需手动指定这两个目录，使用时直接指定动态库名即可。
    - mysql-devel安装完后，在/usr/lib64/mysql下有其库文件，在/usr/include/mysql下有其头文件。
    - jsoncpp安装完之后，在/usr/local/lib64/下有其库文件，在/usr/local/include/json下有其头文件
2. 对于g++命令：`g++ main.cpp src/connectionPool.cpp src/databaseConnect.cpp -Iinclude -lmysqlclient -ljsoncpp -lpthread -o main -Wall -std=c++11`
    - 编译时找不到到动态库-lmysqlclient：需要将动态库文件链接到/usr/lib64/或/usr/local/lib64/目录下，不能在其子目录下。因此要使用`ln -s /usr/lib64/mysql/libmysqlclient.so.21.2.34 /usr/lib64/libmysqlclient.so`
    - 通过CMake在编译时不需要这样做。
    - 运行时找不到动态库error while loading shared libraries: libjsoncpp.so.25: cannot open shared object file: No such file or directory：需要将jsoncpp动态库所在路径/usr/local/lib64/添加进/etc/ld.so.conf.d/jsoncpp.conf文件中，并使用`ldconfig`使其生效。
    - 通过CMake在编译时也需要这样做。
#### 踩坑3：CMakeLists.txt中关于设置Debug调试模式参数，make编译时出现指令报错as：无法识别的选项“--gdwarf-5”
1. 报错原因：在 [GCC 11: Dwarf Error](https://github.com/crosstool-ng/crosstool-ng/issues/1546) 和 [GCC 11 Release Series Changes, New Features, and Fixes](https://gcc.gnu.org/gcc-11/changes.html) 中可以看到GCC 11编译器在调试时默认-gdwarf-5为调试选项，而汇编器as需要2.35.2以上才支持-gdwarf-5选项。
2. 解决办法：
   - 法1：在设置Debug调试选项时，指定-gdwarf-2选项。
   - 法2：通过源码编译安装最新版的Binutils工具集，以此来升级as汇编器。["as: unrecognized option '--gdwarf-5'"](https://www.qiniu.com/qfans/qnso-74000991#comments)
### 参考链接
---
1. [BiliBili--基于C++11的数据库连接池](https://www.bilibili.com/video/BV1Fr4y1s7w4?p=1)
2. [BiliBili--基于VSCode和CMake实现C/C++开发 | Linux篇](https://www.bilibili.com/video/BV1fy4y1b7TC/?p=1) 
3. [Github--database_connect_pool_StarryDecade](https://github.com/StarryDecade/database_connect_pool)
4. [Github--MysqlPool_Shangyizhou](https://github.com/Shangyizhou/MysqlPool)
