#!/usr/bin/env bats

########################################################################################################################
# Test remove command
########################################################################################################################

load test_helper

@test '"cls" empties the timesheet' {
  $BATS_TEST_DIRNAME/ttt s "foo bar baz"
  $BATS_TEST_DIRNAME/ttt v i=0 | grep "foo bar baz"

  run $BATS_TEST_DIRNAME/ttt cls

  run grep -c '<td class="meta">' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 0 ]]
}

@test '"clear" empties the timesheet' {
  $BATS_TEST_DIRNAME/ttt s "foo bar baz"
  $BATS_TEST_DIRNAME/ttt v i=0 | grep "foo bar baz"

  run $BATS_TEST_DIRNAME/ttt clear

  run grep -c '<td class="meta">' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 0 ]]
}

@test '"cls" can be undone' {
  $BATS_TEST_DIRNAME/ttt s "foo bar baz"
  $BATS_TEST_DIRNAME/ttt v i=0 | grep "foo bar baz"

  run $BATS_TEST_DIRNAME/ttt cls

  run grep -c '<td class="meta">' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 0 ]]

  run $BATS_TEST_DIRNAME/ttt z
  $BATS_TEST_DIRNAME/ttt v i=0 | grep "foo bar baz"
}
