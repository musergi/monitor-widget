cmake_minimum_required(VERSION 3.15)
project(monitor-widget)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtk+-3.0)

add_executable(widget src/main.c src/meminfo.c src/meminfo.h src/cpuinfo.c src/cpuinfo.h src/mtk_ram_widget.c
        src/mtk_ram_widget.h src/mtk_user_data.c src/mtk_user_data.h src/mtk_cpu_widget.c src/mtk_cpu_widget.h
        src/mtk_thread_widget.c src/mtk_thread_widget.h src/mtk_settings.h src/mtk_settings.c)
target_link_libraries(widget PRIVATE ${GTK_LIBRARIES})
target_include_directories(widget PRIVATE ${GTK_INCLUDE_DIRS})
target_link_directories(widget PRIVATE ${GTK_LIBRARY_DIRS})
