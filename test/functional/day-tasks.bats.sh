#!/usr/bin/env bats

########################################################################################################################
# Test full-day command
########################################################################################################################

load test_helper

@test 'When there are no entries, "ud" fails' {
  run $BATS_TEST_DIRNAME/ttt ud
  [[ $status == 0 ]]
}

@test 'When there are no entries, "ud -1" fails' {
  run $BATS_TEST_DIRNAME/ttt ud -1
  [[ $status == 0 ]]
}
