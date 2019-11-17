#!/usr/bin/env bats

########################################################################################################################
# Test merge command
########################################################################################################################

load test_helper

@test 'If there are no entries, "merge" fails w/ error message' {
  run $BATS_TEST_DIRNAME/ttt

  # Merge fails
  $BATS_TEST_DIRNAME/ttt m 1 | grep 'Cannot merge'

  # There are no entries
  run grep -c '<td class="meta">/' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 0 ]]
}

@test 'After "merge" there is one entry less than before' {
  run $BATS_TEST_DIRNAME/ttt s
  run $BATS_TEST_DIRNAME/ttt s
  run $BATS_TEST_DIRNAME/ttt p
  # There are 2 entries
  run grep -c '<td class="meta">' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 2 ]]

  run $BATS_TEST_DIRNAME/ttt m 0
  # There is 1 entry
  run grep -c '<td class="meta">' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 1 ]]
}

@test '"merge" of two entries with a break inbetween fails' {
  run $BATS_TEST_DIRNAME/ttt s
  run $BATS_TEST_DIRNAME/ttt p
  run $BATS_TEST_DIRNAME/ttt s i=0 12:30
  run $BATS_TEST_DIRNAME/ttt p i=0 12:35
  run $BATS_TEST_DIRNAME/ttt s
  run $BATS_TEST_DIRNAME/ttt p
  run $BATS_TEST_DIRNAME/ttt s i=1 12:40
  run $BATS_TEST_DIRNAME/ttt p i=1 12:45
  # Merge fails
  $BATS_TEST_DIRNAME/ttt m 0 | grep 'Cannot merge: Entries have a gap of 5 minutes'
}

@test '"merge" of the last entry fails w/ an error message' {
  run $BATS_TEST_DIRNAME/ttt s
  run $BATS_TEST_DIRNAME/ttt p
  # Merge fails
  $BATS_TEST_DIRNAME/ttt m 0 | grep 'Cannot merge'
}

@test 'Merging two entries retains the comment of the 1st entry if the other has none' {
  run $BATS_TEST_DIRNAME/ttt s
  run $BATS_TEST_DIRNAME/ttt s foo
  run $BATS_TEST_DIRNAME/ttt s
  run $BATS_TEST_DIRNAME/ttt p
  run $BATS_TEST_DIRNAME/ttt m 1
  run grep -c 'foo' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 1 ]]
}

@test 'Merging two entries retains the comment of the 2nd entry if the other has none' {
  run $BATS_TEST_DIRNAME/ttt s
  run $BATS_TEST_DIRNAME/ttt s
  run $BATS_TEST_DIRNAME/ttt s foo
  run $BATS_TEST_DIRNAME/ttt p
  run $BATS_TEST_DIRNAME/ttt m 1
  run grep -c 'foo' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 1 ]]
}

@test 'Merging two entries retains the task-number of the 1st entry if the other has none' {
  run $BATS_TEST_DIRNAME/ttt s
  run $BATS_TEST_DIRNAME/ttt s 123 foo
  run $BATS_TEST_DIRNAME/ttt s bar
  run $BATS_TEST_DIRNAME/ttt p
  run $BATS_TEST_DIRNAME/ttt m 1
  run grep -c '123' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 1 ]]
}

@test 'Merging two entries retains the task-number of the 2nd entry if the other has none' {
  run $BATS_TEST_DIRNAME/ttt s
  run $BATS_TEST_DIRNAME/ttt s foo
  run $BATS_TEST_DIRNAME/ttt s 123 bar
  run $BATS_TEST_DIRNAME/ttt p
  run $BATS_TEST_DIRNAME/ttt m 1
  run grep -c '123' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 1 ]]
}

@test 'Merging two entries that have different comments, appends them' {
  run $BATS_TEST_DIRNAME/ttt s "fixed bug"
  run $BATS_TEST_DIRNAME/ttt s "tested"
  run $BATS_TEST_DIRNAME/ttt p
  run $BATS_TEST_DIRNAME/ttt m
  run grep -c 'fixed bug. tested' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 1 ]]
}

@test 'Merging two entries that have the same comment, retains it once' {
  run $BATS_TEST_DIRNAME/ttt s
  run $BATS_TEST_DIRNAME/ttt s foo
  run $BATS_TEST_DIRNAME/ttt s foo
  run $BATS_TEST_DIRNAME/ttt p
  run $BATS_TEST_DIRNAME/ttt m 1
  run grep -c 'foo' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 1 ]]
}

@test 'Merging a stopped w/ an ongoing entry, sets the merged entry ongoing' {
  run $BATS_TEST_DIRNAME/ttt s foo
  run $BATS_TEST_DIRNAME/ttt s
  # There is 1 ongoing entry
  run grep -c '<td class="meta">s/' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 1 ]]
  # Merge
  run $BATS_TEST_DIRNAME/ttt m 1
  # There is 1 ongoing entry
  run grep -c '<td class="meta">s' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 1 ]]
}
