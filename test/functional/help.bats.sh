#!/usr/bin/env bats

########################################################################################################################
# Test commands recognition and display of help
########################################################################################################################

load test_helper

@test "Running without any command displays help" {
  run $BATS_TEST_DIRNAME/tsp
  [ "$status" -eq 0 ]
  [ "${lines[1]}" = "Usage: tsp <command> [args]" ]
}

@test "\"help\" and \"h\" commands display help" {
  run $BATS_TEST_DIRNAME/tsp help
  [ "$status" -eq 0 ]
  [ "${lines[1]}" = "Usage: tsp <command> [args]" ]

  run $BATS_TEST_DIRNAME/tsp h
  [ "$status" -eq 0 ]
  [ "${lines[1]}" = "Usage: tsp <command> [args]" ]
}

@test "\"help start\" (and variations) display help on start command" {
  $BATS_TEST_DIRNAME/tsp help start | grep 'start (s):'
  $BATS_TEST_DIRNAME/tsp help s | grep 'start (s):'
  $BATS_TEST_DIRNAME/tsp h start | grep 'start (s):'
  $BATS_TEST_DIRNAME/tsp h s | grep 'start (s):'
}

@test "\"help pause\" (and variations) display help on stop command" {
  $BATS_TEST_DIRNAME/tsp help pause | grep 'pause / stop (p):'
  $BATS_TEST_DIRNAME/tsp help p | grep 'pause / stop (p):'
  $BATS_TEST_DIRNAME/tsp h pause | grep 'pause / stop (p):'
  $BATS_TEST_DIRNAME/tsp h p | grep 'pause / stop (p):'
  $BATS_TEST_DIRNAME/tsp help stop | grep 'pause / stop (p):'
  $BATS_TEST_DIRNAME/tsp h stop | grep 'pause / stop (p):'
}

@test "\"help comment\" (and variations) display help on comment command" {
  $BATS_TEST_DIRNAME/tsp help comment | grep 'comment (c):'
  $BATS_TEST_DIRNAME/tsp help c | grep 'comment (c):'
  $BATS_TEST_DIRNAME/tsp h comment | grep 'comment (c):'
  $BATS_TEST_DIRNAME/tsp h c | grep 'comment (c):'
}

@test "\"help resume\" (and variations) display help on resume command" {
  $BATS_TEST_DIRNAME/tsp help resume | grep 'resume (r):'
  $BATS_TEST_DIRNAME/tsp help r | grep 'resume (r):'
  $BATS_TEST_DIRNAME/tsp h resume | grep 'resume (r):'
  $BATS_TEST_DIRNAME/tsp h r | grep 'resume (r):'
}

@test "\"help task\" (and variations) display help on task command" {
  $BATS_TEST_DIRNAME/tsp help task | grep 'task (t):'
  $BATS_TEST_DIRNAME/tsp help t | grep 'task (t):'
  $BATS_TEST_DIRNAME/tsp h task | grep 'task (t):'
  $BATS_TEST_DIRNAME/tsp h t | grep 'task (t):'
}

@test "\"help split\" (and variations) display help on split command" {
  $BATS_TEST_DIRNAME/tsp help split | grep 'split (sp):'
  $BATS_TEST_DIRNAME/tsp help sp | grep 'split (sp):'
  $BATS_TEST_DIRNAME/tsp h split | grep 'split (sp):'
  $BATS_TEST_DIRNAME/tsp h sp | grep 'split (sp):'
}

@test "\"help merge\" (and variations) display help on merge command" {
  $BATS_TEST_DIRNAME/tsp help merge | grep 'merge (m):'
  $BATS_TEST_DIRNAME/tsp help m | grep 'merge (m):'
  $BATS_TEST_DIRNAME/tsp h merge | grep 'merge (m):'
  $BATS_TEST_DIRNAME/tsp h m | grep 'merge (m):'
}

@test "\"help remove\" (and variations) display help on remove command" {
  $BATS_TEST_DIRNAME/tsp help remove | grep 'remove (rm):'
  $BATS_TEST_DIRNAME/tsp help rm | grep 'remove (rm):'
  $BATS_TEST_DIRNAME/tsp h remove | grep 'remove (rm):'
  $BATS_TEST_DIRNAME/tsp h rm | grep 'remove (rm):'
}

@test "\"help undo\" (and variations) display help on undo command" {
  $BATS_TEST_DIRNAME/tsp help undo | grep 'undo (z):'
  $BATS_TEST_DIRNAME/tsp help z | grep 'undo (z):'
  $BATS_TEST_DIRNAME/tsp h undo | grep 'undo (z):'
  $BATS_TEST_DIRNAME/tsp h z | grep 'undo (z):'
}

@test "\"help recalculate\" (and variations) display help on recalculate command" {
  $BATS_TEST_DIRNAME/tsp help recalculate | grep 'recalculate (rc):'
  $BATS_TEST_DIRNAME/tsp help rc | grep 'recalculate (rc):'
  $BATS_TEST_DIRNAME/tsp h recalculate | grep 'recalculate (rc):'
  $BATS_TEST_DIRNAME/tsp h rc | grep 'recalculate (rc):'
}

@test "\"help browse\" (and variations) display help on browse command" {
  $BATS_TEST_DIRNAME/tsp help browse | grep 'browse (b):'
  $BATS_TEST_DIRNAME/tsp help b | grep 'browse (b):'
  $BATS_TEST_DIRNAME/tsp h browse | grep 'browse (b):'
  $BATS_TEST_DIRNAME/tsp h b | grep 'browse (b):'
}

@test "\"help view\" (and variations) display help on view command" {
  $BATS_TEST_DIRNAME/tsp help view | grep 'view (v):'
  $BATS_TEST_DIRNAME/tsp help v | grep 'view (v):'
  $BATS_TEST_DIRNAME/tsp h view | grep 'view (v):'
  $BATS_TEST_DIRNAME/tsp h v | grep 'view (v):'
}

@test "\"help csv\" and \"h csv\" display help on csv command" {
  $BATS_TEST_DIRNAME/tsp help csv | grep 'csv:'
  $BATS_TEST_DIRNAME/tsp h csv | grep 'csv:'
}

@test "\"help url\" (and variations) display help on url command" {
  $BATS_TEST_DIRNAME/tsp help url | grep 'url (u):'
  $BATS_TEST_DIRNAME/tsp help u | grep 'url (u):'
  $BATS_TEST_DIRNAME/tsp h url | grep 'url (u):'
  $BATS_TEST_DIRNAME/tsp h u | grep 'url (u):'
}

@test "\"help help\" (and variations) display help on help command" {
  $BATS_TEST_DIRNAME/tsp help help | grep 'help (h):'
  $BATS_TEST_DIRNAME/tsp help h | grep 'help (h):'
  $BATS_TEST_DIRNAME/tsp h help | grep 'help (h):'
  $BATS_TEST_DIRNAME/tsp h h | grep 'help (h):'
}

@test "\"help version\" (and variations) display help on version command" {
  $BATS_TEST_DIRNAME/tsp help version | grep 'version (V):'
  $BATS_TEST_DIRNAME/tsp help V | grep 'version (V):'
  $BATS_TEST_DIRNAME/tsp h version | grep 'version (V):'
  $BATS_TEST_DIRNAME/tsp h V | grep 'version (V):'
}
