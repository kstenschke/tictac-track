#!/usr/bin/env bats

########################################################################################################################
# Test split command
########################################################################################################################

load test_helper

@test 'If there are no entries, "split" fails w/ error message' {
  run $BATS_TEST_DIRNAME/ttt

  $BATS_TEST_DIRNAME/ttt sp 0 0:30 | grep 'Cannot split'
}

@test 'Splitting an invalid entry index, fails w/ error message' {
  run $BATS_TEST_DIRNAME/ttt s
  run $BATS_TEST_DIRNAME/ttt p

  $BATS_TEST_DIRNAME/ttt sp 1 0:10 | grep 'Cannot split'
}

@test 'Splitting an entry at a larger amount of time than it contains, fails w/ error message' {
  run $BATS_TEST_DIRNAME/ttt s
  run $BATS_TEST_DIRNAME/ttt p
  run $BATS_TEST_DIRNAME/ttt s i=0 10:00
  run $BATS_TEST_DIRNAME/ttt p i=0 10:10

  $BATS_TEST_DIRNAME/ttt sp 0 0:30 | grep 'Cannot split'
}

@test 'After splitting, there is one entry more than before' {
  run $BATS_TEST_DIRNAME/ttt s
  run $BATS_TEST_DIRNAME/ttt p
  run $BATS_TEST_DIRNAME/ttt s i=0 10:00
  run $BATS_TEST_DIRNAME/ttt p i=0 10:30
  $BATS_TEST_DIRNAME/ttt sp 0 0:15
  run grep -c '<td class="meta">' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 2 ]]
}

@test 'Splitting an ongoing entry fails w/ an error message' {
  run $BATS_TEST_DIRNAME/ttt s

  $BATS_TEST_DIRNAME/ttt sp 0 0:10 | grep 'Cannot split'
}
