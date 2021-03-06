#!/usr/bin/env bats

########################################################################################################################
# Test task command
########################################################################################################################

load test_helper

@test 'Updating task-number of an invalid entry index fails w/ error message' {
  $BATS_TEST_DIRNAME/ttt s 1234
  $BATS_TEST_DIRNAME/ttt t i=1 1234 | grep 'Cannot'
}

@test 'A task-number can be added to an existing entry' {
  # Create ongoing entry
  $BATS_TEST_DIRNAME/ttt s
  $BATS_TEST_DIRNAME/ttt p

  $BATS_TEST_DIRNAME/ttt t i=0 12345
  $BATS_TEST_DIRNAME/ttt v i=0 | grep '12345'
}

@test 'A task-number can be removed from an existing entry' {
  $BATS_TEST_DIRNAME/ttt s 1234
  $BATS_TEST_DIRNAME/ttt v i=0 | grep '1234'
  $BATS_TEST_DIRNAME/ttt p
  $BATS_TEST_DIRNAME/ttt t
  run grep -c '1234' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 0 ]]
}

@test 'At the same time with updating a task-number, a comment can be added to an entry' {
  # Create ongoing entry
  $BATS_TEST_DIRNAME/ttt s
  $BATS_TEST_DIRNAME/ttt p

  $BATS_TEST_DIRNAME/ttt t i=0 12345 "foo bar"

  run grep -c '>12345<' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" -eq 1 ]]

  run grep -c '>foo bar<' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" -eq 1 ]]
}

@test 'At the same time with updating the task-number of the latest entry, a comment can be added to it' {
  # Create ongoing entry
  $BATS_TEST_DIRNAME/ttt s
  $BATS_TEST_DIRNAME/ttt p

  $BATS_TEST_DIRNAME/ttt t 12345 "foo bar"

  run grep -c '>12345<' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" -eq 1 ]]

  run grep -c '>foo bar<' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" -eq 1 ]]
}
