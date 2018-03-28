TimesheetPlus - Changelog
=========================

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


v1.0
----

* Initial release
