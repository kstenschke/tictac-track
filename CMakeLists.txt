cmake_minimum_required(VERSION 3.5)
project(ttt
        VERSION 1.6.1)

include_directories(${CMAKE_SOURCE_DIR})

set(CMAKE_CXX_STANDARD 17)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin/mac)
else()
    # Linux
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin/linux)
endif()

include_directories("${CMAKE_SOURCE_DIR}/src")

set (TTT_EXECUTABLE_NAME "ttt")
set (TTT_VERSION_MAJOR 1)
set (TTT_VERSION_MINOR 7)
set (TTT_VERSION_PATCH 0)
# Generate config.h from config.h.in, with injected CMake values
configure_file (ttt/config.h.in ../ttt/config.h)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")

# tictac-track client executable
add_executable(ttt
        ttt/class/app/app.cc
        ttt/class/app/app_arguments.cc
        ttt/class/app/app_commands.cc
        ttt/class/app/app_config.cc
        ttt/class/app/app_error.cc
        ttt/class/app/app_help.cc
        ttt/class/app/app_locale.cc
        
        ttt/helper/helper_date_time.cc
        ttt/helper/helper_file.cc
        ttt/helper/helper_html.cc
        ttt/helper/helper_numeric.cc
        ttt/helper/helper_string.cc
        ttt/helper/helper_system.cc
        ttt/helper/helper_tui.cc

        ttt/class/report/report_browser.cc
        ttt/class/report/report_crud.cc
        ttt/class/report/report_date_time.cc
        ttt/class/report/report_file.cc
        ttt/class/report/report_parser.cc
        ttt/class/report/report_recalculator.cc
        ttt/class/report/report_renderer.cc
        ttt/class/report/report_renderer_cli.cc
        ttt/class/report/report_renderer_csv.cc
        ttt/class/report/report.cc

        vendor/entities/decode_html_entities_utf8.cc
        ttt/main.cc)
