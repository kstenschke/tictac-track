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
      cp $BATS_TEST_DIRNAME/../../bin/mac/ttt $BATS_TEST_DIRNAME/ttt
    elif [ $OS == "linux" ]; then
      cp $BATS_TEST_DIRNAME/../../bin/linux/ttt $BATS_TEST_DIRNAME/ttt
    fi
}

teardown() {
  if [ -f $BATS_TEST_DIRNAME/ttt ] ; then rm $BATS_TEST_DIRNAME/ttt; fi
  if [ -f $BATS_TEST_DIRNAME/.ttt.ini ] ; then rm $BATS_TEST_DIRNAME/.ttt.ini; fi
  if [ -f $BATS_TEST_DIRNAME/timesheet.html ] ; then rm $BATS_TEST_DIRNAME/timesheet.html; fi
  if [ -f $BATS_TEST_DIRNAME/timesheet.html.bak ] ; then rm $BATS_TEST_DIRNAME/timesheet.html.bak; fi
}
