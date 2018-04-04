#!/usr/bin/env bats

########################################################################################################################
# Test comment command
########################################################################################################################

load test_helper

@test 'Commenting an invalid entry index fails w/ error message' {
  $BATS_TEST_DIRNAME/ttt c "foo bar baz" | grep 'Cannot'
  $BATS_TEST_DIRNAME/ttt c i=0 "foo bar baz" | grep 'Cannot'
}

@test 'A comment can be added to an existing entry' {
  # Create running entry
  $BATS_TEST_DIRNAME/ttt s
  # Stop entry
  $BATS_TEST_DIRNAME/ttt p
  # Alter entry's comment
  $BATS_TEST_DIRNAME/ttt c i=0 "foo bar baz"
  $BATS_TEST_DIRNAME/ttt v i=0 | grep 'foo bar baz'
}

@test 'Text can be appended to an existing comment' {
  $BATS_TEST_DIRNAME/ttt s foo
  $BATS_TEST_DIRNAME/ttt c ", bar, baz"
  $BATS_TEST_DIRNAME/ttt v i=0 | grep 'foo, bar, baz'
}

@test 'A comment can be removed from an existing entry' {
  $BATS_TEST_DIRNAME/ttt s foo
  $BATS_TEST_DIRNAME/ttt p
  $BATS_TEST_DIRNAME/ttt v i=0 | grep 'foo'
  $BATS_TEST_DIRNAME/ttt c i=0
  run grep -c 'foo' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 0 ]]
}

@test 'Multiple items comments can be set by passing comma-separated indexes' {
  # Create 6 entries commented w/ "foo"
  $BATS_TEST_DIRNAME/ttt s
  $BATS_TEST_DIRNAME/ttt p
  $BATS_TEST_DIRNAME/ttt s
  $BATS_TEST_DIRNAME/ttt p
  $BATS_TEST_DIRNAME/ttt s
  $BATS_TEST_DIRNAME/ttt p
  $BATS_TEST_DIRNAME/ttt s
  $BATS_TEST_DIRNAME/ttt p
  $BATS_TEST_DIRNAME/ttt s
  $BATS_TEST_DIRNAME/ttt p
  $BATS_TEST_DIRNAME/ttt s
  $BATS_TEST_DIRNAME/ttt p

  # There are 6 entries now
  run grep -c '<td class="meta">' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 6 ]]
  # There are 0 "foo" comments
  amount_foos=$(cat $BATS_TEST_DIRNAME/timesheet.html | grep "foo" -o | wc -l | xargs)
  [[ "$amount_foos" = 0 ]]

  # Remove multiple comments
  run $BATS_TEST_DIRNAME/ttt c i=1,3,5 foo

  # There are 6 entries now
  run grep -c '<td class="meta">' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 6 ]]
  # There are 3 "foo" comments
  amount_entries=$(cat $BATS_TEST_DIRNAME/timesheet.html | grep "foo" -o | wc -l | xargs)
  [[ "$amount_entries" = 3 ]]
}

@test 'Multiple items comments can be appended by passing comma-separated indexes' {
  # Create 6 entries commented w/ "foo"
  $BATS_TEST_DIRNAME/ttt s a
  $BATS_TEST_DIRNAME/ttt p
  $BATS_TEST_DIRNAME/ttt s b
  $BATS_TEST_DIRNAME/ttt p
  $BATS_TEST_DIRNAME/ttt s c
  $BATS_TEST_DIRNAME/ttt p
  $BATS_TEST_DIRNAME/ttt s d
  $BATS_TEST_DIRNAME/ttt p
  $BATS_TEST_DIRNAME/ttt s e
  $BATS_TEST_DIRNAME/ttt p
  $BATS_TEST_DIRNAME/ttt s f
  $BATS_TEST_DIRNAME/ttt p

  # Append to multiple comments
  run $BATS_TEST_DIRNAME/ttt c i=1,3,5 oo

  # Check comments
  amount_boo=$(cat $BATS_TEST_DIRNAME/timesheet.html | grep "boo" -o | wc -l | xargs)
  amount_doo=$(cat $BATS_TEST_DIRNAME/timesheet.html | grep "doo" -o | wc -l | xargs)
  amount_foo=$(cat $BATS_TEST_DIRNAME/timesheet.html | grep "foo" -o | wc -l | xargs)
  [[ "$amount_boo" = 1 ]]
  [[ "$amount_doo" = 1 ]]
  [[ "$amount_foo" = 1 ]]
}

@test 'Multiple items comments can be removed by passing comma-separated indexes' {
  # Create 6 entries commented w/ "foo"
  $BATS_TEST_DIRNAME/ttt s foo
  $BATS_TEST_DIRNAME/ttt p
  $BATS_TEST_DIRNAME/ttt s foo
  $BATS_TEST_DIRNAME/ttt p
  $BATS_TEST_DIRNAME/ttt s foo
  $BATS_TEST_DIRNAME/ttt p
  $BATS_TEST_DIRNAME/ttt s foo
  $BATS_TEST_DIRNAME/ttt p
  $BATS_TEST_DIRNAME/ttt s foo
  $BATS_TEST_DIRNAME/ttt p
  $BATS_TEST_DIRNAME/ttt s foo
  $BATS_TEST_DIRNAME/ttt p

  # There are 6 entries now
  run grep -c '<td class="meta">' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 6 ]]
  # There are 6 "foo" comments
  amount_foos=$(cat $BATS_TEST_DIRNAME/timesheet.html | grep "foo" -o | wc -l | xargs)
  [[ "$amount_foos" = 6 ]]

  # Remove multiple comments
  run $BATS_TEST_DIRNAME/ttt c i=1,3,5

  # There are 6 entries now
  run grep -c '<td class="meta">' $BATS_TEST_DIRNAME/timesheet.html
  [[ "$output" = 6 ]]
  # There are 3 "foo" comments
  amount_entries=$(cat $BATS_TEST_DIRNAME/timesheet.html | grep "foo" -o | wc -l | xargs)
  [[ "$amount_entries" = 3 ]]
}

@test 'Text in double quotes w/ a leading space can be appended to a comment' {
  $BATS_TEST_DIRNAME/ttt s foo
  $BATS_TEST_DIRNAME/ttt c " bar"
  $BATS_TEST_DIRNAME/ttt c i=0 " baz"
  $BATS_TEST_DIRNAME/ttt v i=0 | grep 'foo bar baz'
}

@test 'Text in double quotes cannot append more than 1 leading space to a comment' {
  $BATS_TEST_DIRNAME/ttt s foo
  $BATS_TEST_DIRNAME/ttt c "      bar"
  $BATS_TEST_DIRNAME/ttt v i=0 | grep 'foo bar'
}

@test 'Text in double quotes cannot append trailing space to a comment' {
  $BATS_TEST_DIRNAME/ttt s foo
  $BATS_TEST_DIRNAME/ttt c " bar   "
  $BATS_TEST_DIRNAME/ttt v i=0 | grep 'foo bar'
  amount_entries=$(cat $BATS_TEST_DIRNAME/timesheet.html | grep "foo bar " -o | wc -l | xargs)
  [[ "$amount_entries" = 0 ]]

  $BATS_TEST_DIRNAME/ttt c " "
  amount_entries=$(cat $BATS_TEST_DIRNAME/timesheet.html | grep "foo bar " -o | wc -l | xargs)
  [[ "$amount_entries" = 0 ]]
}
