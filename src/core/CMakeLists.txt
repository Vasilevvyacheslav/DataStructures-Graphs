set(CORE_SOURCES
    Graphs.cpp
    GraphAlgorithms.cpp
)

set(CORE_HEADERS
    Graphs.h
    GraphAlgorithms.h
)

# Создаем статическую библиотеку
add_library(core_module STATIC ${CORE_SOURCES} ${CORE_HEADERS})

# Делаем заголовки доступными для других (например для main.cpp)
target_include_directories(core_module PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
