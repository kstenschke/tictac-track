#!/bin/bash

########################################################################################################################
# Run functional tests written with bats - https://github.com/sstephenson/bats                                         #
#                                                                                                                      #
# Copyright (c) 2018-2019, Kay Stenschke                                                                               #
# All rights reserved.                                                                                                 #
#                                                                                                                      #
# Install bats on Linux: sudo apt-get install bats                                                                     #
# Install bats on Mac: brew install bats                                                                               #
########################################################################################################################

START_TIME=$SECONDS

printf "\033[4mTest commands recognition and display of help\033[0m\n"
bats ./test/functional/help.bats.sh

printf "\n\033[4mTest initialization\033[0m\n"
bats ./test/functional/initialization.bats.sh

printf "\n\033[4mTest clear command\033[0m\n"
bats ./test/functional/clear.bats.sh

printf "\n\033[4mTest start command\033[0m\n"
bats ./test/functional/start.bats.sh

printf "\n\033[4mTest stop command\033[0m\n"
bats ./test/functional/stop.bats.sh

printf "\n\033[4mTest resume command\033[0m\n"
bats ./test/functional/resume.bats.sh

printf "\n\033[4mTest comment command\033[0m\n"
bats ./test/functional/comment.bats.sh

printf "\n\033[4mTest task command\033[0m\n"
bats ./test/functional/task.bats.sh

printf "\n\033[4mTest merge command\033[0m\n"
bats ./test/functional/merge.bats.sh

printf "\n\033[4mTest split command\033[0m\n"
bats ./test/functional/split.bats.sh

printf "\n\033[4mTest (all-)day command\033[0m\n"
bats ./test/functional/day.bats.sh

printf "\n\033[4mTest remove command\033[0m\n"
bats ./test/functional/remove.bats.sh

printf "\n\033[4mTest undo command and backup creation\033[0m\n"
bats ./test/functional/undo-backup.bats.sh

printf "\n\033[4mTest view command\033[0m\n"
bats ./test/functional/view.bats.sh

ELAPSED_TIME=$(($SECONDS - $START_TIME))
printf "\nDone. Bats tests ran for $ELAPSED_TIME seconds.\n\n";
