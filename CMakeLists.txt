cmake_minimum_required(VERSION 3.16)

project(Graphs VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Найти Qt
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# Автоматическая обработка MOC, UIC, RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Исходники
set(PROJECT_SOURCES
    main.cpp
    Graphs.cpp
    GraphAlgorithms.cpp
)

# Заголовочные файлы
set(PROJECT_HEADERS
    Graphs.h
    GraphAlgorithms.h
)
# Если есть UI файлы
# set(PROJECT_SOURCES ${PROJECT_SOURCES} mainwindow.ui)

# Создать исполняемый файл
add_executable(${PROJECT_NAME}
    ${PROJECT_SOURCES}
    ${PROJECT_HEADERS}
)

# Подключить Qt
target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Для Windows: скопировать DLL при сборке
if(WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(DLL_DIR "C:/Qt/${QT_VERSION_MAJOR}/bin")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${DLL_DIR}/Qt5Core.dll"
        "${DLL_DIR}/Qt5Gui.dll"
        "${DLL_DIR}/Qt5Widgets.dll"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
endif()
