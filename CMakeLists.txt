# https://subingwen.cn/cmake/CMake-primer/index.html
# line comment
c_make_minimum_required(VERSION 3.5)
# project name
c_project(chapter02 VERSION 1.0.0 LANGUAGES CXX)

#[[ multiple line comment
PROJECT_SOURCE_DIR宏对应的值就是我们在使用cmake命令时，后面紧跟的目录，一般是工程的根目录。
PROJECT_BINARY_DIR: 执行cmake命令时所在的目录，一般是工程的根目录下的build目录。
CMAKE_CURRENT_SOURCE_DIR: 当前处理的CMakeLists.txt所在的路径。
CMAKE_CURRENT_BINARY_DIR: target编译目录。
EXECUTABLE_OUTPUT_PATH: 可执行文件的输出目录。
LIBRARY_OUTPUT_PATH: 库文件的输出目录。
PROJECT_NAME: 工程名，使用project命令定义。
CMAKE_BINARY_DIR: 执行cmake命令的目录，一般是工程的根目录下的build目录。
CMAKE_SOURCE_DIR: 工程的根目录。
]]
# include directory
include_directories(${PROJECT_SOURCE_DIR}/include)

# source file
file(GLOB MAIN_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

# EXECUTABLE_OUTPUT_PATH
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# LIBRARY_OUTPUT_PATH
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

# custom define MICRO
add_definitions(-DDEBUG)

# set c++ standard
set(CMAKE_CXX_STANDARD 11)
# set c++ standard required
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set c++ extension
set(CMAKE_CXX_EXTENSIONS OFF)

# add library directory
link_directories("${PROJECT_SOURCE_DIR}/lib")

# add executable(target name source file list, separate by space or simecolon)
add_executable(app main.cpp)

# add library(target name library type source file list)
target_link_libraries(app pthread)