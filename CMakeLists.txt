cmake_minimum_required(VERSION 3.5)
project(ttt
        VERSION 1.4)

set(CMAKE_CXX_STANDARD 11)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/mac)
else()
    # Linux
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/linux)
endif()

include_directories(
        "${CMAKE_SOURCE_DIR}/src"
)

# tictac-track client executable
add_executable(ttt
        src/app/app.cc
        src/app/app_arguments.cc
        src/app/app_commands.cc
        src/app/app_config.cc
        src/app/app_error.cc
        src/app/app_help.cc
        src/app/app_locale.cc
        src/helper/helper_date_time.cc
        src/helper/helper_file.cc
        src/helper/helper_html.cc
        src/helper/helper_numeric.cc
        src/helper/helper_string.cc
        src/helper/helper_system.cc
        src/report/report_browser.cc
        src/report/report_crud.cc
        src/report/report_date_time.cc
        src/report/report_file.cc
        src/report/report_parser.cc
        src/report/report_recalculator.cc
        src/report/report_renderer.cc
        src/report/report_renderer_cli.cc
        src/report/report_renderer_csv.cc
        src/report/report.cc
        src/vendor/entities/decode_html_entities_utf8.cc
        src/main.cc)
