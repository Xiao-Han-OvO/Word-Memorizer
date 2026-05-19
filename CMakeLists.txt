cmake_minimum_required(VERSION 3.16)

project(WordMemorizer VERSION 3.0.0 LANGUAGES CXX)

# qt_policy(SET QTP0001 NEW)
# qt_policy(SET QTP0004 NEW)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 6.2 REQUIRED COMPONENTS Core Gui Qml Quick QuickControls2)

qt_standard_project_setup(REQUIRES 6.2)

qt_add_executable(word_memorizer
    src/main.cpp
    src/AppController.cpp
    src/AppController.h
    src/WordEngine.cpp
    src/WordEngine.h
    src/WordListModel.cpp
    src/WordListModel.h
)

qt_add_qml_module(word_memorizer
    URI WordMemorizer
    VERSION 1.0
    QML_FILES
        Main.qml
        LucideIcon.qml
)

target_link_libraries(word_memorizer
    PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Qml
        Qt6::Quick
        Qt6::QuickControls2
)

set_target_properties(word_memorizer PROPERTIES
    OUTPUT_NAME "word_memorizer"
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

install(TARGETS word_memorizer
    BUNDLE DESTINATION .
    RUNTIME DESTINATION bin
)
