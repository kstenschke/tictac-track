cmake_minimum_required(VERSION 3.5)
project(ttt
        VERSION 1.3)

set(CMAKE_CXX_STANDARD 11)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/mac)
else()
    # Linux
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/linux)
endif()

add_executable(ttt
        src/client/app/app.cc
        src/client/app/arguments.cc
        src/client/app/commands.cc
        src/client/app/config.cc
        src/client/app/error.cc
        src/client/app/help.cc
        src/client/app/app_locale.cc
        src/client/helper/date_time.cc
        src/client/helper/file.cc
        src/client/helper/html.cc
        src/client/helper/numeric.cc
        src/client/helper/helper_string.cc
        src/client/helper/system.cc
        src/client/report/browser.cc
        src/client/report/crud.cc
        src/client/report/date_time.cc
        src/client/report/file.cc
        src/client/report/html_parser.cc
        src/client/report/recalculator.cc
        src/client/report/renderer.cc
        src/client/report/renderer_cli.cc
        src/client/report/renderer_csv.cc
        src/client/report/report.cc
        src/vendor/decode_html_entities_utf8.cc
        src/client/main.cc)
