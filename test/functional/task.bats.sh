#!/usr/bin/env bats

########################################################################################################################
# Test task command
########################################################################################################################

load test_helper

@test 'Updating task-number of an invalid entry index fails w/ error message' {
  $BATS_TEST_DIRNAME/tsp s 1234
  $BATS_TEST_DIRNAME/tsp t i=1 1234 | grep 'Cannot'
}

@test 'A task-number can be added to an existing entry' {
  # Create running entry
  $BATS_TEST_DIRNAME/tsp s
  # Stop entry
  $BATS_TEST_DIRNAME/tsp p
  # Alter entry's comment
  $BATS_TEST_DIRNAME/tsp t i=0 12345
  $BATS_TEST_DIRNAME/tsp v i=0 | grep '12345'
}

@test 'A task-number can be removed from an existing entry' {
  $BATS_TEST_DIRNAME/tsp s 1234
  $BATS_TEST_DIRNAME/tsp v i=0 | grep '1234'
  $BATS_TEST_DIRNAME/tsp p
  $BATS_TEST_DIRNAME/tsp t
  run grep -c '1234' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 0 ]]
}
