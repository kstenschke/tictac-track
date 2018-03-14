#!/usr/bin/env bats

########################################################################################################################
# Test remove command
########################################################################################################################

load test_helper

@test '"rm a" fails if there are no entries' {
  $BATS_TEST_DIRNAME/tsp rm a | grep 'Cannot remove: there are no entries.'
}

@test '"rm -1" fails if there are no entries' {
  $BATS_TEST_DIRNAME/tsp rm -1 | grep 'Cannot remove: there are no entries.'
}

@test 'After removing 1 entry there is 1 less in timesheet' {
  run $BATS_TEST_DIRNAME/tsp s
  run $BATS_TEST_DIRNAME/tsp p
  run $BATS_TEST_DIRNAME/tsp s
  run $BATS_TEST_DIRNAME/tsp p
  run grep -c '<td class="meta">' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 2 ]]

  $BATS_TEST_DIRNAME/tsp rm -1
  run grep -c '<td class="meta">' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 1 ]]
}

@test 'After "rm a", there are no entries in timesheet' {
  run $BATS_TEST_DIRNAME/tsp s
  run $BATS_TEST_DIRNAME/tsp p
  run $BATS_TEST_DIRNAME/tsp s
  run $BATS_TEST_DIRNAME/tsp p
  run grep -c '<td class="meta">' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 2 ]]

  $BATS_TEST_DIRNAME/tsp rm a
  run grep -c '<td class="meta">' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 0 ]]
}

@test 'Removing a non-existing entry by ID, fails w/ error message' {
  run $BATS_TEST_DIRNAME/tsp s
  run $BATS_TEST_DIRNAME/tsp p
  run $BATS_TEST_DIRNAME/tsp s
  run $BATS_TEST_DIRNAME/tsp p

  $BATS_TEST_DIRNAME/tsp rm 5 | grep 'Cannot remove entry 5'
}

@test '"rm -3" removes the last 3 entries' {
  run $BATS_TEST_DIRNAME/tsp s
  run $BATS_TEST_DIRNAME/tsp p

  run $BATS_TEST_DIRNAME/tsp s
  run $BATS_TEST_DIRNAME/tsp p

  run $BATS_TEST_DIRNAME/tsp s
  run $BATS_TEST_DIRNAME/tsp p

  run $BATS_TEST_DIRNAME/tsp s
  run $BATS_TEST_DIRNAME/tsp p

  # there are 4 entries now
  run grep -c '<td class="meta">' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 4 ]]

  run $BATS_TEST_DIRNAME/tsp rm -3

  # there is 1 entry now
  run grep -c '<td class="meta">' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 1 ]]
}
