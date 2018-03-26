cmake_minimum_required(VERSION 3.5)
project(tsp
        VERSION 1.2)

set(CMAKE_CXX_STANDARD 11)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/mac)
else()
    # Linux
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/linux)
endif()

add_executable(tsp
        src/app/app.cc
        src/app/arguments.cc
        src/app/commands.cc
        src/app/config.cc
        src/app/error.cc
        src/app/help.cc
        src/app/app_locale.cc
        src/helper/date_time.cc
        src/helper/file.cc
        src/helper/html.cc
        src/helper/numeric.cc
        src/helper/helper_string.cc
        src/helper/system.cc
        src/report/browser.cc
        src/report/crud.cc
        src/report/date_time.cc
        src/report/file.cc
        src/report/html_parser.cc
        src/report/recalculator.cc
        src/report/renderer.cc
        src/report/renderer_cli.cc
        src/report/renderer_csv.cc
        src/report/report.cc
        src/vendor/decode_html_entities_utf8.cc
        src/main.cc)
