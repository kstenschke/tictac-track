#!/usr/bin/env bats

########################################################################################################################
# Test comment command
########################################################################################################################

load test_helper

@test 'Commenting an invalid entry index fails w/ error message' {
  $BATS_TEST_DIRNAME/tsp c "foo bar baz" | grep 'Cannot'
  $BATS_TEST_DIRNAME/tsp c i=0 "foo bar baz" | grep 'Cannot'
}

@test 'A comment can be added to an existing entry' {
  # Create running entry
  $BATS_TEST_DIRNAME/tsp s
  # Stop entry
  $BATS_TEST_DIRNAME/tsp p
  # Alter entry's comment
  $BATS_TEST_DIRNAME/tsp c i=0 "foo bar baz"
  $BATS_TEST_DIRNAME/tsp v i=0 | grep 'foo bar baz'
}

@test 'Text can be appended to an existing comment' {
  $BATS_TEST_DIRNAME/tsp s foo
  $BATS_TEST_DIRNAME/tsp c ", bar, baz"
  $BATS_TEST_DIRNAME/tsp v i=0 | grep 'foo, bar, baz'
}

@test 'A comment can be removed from an existing entry' {
  $BATS_TEST_DIRNAME/tsp s foo
  $BATS_TEST_DIRNAME/tsp p
  $BATS_TEST_DIRNAME/tsp v i=0 | grep 'foo'
  $BATS_TEST_DIRNAME/tsp c i=0
  run grep -c 'foo' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 0 ]]
}

@test 'Multiple items comments can be set by passing comma-separated indexes' {
  # Create 6 entries commented w/ "foo"
  $BATS_TEST_DIRNAME/tsp s
  $BATS_TEST_DIRNAME/tsp p
  $BATS_TEST_DIRNAME/tsp s
  $BATS_TEST_DIRNAME/tsp p
  $BATS_TEST_DIRNAME/tsp s
  $BATS_TEST_DIRNAME/tsp p
  $BATS_TEST_DIRNAME/tsp s
  $BATS_TEST_DIRNAME/tsp p
  $BATS_TEST_DIRNAME/tsp s
  $BATS_TEST_DIRNAME/tsp p
  $BATS_TEST_DIRNAME/tsp s
  $BATS_TEST_DIRNAME/tsp p

  # There are 6 entries now
  run grep -c '<td class="meta">' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 6 ]]
  # There are 0 "foo" comments
  amount_foos=$(cat $BATS_TEST_DIRNAME/timesheet.html | grep "foo" -o | wc -l | xargs)
  [[ "$amount_foos" = 0 ]]

  # Remove multiple comments
  run $BATS_TEST_DIRNAME/tsp c i=1,3,5 foo

  # There are 6 entries now
  run grep -c '<td class="meta">' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 6 ]]
  # There are 3 "foo" comments
  amount_entries=$(cat $BATS_TEST_DIRNAME/timesheet.html | grep "foo" -o | wc -l | xargs)
  [[ "$amount_entries" = 3 ]]
}

@test 'Multiple items comments can be removed by passing comma-separated indexes' {
  # Create 6 entries commented w/ "foo"
  $BATS_TEST_DIRNAME/tsp s foo
  $BATS_TEST_DIRNAME/tsp p
  $BATS_TEST_DIRNAME/tsp s foo
  $BATS_TEST_DIRNAME/tsp p
  $BATS_TEST_DIRNAME/tsp s foo
  $BATS_TEST_DIRNAME/tsp p
  $BATS_TEST_DIRNAME/tsp s foo
  $BATS_TEST_DIRNAME/tsp p
  $BATS_TEST_DIRNAME/tsp s foo
  $BATS_TEST_DIRNAME/tsp p
  $BATS_TEST_DIRNAME/tsp s foo
  $BATS_TEST_DIRNAME/tsp p

  # There are 6 entries now
  run grep -c '<td class="meta">' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 6 ]]
  # There are 6 "foo" comments
  amount_foos=$(cat $BATS_TEST_DIRNAME/timesheet.html | grep "foo" -o | wc -l | xargs)
  [[ "$amount_foos" = 6 ]]

  # Remove multiple comments
  run $BATS_TEST_DIRNAME/tsp c i=1,3,5

  # There are 6 entries now
  run grep -c '<td class="meta">' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 6 ]]
  # There are 3 "foo" comments
  amount_entries=$(cat $BATS_TEST_DIRNAME/timesheet.html | grep "foo" -o | wc -l | xargs)
  [[ "$amount_entries" = 3 ]]
}
