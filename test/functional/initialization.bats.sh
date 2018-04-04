#!/usr/bin/env bats

########################################################################################################################
# Test initialization - creation of ini and timesheet.html
########################################################################################################################

load test_helper

@test "Running creates ini-file if not there" {
  # Assert ini doesn't exist
  run ls $BATS_TEST_DIRNAME/.ttt.ini
  [ "$status" -eq 1 ]
  # Assert running the program creates ini
  run $BATS_TEST_DIRNAME/ttt
  run ls $BATS_TEST_DIRNAME/.ttt.ini
  [ "$status" -eq 0 ]
}

@test "Running creates timesheet.html file if not there" {
  # Assert timesheet doesn't exist
  run ls $BATS_TEST_DIRNAME/timesheet.html
  [ "$status" -eq 1 ]
  # Assert running the program creates timesheet
  run $BATS_TEST_DIRNAME/ttt
  run ls $BATS_TEST_DIRNAME/timesheet.html
  [ "$status" -eq 0 ]
}

@test "The report-path in a default ini is the path of the executable" {
  # Create ini
  run $BATS_TEST_DIRNAME/ttt
  # Compare report-path
  run grep ";report_path=" $BATS_TEST_DIRNAME/.ttt.ini
  [[ "$output" = *$BATS_TEST_DIRNAME* ]]
}
