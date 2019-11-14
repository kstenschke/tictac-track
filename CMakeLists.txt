cmake_minimum_required(VERSION 3.5)
project(ttt
        VERSION 1.5.0)

set(CMAKE_CXX_STANDARD 11)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin/mac)
else()
    # Linux
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin/linux)
endif()

include_directories("${CMAKE_SOURCE_DIR}/src")

set (TTT_EXECUTABLE_NAME "ttt")
set (TTT_VERSION_MAJOR 1)
set (TTT_VERSION_MINOR 5)
set (TTT_VERSION_PATCH 0)
# Generate config.h from config.h.in, with injected CMake values
configure_file (src/ttt/config.h.in ../src/ttt/config.h)

# tictac-track client executable
add_executable(ttt
        src/ttt/class/app/app.cc
        src/ttt/class/app/app_arguments.cc
        src/ttt/class/app/app_commands.cc
        src/ttt/class/app/app_config.cc
        src/ttt/class/app/app_error.cc
        src/ttt/class/app/app_help.cc
        src/ttt/class/app/app_locale.cc
        
        src/ttt/helper/helper_date_time.cc
        src/ttt/helper/helper_file.cc
        src/ttt/helper/helper_html.cc
        src/ttt/helper/helper_numeric.cc
        src/ttt/helper/helper_string.cc
        src/ttt/helper/helper_system.cc
        src/ttt/helper/helper_tui.cc

        src/ttt/class/report/report_browser.cc
        src/ttt/class/report/report_crud.cc
        src/ttt/class/report/report_date_time.cc
        src/ttt/class/report/report_file.cc
        src/ttt/class/report/report_parser.cc
        src/ttt/class/report/report_recalculator.cc
        src/ttt/class/report/report_renderer.cc
        src/ttt/class/report/report_renderer_cli.cc
        src/ttt/class/report/report_renderer_csv.cc
        src/ttt/class/report/report.cc

        src/vendor/entities/decode_html_entities_utf8.cc
        src/ttt/main.cc)
