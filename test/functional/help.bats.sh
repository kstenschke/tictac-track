#!/usr/bin/env bats

########################################################################################################################
# Test commands recognition and display of help
########################################################################################################################

load test_helper

@test "Running without any command displays help" {
  run $BATS_TEST_DIRNAME/ttt
  [ "$status" -eq 0 ]
  [ "${lines[1]}" = "Usage: ttt <command> [args]" ]
}

@test "\"help\" and \"h\" commands display help" {
  run $BATS_TEST_DIRNAME/ttt help
  [ "$status" -eq 0 ]
  [ "${lines[1]}" = "Usage: ttt <command> [args]" ]

  run $BATS_TEST_DIRNAME/ttt h
  [ "$status" -eq 0 ]
  [ "${lines[1]}" = "Usage: ttt <command> [args]" ]
}

@test "\"help start\" (and variations) display help on start command" {
  $BATS_TEST_DIRNAME/ttt help start | grep 'start (s):'
  $BATS_TEST_DIRNAME/ttt help s | grep 'start (s):'
  $BATS_TEST_DIRNAME/ttt h start | grep 'start (s):'
  $BATS_TEST_DIRNAME/ttt h s | grep 'start (s):'
}

@test "\"help pause\" (and variations) display help on stop command" {
  $BATS_TEST_DIRNAME/ttt help pause | grep 'pause / stop (p):'
  $BATS_TEST_DIRNAME/ttt help p | grep 'pause / stop (p):'
  $BATS_TEST_DIRNAME/ttt h pause | grep 'pause / stop (p):'
  $BATS_TEST_DIRNAME/ttt h p | grep 'pause / stop (p):'
  $BATS_TEST_DIRNAME/ttt help stop | grep 'pause / stop (p):'
  $BATS_TEST_DIRNAME/ttt h stop | grep 'pause / stop (p):'
}

@test "\"help comment\" (and variations) display help on comment command" {
  $BATS_TEST_DIRNAME/ttt help comment | grep 'comment (c):'
  $BATS_TEST_DIRNAME/ttt help c | grep 'comment (c):'
  $BATS_TEST_DIRNAME/ttt h comment | grep 'comment (c):'
  $BATS_TEST_DIRNAME/ttt h c | grep 'comment (c):'
}

@test "\"help resume\" (and variations) display help on resume command" {
  $BATS_TEST_DIRNAME/ttt help resume | grep 'resume (r):'
  $BATS_TEST_DIRNAME/ttt help r | grep 'resume (r):'
  $BATS_TEST_DIRNAME/ttt h resume | grep 'resume (r):'
  $BATS_TEST_DIRNAME/ttt h r | grep 'resume (r):'
}

@test "\"help task\" (and variations) display help on task command" {
  $BATS_TEST_DIRNAME/ttt help task | grep 'task (t):'
  $BATS_TEST_DIRNAME/ttt help t | grep 'task (t):'
  $BATS_TEST_DIRNAME/ttt h task | grep 'task (t):'
  $BATS_TEST_DIRNAME/ttt h t | grep 'task (t):'
}

@test "\"help split\" (and variations) display help on split command" {
  $BATS_TEST_DIRNAME/ttt help split | grep 'split (sp):'
  $BATS_TEST_DIRNAME/ttt help sp | grep 'split (sp):'
  $BATS_TEST_DIRNAME/ttt h split | grep 'split (sp):'
  $BATS_TEST_DIRNAME/ttt h sp | grep 'split (sp):'
}

@test "\"help merge\" (and variations) display help on merge command" {
  $BATS_TEST_DIRNAME/ttt help merge | grep 'merge (m):'
  $BATS_TEST_DIRNAME/ttt help m | grep 'merge (m):'
  $BATS_TEST_DIRNAME/ttt h merge | grep 'merge (m):'
  $BATS_TEST_DIRNAME/ttt h m | grep 'merge (m):'
}

