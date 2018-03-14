#!/usr/bin/env bats

########################################################################################################################
# Test undo command and backup creation
########################################################################################################################

load test_helper

@test 'If there is no backup, undo fails w/ error message' {
  $BATS_TEST_DIRNAME/tsp z | grep 'Cannot undo'

  $BATS_TEST_DIRNAME/tsp s
  # Undoing also removes backup, so one cannot twice successively
  $BATS_TEST_DIRNAME/tsp z
  $BATS_TEST_DIRNAME/tsp z | grep 'Cannot undo'
}

@test 'Starting a new entry creates a backup for undo' {
  # timesheet.html does not exist
  run ls $BATS_TEST_DIRNAME/timesheet.html
  [ "$status" -eq 1 ]
  # timesheet.html.bak does not exist
  run ls $BATS_TEST_DIRNAME/timesheet.html.bak
  [ "$status" -eq 1 ]

  # Add entry, create timesheet.html and backup
  run $BATS_TEST_DIRNAME/tsp s

  # timesheet.html does exist
  run ls $BATS_TEST_DIRNAME/timesheet.html
  [ "$status" -eq 0 ]
  # timesheet.html.bak does exist
  run ls $BATS_TEST_DIRNAME/timesheet.html.bak
  [ "$status" -eq 0 ]
}

@test 'After starting a new entry, there is 1 more entry in timesheet than in the backup' {
  # Add entry, create timesheet.html
  run $BATS_TEST_DIRNAME/tsp s
  # There is 1 running entry now
  run grep -c '<td class="meta">s/' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 1 ]]

  # Add another entry
  run $BATS_TEST_DIRNAME/tsp s

  # There are two entries in timesheet.html
  run grep -c '<td class="meta">' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 2 ]]
  # There is one entry in timesheet.html.bak
  run grep -c '<td class="meta">' $BATS_TEST_DIRNAME/timesheet.html.bak
  [[ "$output" = 1 ]]
}

@test "After updating start-time of an entry, the previous time is still in the backup" {
  run $BATS_TEST_DIRNAME/tsp s
  run $BATS_TEST_DIRNAME/tsp p
  run $BATS_TEST_DIRNAME/tsp s i=0 01:23
  run $BATS_TEST_DIRNAME/tsp s i=0 02:34
  run grep -c '01:23' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 0 ]]
  run grep -c '02:34' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 1 ]]
  run grep -c '01:23' $BATS_TEST_DIRNAME/timesheet.html.bak
  [[ "$output" = 1 ]]
}

@test "After updating end-time of an entry, the previous time is still in the backup" {
  run $BATS_TEST_DIRNAME/tsp s
  run $BATS_TEST_DIRNAME/tsp p
  run $BATS_TEST_DIRNAME/tsp p i=0 01:23
  run $BATS_TEST_DIRNAME/tsp p i=0 02:34
  run grep -c '01:23' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 0 ]]
  run grep -c '02:34' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 1 ]]
  run grep -c '01:23' $BATS_TEST_DIRNAME/timesheet.html.bak
  [[ "$output" = 1 ]]
}

@test 'After removing 1 entry, there is 1 entry more in backup than in timesheet' {
  # Create 4 entries
  $BATS_TEST_DIRNAME/tsp s
  $BATS_TEST_DIRNAME/tsp s
  $BATS_TEST_DIRNAME/tsp s
  $BATS_TEST_DIRNAME/tsp s
  $BATS_TEST_DIRNAME/tsp p
  # Remove 1 entry
  $BATS_TEST_DIRNAME/tsp rm
  # There are 3 entries in timesheet.html
  amount_entries=$(cat $BATS_TEST_DIRNAME/timesheet.html | grep "<td class=\"meta\">" -o | wc -l | xargs)
  [[ "$amount_entries" = 3 ]]
  # There are 4 entries in timesheet.html.bak
  amount_entries=$(cat $BATS_TEST_DIRNAME/timesheet.html.bak | grep "<td class=\"meta\">" -o | wc -l | xargs)
  [[ "$amount_entries" = 4 ]]
}

@test 'After merging 2 entries, there is 1 entry less in backup than in timesheet' {
  run $BATS_TEST_DIRNAME/tsp s "fixed bug"
  run $BATS_TEST_DIRNAME/tsp s "tested"
  run $BATS_TEST_DIRNAME/tsp p
  # There are 2 entries in timesheet.html
  amount_entries=$(cat $BATS_TEST_DIRNAME/timesheet.html | grep "<td class=\"meta\">" -o | wc -l | xargs)
  [[ "$amount_entries" = 2 ]]
  run $BATS_TEST_DIRNAME/tsp m
  # There is 1 entry in timesheet.html
  amount_entries=$(cat $BATS_TEST_DIRNAME/timesheet.html | grep "<td class=\"meta\">" -o | wc -l | xargs)
  [[ "$amount_entries" = 1 ]]
  # There are 2 entries in timesheet.html.bak
  amount_entries=$(cat $BATS_TEST_DIRNAME/timesheet.html.bak | grep "<td class=\"meta\">" -o | wc -l | xargs)
  [[ "$amount_entries" = 2 ]]
}

@test 'After splitting 1 entry, there is 1 entry less in backup than in timesheet' {
  $BATS_TEST_DIRNAME/tsp s
  $BATS_TEST_DIRNAME/tsp p
  $BATS_TEST_DIRNAME/tsp s i=0 01:23
  $BATS_TEST_DIRNAME/tsp p i=0 03:45
  $BATS_TEST_DIRNAME/tsp sp 0 00:30

  run grep -c '<td class="meta">' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 2 ]]
  run grep -c '<td class="meta">' $BATS_TEST_DIRNAME/timesheet.html.bak
  [[ "$output" = 1 ]]
}

@test 'Undo restores a removed entry' {
  $BATS_TEST_DIRNAME/tsp s

  $BATS_TEST_DIRNAME/tsp s
  $BATS_TEST_DIRNAME/tsp p
  $BATS_TEST_DIRNAME/tsp s i=0 01:23
  $BATS_TEST_DIRNAME/tsp p i=0 03:45

  $BATS_TEST_DIRNAME/tsp rm

  run grep -c '<td class="meta">' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 1 ]]
  run grep -c '01:23' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 0 ]]

  $BATS_TEST_DIRNAME/tsp z

  run grep -c '<td class="meta">' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 2 ]]
  run grep -c '01:23' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 1 ]]
}
