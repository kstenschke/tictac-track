#!/usr/bin/env bash

setup() {
  cp $BATS_TEST_DIRNAME/../../bin/mac/tsp $BATS_TEST_DIRNAME/tsp
}

teardown() {
  if [ -f $BATS_TEST_DIRNAME/tsp ] ; then rm $BATS_TEST_DIRNAME/tsp; fi
  if [ -f $BATS_TEST_DIRNAME/.timesheetplus.ini ] ; then rm $BATS_TEST_DIRNAME/.timesheetplus.ini; fi
  if [ -f $BATS_TEST_DIRNAME/timesheet.html ] ; then rm $BATS_TEST_DIRNAME/timesheet.html; fi
  if [ -f $BATS_TEST_DIRNAME/timesheet.html.bak ] ; then rm $BATS_TEST_DIRNAME/timesheet.html.bak; fi
}