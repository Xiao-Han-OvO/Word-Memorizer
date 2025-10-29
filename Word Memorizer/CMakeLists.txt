cmake_minimum_required(VERSION 3.10)
project(WordMemorizer)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找 GTKMM
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)

# 包含目录
include_directories(${GTKMM_INCLUDE_DIRS})
include_directories(src)

# 打印调试信息
message(STATUS "当前目录: ${CMAKE_CURRENT_SOURCE_DIR}")
file(GLOB SOURCE_FILES "src/*.cpp")
message(STATUS "找到的源文件: ${SOURCE_FILES}")

# 添加可执行文件
add_executable(word_memorizer
    src/main.cpp
    src/MainWindow.cpp
    src/WordManager.cpp
    src/SettingsManager.cpp      # 新增
    src/SettingsDialog.cpp       # 新增
)

# 链接库
target_link_libraries(word_memorizer ${GTKMM_LIBRARIES})

# 编译器标志
target_compile_options(word_memorizer PRIVATE ${GTKMM_CFLAGS_OTHER})