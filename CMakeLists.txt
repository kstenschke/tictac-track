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

include_directories(
        "${CMAKE_SOURCE_DIR}/src"
)

# tictac-track client executable
add_executable(ttt
        src/apps/client/app/app.cc
        src/apps/client/app/app_arguments.cc
        src/apps/client/app/app_commands.cc
        src/apps/client/app/app_config.cc
        src/apps/client/app/app_error.cc
        src/apps/client/app/app_help.cc
        src/apps/client/app/app_locale.cc
        src/helper/helper_date_time.cc
        src/helper/helper_file.cc
        src/helper/helper_html.cc
        src/helper/helper_numeric.cc
        src/helper/helper_string.cc
        src/helper/helper_system.cc
        src/apps/client/report/report_browser.cc
        src/apps/client/report/report_crud.cc
        src/apps/client/report/report_date_time.cc
        src/apps/client/report/report_file.cc
        src/apps/client/report/report_html_parser.cc
        src/apps/client/report/report_recalculator.cc
        src/apps/client/report/report_renderer.cc
        src/apps/client/report/report_renderer_cli.cc
        src/apps/client/report/report_renderer_csv.cc
        src/apps/client/report/report.cc
        src/vendor/entities/decode_html_entities_utf8.cc
        src/apps/client/main.cc)

# redmine-sync executable
add_executable(ttt-redmine_sync
        src/apps/redmine-sync/app/app.cc
        src/apps/redmine-sync/app/app_config.cc
        src/apps/client/app/app_error.cc
        src/helper/helper_date_time.cc
        src/helper/helper_file.cc
        src/helper/helper_numeric.cc
        src/helper/helper_string.cc
        src/helper/helper_system.cc
        src/apps/redmine-sync/report/report_file.cc
        src/apps/redmine-sync/main.cc)