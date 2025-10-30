cmake_minimum_required(VERSION 3.10)
project(WordMemorizer)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找GTKmm包
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)

include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${GTKMM_INCLUDE_DIRS})
link_directories(${GTKMM_LIBRARY_DIRS})

# 设置编译器标志
add_definitions(${GTKMM_CFLAGS_OTHER})

# 源文件列表
set(SOURCES
    src/main.cpp
    src/MainWindow.cpp
    src/WordManager.cpp
    src/SettingsManager.cpp
    src/SettingsDialog.cpp
    src/AboutDialog.cpp
    src/WordManagerDialog.cpp
    src/UsageDialog.cpp
)

# 头文件列表
set(HEADERS
    src/MainWindow.h
    src/WordManager.h
    src/SettingsManager.h
    src/SettingsDialog.h
    src/AboutDialog.h
    src/UsageDialog.h
)

# 创建可执行文件
add_executable(word_memorizer ${SOURCES} ${HEADERS})

# 链接库
target_link_libraries(word_memorizer ${GTKMM_LIBRARIES})

# 设置可执行文件的属性
set_target_properties(word_memorizer PROPERTIES
    OUTPUT_NAME "word_memorizer"
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)