cmake_minimum_required(VERSION 3.15)
project(monitor-widget)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtk+-3.0)

add_executable(widget src/main.c)
target_link_libraries(widget PRIVATE ${GTK_LIBRARIES})
target_compile_definitions(widget PRIVATE ${GTK_CFLAGS_OTHER})
target_include_directories(widget PRIVATE ${GTK_INCLUDE_DIRS})
target_link_directories(widget PRIVATE ${GTK_LIBRARY_DIRS})
