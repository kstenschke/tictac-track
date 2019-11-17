#!/usr/bin/env bats

########################################################################################################################
# Test start command
########################################################################################################################

load test_helper

@test 'If no entry is ongoing, "start" adds a new ongoing entry' {
  run $BATS_TEST_DIRNAME/ttt
  [ "$status" -eq 0 ]

  # timesheet.html exists
  ls $BATS_TEST_DIRNAME/timesheet.html
  # There is content
  cat $BATS_TEST_DIRNAME/timesheet.html | grep '<html>'
  # There is no ongoing entry
  run grep -c '<td class="meta">s/' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 0 ]]

  run $BATS_TEST_DIRNAME/ttt s
  [ "$status" -eq 0 ]
  # There is 1 ongoing entry now
  run grep -c '<td class="meta">s/' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 1 ]]
}

@test "If one entry runs, \"start\" stops it and adds a new ongoing entry " {
  run $BATS_TEST_DIRNAME/ttt s
  [ "$status" -eq 0 ]
  # There is exactly 1 ongoing entry
  run grep -c '<td class="meta">s/' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 1 ]]
  # There is no stopped entry
  run grep -c '<td class="meta">p/' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 0 ]]

  run $BATS_TEST_DIRNAME/ttt s
  [ "$status" -eq 0 ]
  # There is exactly 1 ongoing entry
  run grep -c '<td class="meta">s/' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 1 ]]
  # There is exactly 1 stopped entry
  run grep -c '<td class="meta">p/' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 1 ]]
}

@test "If there's no entries, editing \"s i=1 12:34\" fails w/ error message" {
  run $BATS_TEST_DIRNAME/ttt
  [ "$status" -eq 0 ]
  # There are no entries
  run grep -c '<td class="meta">' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 0 ]]

  # Editing fails
  $BATS_TEST_DIRNAME/ttt s i=1 12:34 | grep 'Cannot update entry 1'
  # There are no entries
  run grep -c '<td class="meta">' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 0 ]]
}

@test "If there are 3 entries, editing \"s i=5 12:34\" fails w/ error message" {
  run $BATS_TEST_DIRNAME/ttt
  [ "$status" -eq 0 ]
  # There are no entries
  run grep -c '<td class="meta">' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 0 ]]

  $BATS_TEST_DIRNAME/ttt s
  $BATS_TEST_DIRNAME/ttt s
  $BATS_TEST_DIRNAME/ttt s
  # There are 3 entries
  run grep -c '<td class="meta">' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 3 ]]

  # Editing fails
  $BATS_TEST_DIRNAME/ttt s i=5 12:34 | grep 'Cannot update entry 5'
  # There are 3 entries
  run grep -c '<td class="meta">' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 3 ]]
}

@test 'Start and stop alter the start- and end-time of an existing entry correctly' {
  # Create ongoing entry
  $BATS_TEST_DIRNAME/ttt s
  # Stop entry
  $BATS_TEST_DIRNAME/ttt p
  # Alter entry's end-time
  $BATS_TEST_DIRNAME/ttt p i=0 19:00
  # Alter entry's start-time
  $BATS_TEST_DIRNAME/ttt s i=0 18:00
  $BATS_TEST_DIRNAME/ttt v i=0 | grep '18:00 \| 19:00'
}

@test 'Starting an entry can store a comment' {
  # Create ongoing entry w/ comment
  $BATS_TEST_DIRNAME/ttt s "foo bar baz"
  $BATS_TEST_DIRNAME/ttt v i=0 | grep "foo bar baz"
}

@test 'Starting an entry can store a task-number' {
  $BATS_TEST_DIRNAME/ttt s 12345
  $BATS_TEST_DIRNAME/ttt v i=0 | grep "12345"
}

@test 'Starting an entry can store a comment and a task-number' {
  $BATS_TEST_DIRNAME/ttt s 12345 "foo bar baz"
  $BATS_TEST_DIRNAME/ttt v i=0 | grep "12345"
  $BATS_TEST_DIRNAME/ttt v i=0 | grep "foo bar baz"
}

@test 'The start-time of an existing entry can be updated w/ arguments: s i=<ID> <hh:ss>' {
  run $BATS_TEST_DIRNAME/ttt s
  run $BATS_TEST_DIRNAME/ttt p
  run $BATS_TEST_DIRNAME/ttt s
  run $BATS_TEST_DIRNAME/ttt p
  run $BATS_TEST_DIRNAME/ttt s
  run $BATS_TEST_DIRNAME/ttt p

  run $BATS_TEST_DIRNAME/ttt s i=0 01:23
  run $BATS_TEST_DIRNAME/ttt s i=1 12:34
  run $BATS_TEST_DIRNAME/ttt s i=2 34:56

  run grep -c '>01:23<' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 1 ]]

  run grep -c '>12:34<' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 1 ]]

  run grep -c '>34:56<' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 1 ]]
}

@test 'The start-time of an existing entry can be updated w/ arguments: s <ID> <hh:ss>' {
  run $BATS_TEST_DIRNAME/ttt s
  run $BATS_TEST_DIRNAME/ttt p
  run $BATS_TEST_DIRNAME/ttt s
  run $BATS_TEST_DIRNAME/ttt p
  run $BATS_TEST_DIRNAME/ttt s
  run $BATS_TEST_DIRNAME/ttt p

  run $BATS_TEST_DIRNAME/ttt s 0 01:23
  run $BATS_TEST_DIRNAME/ttt s 1 12:34
  run $BATS_TEST_DIRNAME/ttt s 2 34:56

  run grep -c '>01:23<' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 1 ]]

  run grep -c '>12:34<' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 1 ]]

  run grep -c '>34:56<' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 1 ]]
}
