#!/usr/bin/env bats

########################################################################################################################
# Test split command
########################################################################################################################

load test_helper

@test 'If there are no entries, "split" fails w/ error message' {
  run $BATS_TEST_DIRNAME/tsp

  $BATS_TEST_DIRNAME/tsp sp 0 0:30 | grep 'Cannot split'
}

@test 'Splitting an invalid entry index, fails w/ error message' {
  run $BATS_TEST_DIRNAME/tsp s
  run $BATS_TEST_DIRNAME/tsp p

  $BATS_TEST_DIRNAME/tsp sp 1 0:10 | grep 'Cannot split'
}

@test 'Splitting an entry at a larger amount of time than it contains, fails w/ error message' {
  run $BATS_TEST_DIRNAME/tsp s
  run $BATS_TEST_DIRNAME/tsp p
  run $BATS_TEST_DIRNAME/tsp s i=0 10:00
  run $BATS_TEST_DIRNAME/tsp p i=0 10:10

  $BATS_TEST_DIRNAME/tsp sp 0 0:30 | grep 'Cannot split'
}

@test 'After splitting, there is one entry more than before' {
  run $BATS_TEST_DIRNAME/tsp s
  run $BATS_TEST_DIRNAME/tsp p
  run $BATS_TEST_DIRNAME/tsp s i=0 10:00
  run $BATS_TEST_DIRNAME/tsp p i=0 10:30
  $BATS_TEST_DIRNAME/tsp sp 0 0:15
  run grep -c '<td class="meta">' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 2 ]]
}

@test 'Splitting a running entry fails w/ an error message' {
  run $BATS_TEST_DIRNAME/tsp s

  $BATS_TEST_DIRNAME/tsp sp 0 0:10 | grep 'Cannot split'
}
