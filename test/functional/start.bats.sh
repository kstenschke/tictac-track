#!/usr/bin/env bats

########################################################################################################################
# Test start command
########################################################################################################################

load test_helper

@test 'If no entry runs, "start" adds a new running entry' {
  run $BATS_TEST_DIRNAME/tsp
  [ "$status" -eq 0 ]

  # timesheet.html exists
  ls $BATS_TEST_DIRNAME/timesheet.html
  # There is content
  cat $BATS_TEST_DIRNAME/timesheet.html | grep '<html>'
  # There is no running entry
  run grep -c '<td class="meta">s/' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 0 ]]

  run $BATS_TEST_DIRNAME/tsp s
  [ "$status" -eq 0 ]
  # There is 1 running entry now
  run grep -c '<td class="meta">s/' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 1 ]]
}

@test "If one entry runs, \"start\" stops it and adds a new running entry " {
  run $BATS_TEST_DIRNAME/tsp s
  [ "$status" -eq 0 ]
  # There is exactly 1 running entry
  run grep -c '<td class="meta">s/' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 1 ]]
  # There is no stopped entry
  run grep -c '<td class="meta">p/' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 0 ]]

  run $BATS_TEST_DIRNAME/tsp s
  [ "$status" -eq 0 ]
  # There is exactly 1 running entry
  run grep -c '<td class="meta">s/' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 1 ]]
  # There is exactly 1 stopped entry
  run grep -c '<td class="meta">p/' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 1 ]]
}

@test "If there's no entries, editing \"s i=1 12:34\" fails w/ error message" {
  run $BATS_TEST_DIRNAME/tsp
  [ "$status" -eq 0 ]
  # There are no entries
  run grep -c '<td class="meta">' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 0 ]]

  # Editing fails
  $BATS_TEST_DIRNAME/tsp s i=1 12:34 | grep 'Cannot update entry 1'
  # There are no entries
  run grep -c '<td class="meta">' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 0 ]]
}

@test "If there are 3 entries, editing \"s i=5 12:34\" fails w/ error message" {
  run $BATS_TEST_DIRNAME/tsp
  [ "$status" -eq 0 ]
  # There are no entries
  run grep -c '<td class="meta">' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 0 ]]

  $BATS_TEST_DIRNAME/tsp s
  $BATS_TEST_DIRNAME/tsp s
  $BATS_TEST_DIRNAME/tsp s
  # There are 3 entries
  run grep -c '<td class="meta">' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 3 ]]

  # Editing fails
  $BATS_TEST_DIRNAME/tsp s i=5 12:34 | grep 'Cannot update entry 5'
  # There are 3 entries
  run grep -c '<td class="meta">' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 3 ]]
}

@test 'Start and stop alter the start- and end-time of an existing entry correctly' {
  # Create running entry
  $BATS_TEST_DIRNAME/tsp s
  # Stop entry
  $BATS_TEST_DIRNAME/tsp p
  # Alter entry's end-time
  $BATS_TEST_DIRNAME/tsp p i=0 19:00
  # Alter entry's start-time
  $BATS_TEST_DIRNAME/tsp s i=0 18:00
  $BATS_TEST_DIRNAME/tsp v i=0 | grep '18:00 \| 19:00'
}

@test 'Starting an entry can store a comment' {
  # Create running entry w/ comment
  $BATS_TEST_DIRNAME/tsp s "foo bar baz"
  $BATS_TEST_DIRNAME/tsp v i=0 | grep "foo bar baz"
}

@test 'Starting an entry can store a task-number' {
  $BATS_TEST_DIRNAME/tsp s 12345
  $BATS_TEST_DIRNAME/tsp v i=0 | grep "12345"
}

@test 'Starting an entry can store a comment and a task-number' {
  $BATS_TEST_DIRNAME/tsp s 12345 "foo bar baz"
  $BATS_TEST_DIRNAME/tsp v i=0 | grep "12345"
  $BATS_TEST_DIRNAME/tsp v i=0 | grep "foo bar baz"
}

@test 'The start-time of an existing entry can be updated w/ arguments: s i=<ID> <hh:ss>' {
  run $BATS_TEST_DIRNAME/tsp s
  run $BATS_TEST_DIRNAME/tsp p
  run $BATS_TEST_DIRNAME/tsp s
  run $BATS_TEST_DIRNAME/tsp p
  run $BATS_TEST_DIRNAME/tsp s
  run $BATS_TEST_DIRNAME/tsp p

  run $BATS_TEST_DIRNAME/tsp s i=0 01:23
  run $BATS_TEST_DIRNAME/tsp s i=1 12:34
  run $BATS_TEST_DIRNAME/tsp s i=2 34:56

  run grep -c '>01:23<' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 1 ]]

  run grep -c '>12:34<' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 1 ]]

  run grep -c '>34:56<' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 1 ]]
}

@test 'The start-time of an existing entry can be updated w/ arguments: s <ID> <hh:ss>' {
  run $BATS_TEST_DIRNAME/tsp s
  run $BATS_TEST_DIRNAME/tsp p
  run $BATS_TEST_DIRNAME/tsp s
  run $BATS_TEST_DIRNAME/tsp p
  run $BATS_TEST_DIRNAME/tsp s
  run $BATS_TEST_DIRNAME/tsp p

  run $BATS_TEST_DIRNAME/tsp s 0 01:23
  run $BATS_TEST_DIRNAME/tsp s 1 12:34
  run $BATS_TEST_DIRNAME/tsp s 2 34:56

  run grep -c '>01:23<' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 1 ]]

  run grep -c '>12:34<' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 1 ]]

  run grep -c '>34:56<' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 1 ]]
}
