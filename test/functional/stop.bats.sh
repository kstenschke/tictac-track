#!/usr/bin/env bats

########################################################################################################################
# Test stop command
########################################################################################################################

load test_helper

@test 'If no entry runs, "stop" fails w/ error message, and adds no new entry' {
  run $BATS_TEST_DIRNAME/ttt
  [ "$status" -eq 0 ]

  # There is no entry
  run grep -c '<td class="meta">/' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 0 ]]

  $BATS_TEST_DIRNAME/ttt p | grep 'Cannot stop'
  # There is no entry
  run grep -c '<td class="meta">/' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 0 ]]
}

@test 'If an entry runs, "stop" stops it, and adds no new entry' {
  run $BATS_TEST_DIRNAME/ttt s
  [ "$status" -eq 0 ]
  # There is 1 running entry
  run grep -c '<td class="meta">s/' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 1 ]]
  # There is no stopped entry
  run grep -c '<td class="meta">p/' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 0 ]]

  run $BATS_TEST_DIRNAME/ttt p
  # There is no running entry
  run grep -c '<td class="meta">s/' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 0 ]]
  # There is 1 stopped entry
  run grep -c '<td class="meta">p/' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 1 ]]
}

@test 'Stopping an entry at an earlier time than started, treats it as crossing midnight' {
  run $BATS_TEST_DIRNAME/ttt s
  run $BATS_TEST_DIRNAME/ttt p
  run $BATS_TEST_DIRNAME/ttt s i=0 23:00
  run $BATS_TEST_DIRNAME/ttt p i=0 01:23
  $BATS_TEST_DIRNAME/ttt v i=0 | grep "01:23"
}

@test 'The end-time of an existing entry can be updated w/ arguments: p i=<ID> <hh:ss>' {
  run $BATS_TEST_DIRNAME/ttt s
  run $BATS_TEST_DIRNAME/ttt p
  run $BATS_TEST_DIRNAME/ttt s
  run $BATS_TEST_DIRNAME/ttt p
  run $BATS_TEST_DIRNAME/ttt s
  run $BATS_TEST_DIRNAME/ttt p

  run $BATS_TEST_DIRNAME/ttt p i=0 01:23
  run $BATS_TEST_DIRNAME/ttt p i=1 12:34
  run $BATS_TEST_DIRNAME/ttt p i=2 34:56

  run grep -c '>01:23<' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 1 ]]

  run grep -c '>12:34<' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 1 ]]

  run grep -c '>34:56<' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 1 ]]
}

@test 'The end-time of an existing entry can be updated w/ arguments: p <ID> <hh:ss>' {
  run $BATS_TEST_DIRNAME/ttt s
  run $BATS_TEST_DIRNAME/ttt p
  run $BATS_TEST_DIRNAME/ttt s
  run $BATS_TEST_DIRNAME/ttt p
  run $BATS_TEST_DIRNAME/ttt s
  run $BATS_TEST_DIRNAME/ttt p

  run $BATS_TEST_DIRNAME/ttt p 0 01:23
  run $BATS_TEST_DIRNAME/ttt p 1 12:34
  run $BATS_TEST_DIRNAME/ttt p 2 34:56

  run grep -c '>01:23<' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 1 ]]

  run grep -c '>12:34<' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 1 ]]

  run grep -c '>34:56<' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 1 ]]
}
