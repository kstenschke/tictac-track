#!/usr/bin/env bats

########################################################################################################################
# Test stop command
########################################################################################################################

load test_helper

@test 'If no entry runs, "stop" fails w/ error message, and adds no new entry' {
  run $BATS_TEST_DIRNAME/tsp
  [ "$status" -eq 0 ]

  # There is no entry
  run grep -c '<td class="meta">/' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 0 ]]

  $BATS_TEST_DIRNAME/tsp p | grep 'Cannot stop'
  # There is no entry
  run grep -c '<td class="meta">/' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 0 ]]
}

@test 'If an entry runs, "stop" stops it, and adds no new entry' {
  run $BATS_TEST_DIRNAME/tsp s
  [ "$status" -eq 0 ]
  # There is 1 running entry
  run grep -c '<td class="meta">s/' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 1 ]]
  # There is no stopped entry
  run grep -c '<td class="meta">p/' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 0 ]]

  run $BATS_TEST_DIRNAME/tsp p
  # There is no running entry
  run grep -c '<td class="meta">s/' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 0 ]]
  # There is 1 stopped entry
  run grep -c '<td class="meta">p/' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 1 ]]
}

@test 'Stopping an entry at an earlier time than started, treats it as crossing midnight' {
  run $BATS_TEST_DIRNAME/tsp s
  run $BATS_TEST_DIRNAME/tsp p
  run $BATS_TEST_DIRNAME/tsp s i=0 23:00
  run $BATS_TEST_DIRNAME/tsp p i=0 01:23
  $BATS_TEST_DIRNAME/tsp v i=0 | grep "01:23"
}
