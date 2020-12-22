#声明要求的cmake最低版本
cmake_minimum_required (VERSION 2.8)

#声明一个cmake工程，工程名为CMakeTemplate
project (PROJECT)

# 添加c++ 11标准支持
set( CMAKE_CXX_FLAGS "-std=c++11" )

#设置编译器编译模式：
set( CMAKE_BUILD_TYPE "Debug" )

set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

#编译生成库文件
#add_library(irfusion comfunc.c post_sins_gnss.cpp)

#这条命令告诉cmake，我们想把这些源文件编译成一个叫作“irfusion”的库。在linux中，库文件分为静态库和动态库两种，静态库以.a作为后缀名，共享库以.so结尾。所有库都是一些函数打包后的集合，差别在于静态库每次被调用都会生成一个副本，而共享库则只有一个副本，更省空间。如果想生成共享库而不是静态库，只需要使用以下语句即可
add_library(irfusion_shared SHARED  one.cpp two.cpp three.cpp)


#add_executable(PROJECT one.cpp)
#target_link_libraries(PROJECT test)