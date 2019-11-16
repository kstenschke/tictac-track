Tictac-Track - Changelog
========================

V1.5.0 - 2019/11/16
-------------------
* Added command: csvdt - Output CSV of items tracked within current day
* Added command: csrtn - Output CSV of 30 most recently tracked task numbers
* Added command: clear (cls) - Empty the timesheet 

V1.4.1 - 2019/02/26
-------------------
* Added CONTRIBUTING.md
* Changed: Use semantic versioning (https://semver.org/)
* Changed: Use sub directory src/ttt to allow sibling projects
* Improved: Move versioning info from source code into CMakeLists.txt
* Improved CLI rendering speed (reduced redundant file operations)
* Improved use of code convention: Use prefix form for all pre-increment / -decrement
* Improved Mac compatibility of localtime_r usage

V1.4 - 2019/02/22
-----------------
* Changed license to BSD-3-Clause

V1.3
----
* Added command: dayTasks (ud) - Sequentially view tasks of day and open related task URLs in browser
* Added config: Default task number for 1st entry started in day (stored if no different one given) 
* Bugfix: Splitting entry by 1 hour was formatting time as 0:60
* Changed application name to Tictac-Track (ttt)
* Improved console timesheet view: Added option to auto-clear console before printing timesheet
* Improved console timesheet view: Highlight end/start-times around breaks
* Improved insertion of full-day entries
* Improved merge: Disallow merge entries in different days
* Improved parser performance
* Improved merge of two comments: Better placing of divider and space characters
* Improved resume command: Now allows for optional comment to be added/appended

V1.2
----
* Bugfix: Parsing "c i=<number>" caused segmentation fault
* Improved removal stability: Attempting to remove more entries than available corrupted timesheet
* Improved start command: Updating start-time of existing entry now accepts ID argument w/o "i=" prefix
* Improved stop command: Updating end-time of existing entry now accepts ID argument w/o "i=" prefix
* Improved task command: Added option to also append to entry comment while updating the task number 

V1.1
----
* Bugfix: Viewing empty timesheet w/ filter was stuck in loop
* Improved CLI timesheet view: Exclamation mark HTML entity was not decoded 
* Improved comment command: Appending to existing comment allows text in double quotes to start w/ a space character
* Improved comment command: Updating/appending to existing comments now accepts ID argument w/o "i=" prefix
* Improved comment command: Updating/appending to existing comments now accepts multiple comma-separated entry IDs 
* Improved resume command: When user entered task-number instead entry ID, the last entry of that task is suggested to be resumed
* Improved task command: Updating task number now accepts multiple comma-separated entry IDs 

v1.0 - 2018/03/18
-----------------
* Initial release