@test "\"help remove\" (and variations) display help on remove command" {
  $BATS_TEST_DIRNAME/ttt help remove | grep 'remove (rm):'
  $BATS_TEST_DIRNAME/ttt help rm | grep 'remove (rm):'
  $BATS_TEST_DIRNAME/ttt h remove | grep 'remove (rm):'
  $BATS_TEST_DIRNAME/ttt h rm | grep 'remove (rm):'
}

@test "\"help undo\" (and variations) display help on undo command" {
  $BATS_TEST_DIRNAME/ttt help undo | grep 'undo (z):'
  $BATS_TEST_DIRNAME/ttt help z | grep 'undo (z):'
  $BATS_TEST_DIRNAME/ttt h undo | grep 'undo (z):'
  $BATS_TEST_DIRNAME/ttt h z | grep 'undo (z):'
}

@test "\"help recalculate\" (and variations) display help on recalculate command" {
  $BATS_TEST_DIRNAME/ttt help recalculate | grep 'recalculate (rc):'
  $BATS_TEST_DIRNAME/ttt help rc | grep 'recalculate (rc):'
  $BATS_TEST_DIRNAME/ttt h recalculate | grep 'recalculate (rc):'
  $BATS_TEST_DIRNAME/ttt h rc | grep 'recalculate (rc):'
}

@test "\"help browse\" (and variations) display help on browse command" {
  $BATS_TEST_DIRNAME/ttt help browse | grep 'browse (b):'
  $BATS_TEST_DIRNAME/ttt help b | grep 'browse (b):'
  $BATS_TEST_DIRNAME/ttt h browse | grep 'browse (b):'
  $BATS_TEST_DIRNAME/ttt h b | grep 'browse (b):'
}

@test "\"help view\" (and variations) display help on view command" {
  $BATS_TEST_DIRNAME/ttt help view | grep 'view (v):'
  $BATS_TEST_DIRNAME/ttt help v | grep 'view (v):'
  $BATS_TEST_DIRNAME/ttt h view | grep 'view (v):'
  $BATS_TEST_DIRNAME/ttt h v | grep 'view (v):'
}

@test "\"help csv\" and \"h csv\" display help on csv command" {
  $BATS_TEST_DIRNAME/ttt help csv | grep 'csv:'
  $BATS_TEST_DIRNAME/ttt h csv | grep 'csv:'
}

@test "\"help csvdt\" and \"h csvdt\" display help on csvdt command" {
  $BATS_TEST_DIRNAME/ttt help csvdt | grep 'csvdt:'
  $BATS_TEST_DIRNAME/ttt h csvdt | grep 'csvdt:'
}

@test "\"help csvrtn\" and \"h csvrtn\" display help on csvrtn command" {
  $BATS_TEST_DIRNAME/ttt help csvrtn | grep 'csvrtn:'
  $BATS_TEST_DIRNAME/ttt h csvrtn | grep 'csvrtn:'
}

@test "\"help url\" (and variations) display help on url command" {
  $BATS_TEST_DIRNAME/ttt help url | grep 'url (u):'
  $BATS_TEST_DIRNAME/ttt help u | grep 'url (u):'
  $BATS_TEST_DIRNAME/ttt h url | grep 'url (u):'
  $BATS_TEST_DIRNAME/ttt h u | grep 'url (u):'
}

@test "\"help help\" (and variations) display help on help command" {
  $BATS_TEST_DIRNAME/ttt help help | grep 'help (h):'
  $BATS_TEST_DIRNAME/ttt help h | grep 'help (h):'
  $BATS_TEST_DIRNAME/ttt h help | grep 'help (h):'
  $BATS_TEST_DIRNAME/ttt h h | grep 'help (h):'
}

@test "\"help version\" (and variations) display help on version command" {
  $BATS_TEST_DIRNAME/ttt help version | grep 'version (V):'
  $BATS_TEST_DIRNAME/ttt help V | grep 'version (V):'
  $BATS_TEST_DIRNAME/ttt h version | grep 'version (V):'
  $BATS_TEST_DIRNAME/ttt h V | grep 'version (V):'
}
