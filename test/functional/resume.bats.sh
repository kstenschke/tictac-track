#!/usr/bin/env bats

########################################################################################################################
# Test resume command
########################################################################################################################

load test_helper

@test 'If there are no entries, resume fails w/ error message' {
  $BATS_TEST_DIRNAME/ttt r | grep 'Cannot resume'
  # There is still no entry
  run grep -c '<td class="meta">/' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 0 ]]
}

@test 'Resuming a running entry fails w/ error message' {
  $BATS_TEST_DIRNAME/ttt s
  $BATS_TEST_DIRNAME/ttt r | grep 'Cannot resume'
  # There is still no entry
  run grep -c '<td class="meta">/' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 0 ]]
}

@test 'Resuming a not existing entry index fails w/ error message' {
  $BATS_TEST_DIRNAME/ttt s
  $BATS_TEST_DIRNAME/ttt r 1 | grep 'Cannot resume'
  # There is still no entry
  run grep -c '<td class="meta">/' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 0 ]]
}

@test 'Resuming an existing entry starts a new one w/ the same comment' {
  $BATS_TEST_DIRNAME/ttt s "foo"
  $BATS_TEST_DIRNAME/ttt s "bar"
  $BATS_TEST_DIRNAME/ttt s "baz"
  $BATS_TEST_DIRNAME/ttt r 1
  # There are two "bar" comments
  run grep -c '>bar</td>' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 2 ]]
}

@test 'Resuming an existing entry starts a new one w/ the same task-number' {
  $BATS_TEST_DIRNAME/ttt s 123
  $BATS_TEST_DIRNAME/ttt s 456
  $BATS_TEST_DIRNAME/ttt s 789
  $BATS_TEST_DIRNAME/ttt r 1
  # There are two task 456 entries
  run grep -c '>456</td>' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 2 ]]
}

@test 'Resuming an existing entry starts a new one w/ the same comment and task-number' {
  $BATS_TEST_DIRNAME/ttt s 123 foo
  $BATS_TEST_DIRNAME/ttt s 456 bar
  $BATS_TEST_DIRNAME/ttt s 789 baz
  $BATS_TEST_DIRNAME/ttt r 1
  run grep -c '>456</td><td>bar</td>' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 2 ]]
}
