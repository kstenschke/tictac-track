Tictac-Track - Changelog
========================

V1.7.0 - not released yet
-------------------------
* Modernize code to use C++ 17
* More closely follow code standards
* Improve performance, RAM usage
* Improve commands recognition: allow uppercase

V1.6.1 - 2020/03/10
-------------------
* Add config option `browser`: Web-browser to open related issues in (Linux-only, Mac uses default web browser)
* Add command `calendarweek` / `W`: Displays current- or number of calendar week at given offset  
* Add to Bats functional testing: Measure and output total duration of tests
* Add: CLI-Renderer now detects max. chars per line and displays long comments truncated w/ ellipsis 
* Add: Error message when there are no entries at date given via `ud -<days>`   
* Improve Travis CI config: Build (on Linux and also) on MacOS w/ gcc and clang, run cppcheck and Bats functional tests   
* Improve: Performance and memory usage
* Improve: Made Bats functional tests Linux/GNU grep compliant (was: MacOs)

V1.6.0 - 2019/11/22
-------------------
* Add optional safeguard (prompt) for entries to require an issue number when being stopped
* Add optional safeguard (prompt) for entries to require a comment when being stopped

V1.5.0 - 2019/11/16
-------------------
* Add command: `csvdt` - Output CSV of items tracked within current day
* Add command: `csrtn` - Output CSV of 30 most recently tracked task numbers
* Add command: `clear` / `cls` - Empty the timesheet 

V1.4.1 - 2019/02/26
-------------------
* Add CONTRIBUTING.md
* Change: Use semantic versioning (https://semver.org/)
* Change: Use sub directory src/ttt to allow sibling projects
* Improve: Move versioning info from source code into CMakeLists.txt
* Improve CLI rendering speed (reduced redundant file operations)
* Improve use of code convention: Use prefix form for all pre-increment / -decrement
* Improve Mac compatibility of localtime_r usage

V1.4 - 2019/02/22
-----------------
* Change license to BSD-3-Clause

V1.3 - 2018/04/26
-----------------
* Add command: `dayTasks` / `ud` - Sequentially view tasks of day in CLI and open related task URLs in browser
* Add config: Default task number for 1st entry started in day (stored if no different one given) 
* Bugfix: Splitting entry by 1 hour was formatting time as 0:60
* Change application name to Tictac-Track (ttt)
* Improve console timesheet view: Added option to auto-clear console before printing timesheet
* Improve console timesheet view: Highlight end/start-times around breaks
* Improve insertion of full-day entries
* Improve merge: Disallow merge entries in different days
* Improve parser performance
* Improve merge of two comments: Better placing of divider and space characters
* Improve `resume` / `r` command: Now allows for optional comment to be added/appended

V1.2 - 2018/03/27
-----------------
* Bugfix: Parsing `c i=<number>` caused segmentation fault
* Improve removal stability: Attempting to remove more entries than available corrupted timesheet
* Improve `start` / `s` command: Updating start-time of existing entry now accepts ID argument w/o "i=" prefix
* Improve `stop` / `p` command: Updating end-time of existing entry now accepts ID argument w/o "i=" prefix
* Improve `task` / `t` command: Added option to also append to entry comment while updating the task number 

V1.1 - 2018/03/19
-----------------
* Bugfix: Viewing empty timesheet w/ filter was stuck in loop
* Improve CLI timesheet view: Exclamation mark HTML entity was not decoded 
* Improve `comment` / `c` command: Appending to existing comment allows text in double quotes to start w/ a space character
* Improve `comment` / `c` command: Updating/appending to existing comments now accepts ID argument w/o "i=" prefix
* Improve `comment` / `c` command: Updating/appending to existing comments now accepts multiple comma-separated entry IDs 
* Improve `resume ` / `r` command: When user entered task-number instead entry ID, the last entry of that task is suggested to be resumed
* Improve `task` / `t` command: Updating task number now accepts multiple comma-separated entry IDs 

v1.0 - 2018/03/18
-----------------
* Initial release
