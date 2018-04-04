#!/usr/bin/env bats

########################################################################################################################
# Test remove command
########################################################################################################################

load test_helper

@test '"rm a" fails if there are no entries' {
  $BATS_TEST_DIRNAME/ttt rm a | grep 'Cannot remove: there are no entries.'
}

@test '"rm -1" fails if there are no entries' {
  $BATS_TEST_DIRNAME/ttt rm -1 | grep 'Cannot remove: there are no entries.'
}

@test 'After removing 1 entry there is 1 less in timesheet' {
  run $BATS_TEST_DIRNAME/ttt s
  run $BATS_TEST_DIRNAME/ttt p
  run $BATS_TEST_DIRNAME/ttt s
  run $BATS_TEST_DIRNAME/ttt p
  run grep -c '<td class="meta">' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 2 ]]

  $BATS_TEST_DIRNAME/ttt rm -1
  run grep -c '<td class="meta">' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 1 ]]
}

@test 'After "rm a", there are no entries in timesheet' {
  run $BATS_TEST_DIRNAME/ttt s
  run $BATS_TEST_DIRNAME/ttt p
  run $BATS_TEST_DIRNAME/ttt s
  run $BATS_TEST_DIRNAME/ttt p
  run grep -c '<td class="meta">' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 2 ]]

  $BATS_TEST_DIRNAME/ttt rm a
  run grep -c '<td class="meta">' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 0 ]]
}

@test 'Removing a non-existing entry by ID, fails w/ error message' {
  run $BATS_TEST_DIRNAME/ttt s
  run $BATS_TEST_DIRNAME/ttt p
  run $BATS_TEST_DIRNAME/ttt s
  run $BATS_TEST_DIRNAME/ttt p

  $BATS_TEST_DIRNAME/ttt rm 5 | grep 'Cannot remove entry 5'
}

@test '"rm -3" removes the last 3 entries' {
  run $BATS_TEST_DIRNAME/ttt s
  run $BATS_TEST_DIRNAME/ttt p

  run $BATS_TEST_DIRNAME/ttt s
  run $BATS_TEST_DIRNAME/ttt p

  run $BATS_TEST_DIRNAME/ttt s
  run $BATS_TEST_DIRNAME/ttt p

  run $BATS_TEST_DIRNAME/ttt s
  run $BATS_TEST_DIRNAME/ttt p

  # there are 4 entries now
  run grep -c '<td class="meta">' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 4 ]]

  run $BATS_TEST_DIRNAME/ttt rm -3

  # there is 1 entry now
  run grep -c '<td class="meta">' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 1 ]]
}

@test 'Attempting to remove more entries than available, removes all entries, timehseet stays uncorrupted' {
  run $BATS_TEST_DIRNAME/ttt s
  run $BATS_TEST_DIRNAME/ttt p
  run $BATS_TEST_DIRNAME/ttt s
  run $BATS_TEST_DIRNAME/ttt p

  # there are 2 entries now
  run grep -c '<td class="meta">' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 4 ]]

  run $BATS_TEST_DIRNAME/ttt rm -10

  # there are 0 entries now
  run grep -c '<td class="meta">' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 0 ]]

  # viewing in CLI displays 12 columns
  amount_separators=$($BATS_TEST_DIRNAME/ttt v | grep "\|" -o | wc -l | xargs)
  [[ "$amount_separators" = 11 ]]
}
