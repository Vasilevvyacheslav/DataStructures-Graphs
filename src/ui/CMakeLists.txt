# 1. Создаем библиотеку
add_library(ui_module STATIC
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    algorithmvisualizer.h algorithmvisualizer.cpp
)

# 2. Подключаем зависимости к ui
target_link_libraries(ui_module PUBLIC
    core_module
    Qt${QT_VERSION_MAJOR}::Widgets
)

# 3. Делаем заголовки из папки ui доступными для main.cpp
target_include_directories(ui_module PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Для доступа к header-ам
target_include_directories(ui_module PUBLIC
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/core
)
