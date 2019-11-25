#!/usr/bin/env bats

########################################################################################################################
# Test view command
########################################################################################################################

load test_helper

@test 'Viewing an empty timesheet, displays 12 columns' {
  amount_separators=$($BATS_TEST_DIRNAME/ttt v | grep "|" -o | wc -l | xargs)
  [[ "$amount_separators" = 11 ]]
}

@test 'Viewing an empty timesheet w/ day filter, displays 12 columns' {
  amount_separators=$($BATS_TEST_DIRNAME/ttt v d | grep "|" -o | wc -l | xargs)
  [[ "$amount_separators" = 11 ]]
}

@test 'Viewed timesheet contains no HTML' {
  # Start an entry w/ task-number and comment
  run $BATS_TEST_DIRNAME/ttt s "foo bar baz" 123

  amount=$($BATS_TEST_DIRNAME/ttt v | grep -o "html" | wc -l | xargs)
  [[ "$amount" = 0 ]]
}

@test 'Viewed timesheet contains correct amount of entries' {
  $BATS_TEST_DIRNAME/ttt s
  $BATS_TEST_DIRNAME/ttt s
  $BATS_TEST_DIRNAME/ttt s
  $BATS_TEST_DIRNAME/ttt s
  $BATS_TEST_DIRNAME/ttt s
  $BATS_TEST_DIRNAME/ttt p

  amount_separators=$($BATS_TEST_DIRNAME/ttt v | grep "|" -o | wc -l | xargs)
  [[ "$amount_separators" -eq 66 ]]
}
