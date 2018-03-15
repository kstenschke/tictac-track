#!/usr/bin/env bash

case "$OSTYPE" in
  bsd*)     OS="bsd" ;;
  darwin*)  OS="osx" ;;
  linux*)   OS="linux" ;;
  solaris*) OS="solaris" ;;
  *)        OS="unknown: $OSTYPE" ;;
esac

setup() {
    if [ "$OS" == "osx" ]; then
      cp $BATS_TEST_DIRNAME/../../bin/mac/tsp $BATS_TEST_DIRNAME/tsp
    elif [ $OS == "linux" ]; then
      cp $BATS_TEST_DIRNAME/../../bin/linux/tsp $BATS_TEST_DIRNAME/tsp
    fi
}

teardown() {
  if [ -f $BATS_TEST_DIRNAME/tsp ] ; then rm $BATS_TEST_DIRNAME/tsp; fi
  if [ -f $BATS_TEST_DIRNAME/.timesheetplus.ini ] ; then rm $BATS_TEST_DIRNAME/.timesheetplus.ini; fi
  if [ -f $BATS_TEST_DIRNAME/timesheet.html ] ; then rm $BATS_TEST_DIRNAME/timesheet.html; fi
  if [ -f $BATS_TEST_DIRNAME/timesheet.html.bak ] ; then rm $BATS_TEST_DIRNAME/timesheet.html.bak; fi
}
