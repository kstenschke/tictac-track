Tictac-Track
============

Simple but powerful timesheet recorder for the commandline
----------------------------------------------------------

## Zero Installation, No Dependencies

* The application consists from a single executable file
* No server or database needed: Timetracking entries are recorded directly to an HTML file
* No background processes: Starting a new entry just records the current time, than tictac-track gets out of your way
* A default configuration file is created on first run


Features
--------

* Works directly from CLI, on Linux and Mac OS (not tested on Windows)
* Stores all work entries into an HTML file, ready to be printed
* CLI viewer: The timesheet can be viewed pretty-printed user-friendly formatted, directly within CLI
* No external editor needed: Entries can be edited directly from CLI (start/end-time, comment, task)
* Task-reference and comments: Timesheet entries contain start- and end-time and optionally a task-number and comment
* Search filter: The CLI viewer can use search filters (by date, week, month, task-number, comment)
* Time sum calculation: Recorded work times are additionally summed: per entry / task per day / day / filtered view / in total
* Overtime saldo: Required vs. recorded work time per day is calculated automatically
* Timesheet entries can be merged, split and deleted from the commandline
* Automatic backup / undo option: Before modifications are done, a backup of the timesheet is recorded,
  every modification command can be undone. Note: currently only one previous command can be undone.
* Terminal color themes: There are several color presets built-in to support different terminal color palettes
* Export: Timesheets can be exported to a CSV file
* External URL-linking: Referenced items can be dynamically opened in web-browser: all tasks of a day / week / month
* Timesheet can be recorded in multiple languages (de, dk, en, es, fi, fr, hu, it, lv, nl, no, pl, pt, ro, ru, sv, tr)


Commands Overview
-----------------

* For every command there is also a 1 or 2 letter shortcut.
* For every command there is a help topic with more detailed information and various usage examples.
  Example: More detailed help on the `start` (or `s`) command is displayed via: `./ttt h s`


### 1. Record work entries:

| Command          | Description
|------------------|-------------------------------------------------------------------------------
| start / s        | Start tracking new work entry
| pause / stop / p | Stop currently running entry
| resume / r       | Start new work entry at current time, w/ task and comment from previous entry


### 2. Update recorded entries:

| Command           | Description
|-------------------|-------------------------------------------------------------------------------
| start / s         | Update start-time of given entry
| stop / pause / p  | Update end-time of given entry
| day / d           | Add a full-day entry, useful to log days of e.g. absence, sickness, etc.
| comment / c       | Unset or append to comment of given entry
| task / t          | Update task-number of given entry
| split / sp        | Split given entry at given duration into two
| merge / m         | Merge given entry w/ the following entry
| remove / rm       | Remove given entry / given amout of latest entries / all entries
| undo / z          | Undo last entry creation / modification
| recalculate / rc  | Recalculate all duration sums (per entry, per task per day, per day)


### 3. View and export timesheet and tasks:

| Command           | Description
|-------------------|-------------------------------------------------------------------------------
| browse / b        | Open timesheet in web browser
| view / v          | Display timesheet in commandline
| week / w          | Display week out of timesheet in commandline
| csv               | Export timesheet to CSV file
| url / u           | Open external task URL in web browser
| dayTasks / ud     | Display tasks of day sequentially in CLI and web browser


### 4. Meta Commands:

| Command           | Description
|-------------------|-------------------------------------------------------------------------------
| date / D          | Display date at given offset of days
| help / h          | Describe usage of this program
| version / V       | Display installed version of this program


## Commands in detail

### Command: start (s) - Add new entry to timesheet report

The new entry's status is set to ongoing. If a previous entry has been ongoing when a new entry is started,
the previous entry is being stopped, with the current time being recorded as that task's end-time.
For the first entry per day, a default task number to be used when none is given, can be configured in ttt.ini.

#### Usage examples:

`s`           - Start a new entry (w/o task-no. or comment) at the current time

`s Foo`       - Start a new entry commented w/ "Foo"

`s "Foo bar"` - Start a new entry commented w/ "Foo bar"

`s Foo 123`   - Start a new entry commented w/ "Foo" and task number: 123

`s 123`       - Start task 123 w/o comment

`s 3 08:30`   - Update start-time of entry 3 to 08:30

`s i=3 08:30` - Update start-time of entry 3 to 08:30


### Command: stop / pause (p) - Stop ongoing entry, storing current time as end-time

#### Usage examples:

`p`           - End currently ongoing entry, store current time as end-time

`p Foo`       - End currently ongoing entry, append "Foo" to it's comment, store current time as end-time

`p "Foo bar"` - End currently ongoing entry, append "Foo bar" to it's comment, store current time as end-time

`p 3 08:30`   - Update end-time of entry 3 to 08:30

`p i=3 08:30` - Update end-time of entry 3 to 08:30


### Command: resume (r) - Resumes last or given entry (starts a new entry w/ same task and comment)

If the given argument is no existing ID, but there is an entry w/ such a task number, the system suggests to resume the
related entry.

#### Usage examples:

`r`       - Resume latest entry (has to be not running anymore).

`r c=foo` - Resume latest entry, append/add "foo" to/as comment.

`r 1`     - Resume entry with ID 1. Stops running entry, if any.

`r 1 foo` - Resume entry with ID 1, append/add "foo" to/as comment. Stop running entry, if any.

`r 2`     - Resume entry with ID 2. Stops running entry, if any.

`r -1`    - Resume entry 1 place before latest entry. Stops running entry, if any.

`r -2`    - Resume entry 2 places before latest entry. Stops running entry, if any.

### Command: day (d) - Add all-day entry

Adds a new entry, starting at the configured daily default starting time (`default_daily_start_time`) spanning for
the configured required hours/day (`debit_per_day`).
Useful to log full days of e.g. external work, conferences, sickness, etc.


#### Usage examples:

`d`                    - Add all-day entry at current day

`d t=123`              - Add all-day entry at current day, for task 123

`d 123`                - Add all-day entry at current day, for task 123

`d c=foo`              - Add all-day entry at current day, commented "foo"

`d 123 c=foo`          - Add all-day entry at current day, for task 123, commented "foo"

`d c="foo bar"`        - Add all-day entry at current day, commented "foo bar"

`d -1`                 - Add all-day entry at previous day

`d -2 c=foo`           - Add all-day entry two days in the past, commented "foo"

`d -3 c="foo bar"`     - Add all-day entry three days in the past, commented "foo bar"

`d -3 123 c="foo bar"` - Add all-day entry three days in the past, for task 123, commented "foo bar"


### Command: comment (c) - Appends given text to comment of given or latest timesheet entry
             
#### Usage examples:

`c`                   - Remove comment from latest entry

`c Foo`               - Append "Foo" to comment of latest entry

`c 3`                 - Remove comment from entry with ID 3

`c "Foo bar"`         - Append "Foo bar" to comment of latest entry

`c i=3`               - Remove comment from entry with ID 3

`c i=3 "Foo bar"`     - Append "Foo bar" to comment of entry with ID 3

`c 3 "Foo bar"`       - Append "Foo bar" to comment with ID 3

`c i=3,6,7`           - Remove comments from entries with IDs 3, 6 and 7

`c i=3,6,7 "Foo bar"` - Append "Foo bar" to comments of entries with IDs 3, 6 and 7


### Command: task (t) - Stores given task number to given entry/entries or latest entry
                      
#### Usage examples:
 
`t`                     - Unset the task number of the latest entry

`t 123`                 - Set the task number of the latest entry to 123

`t 123 foo`             - Set the task number of the latest entry to 123 and append "foo" to its comment

`t i=3`                 - Unset the task number of the entry with ID 3

`t i=3 123`             - Set the task number of the entry with ID 3 to 123

`t i=3,5,7`             - Unset the task number of the entries with IDs 3, 5 and 7

`t i=3,5,7 1234`        - Set the task number of entries with IDs 3, 5 and 7 to 1234

`t i=3 1234 " foo bar"` - Set the task number of the entry with ID 3 to 1234 and append " foo bar" to its comment

`t i=3,5,7 1234 foo`    - Set the task number of the entries with IDs 3, 5 and 7 to 1234 and append " foo bar" to their comments


### Command: split (sp): Splits given timesheet entry into two
           
#### Usage example:

`sp 5 0:30` - Reduce end-time of entry 5 by 0:30, insert new entry with duration of 0:30 after it


### Command: merge (m): Merges two successive entries

Only successive entries within the same day can be merged.

#### Usage examples:
 
`m 5` - Remove entry 6, set end-time of 5 to that of 6, set task to first given, merge comments if different

`m`   - Merge last two entries


### Command: remove (rm): Removes one or multiple timesheet entries
           
#### Usage examples:

`rm`     - Remove last entry

`rm 5`   - Remove entry with ID 5

`rm i=5` - Remove entry with ID 5

`rm -5`  - Remove last 5 entries

`rm a`   - Remove all entries, initialize timesheet anew


### Command: undo (z): Reverts last operation (comment, remove, start, stop, task)
           
Please note: Only one operation can be undone.


### Command: recalculate (rc): Calculates all duration sums anew (per entry, per task per day, per day)
  
Helpful after manual editing of timesheet entries in an external editor.
Recalculate also updates all localizable labels (column titles, day of week, etc) into the configured language's translation.


### Command: browse (b): Open timesheet in web browser


### Command: view (v): Displays the timesheet in the command-line
           
#### Usage examples:
 
`v`                  - Display full timesheet

`v d`                - Display entries of current day

`v d -1`             - Display entries of previous day

`v w`                - Display entries of current week

`v w -1`             - Display entries of previous week

`v t=123`            - Display entries of task 123

`v d t=123`          - Display entries of task 123 in current day

`v d -1 t=123`       - Display entries of task 123 in previous day

`v w t=123`          - Display entries of task 123 in current week

`v c=foo`            - Display entries with "foo" inside the comment

`v d c=foo`          - Display entries of current day with "foo" inside the comment

`v t=123 c=foo`      - Display entries of task 123 with "foo" inside the comment

`v d t=123 c=foo`    - Display entries of task 123 of current day with "foo" inside the comment

`v w -1 t=123 c=foo` - Display entries of task 123 of previous week with "foo" inside the comment


### Command: csv: Exports timesheet to CSV file

The CSV is named automatically and stored to the current path.


### Command: url (u): Opens configured task action URLs in web browser

#### Usage examples:
 
`u`            - Open url.default in web browser, with "#TASK#" removed

`u edit`       - Open url.edit in web browser, with "#TASK#" removed

`u 123`        - Open url.default in web browser, with "#TASK#" replaced by 123

`u d`          - Open url.default for all tasks in current day, with "#TASK#" replaced by task numbers

`u d -1`       - Open url.default for all tasks in previous day, with "#TASK#" replaced by task numbers

`u d edit`     - Open url.edit in web browser, for all tasks in current day, with "#TASK#" replaced by task numbers

`u d -1 edit`  - Open url.edit in web browser, for all tasks in previous day, with "#TASK#" replaced by task numbers

`u 123 edit`   - Open url.edit in web browser, with "#TASK#" replaced by 123


### Command: dayTasks (ud): Display tasks of day sequentially in CLI and in web browser

`ud`    - Sequentially display entries per task of current day in CLI, open rel. task-URL in browser

`ud -1` - Sequentially display entries per task of previous day in CLI, open rel. task-URL in browser


### Command: help (h): Describes usage of the program or its commands
           
#### Usage examples:

`h view`  - Display info on using the view command

`h v`     - Display info on using the view command


### Command: version (V): Displays current version number


Optional Configuration
----------------------

When run for the first time, tictac-track creates a default configuration file 
(".ttt.ini" in the path of the executable), which can than be edited. 
The following options are available:

* `report_path`: Path where the timesheet is stored (if not specified: path where the application executable is) 
* `locale`: Language for calendaric and other labels (day, date, week, etc.).
  The following languages are built-in:
  German (de), Danish (dk), English (en), Spanish (es), Finnish (fi), French (fr), Hungarian (hu), Italian (it),
  Latvian (lv), Dutch (nl), Norwegian (no), Polish (pl), Portuguese (pt), Romanian (ro), Russian (ru), Swedish (sv), Turkish (tr)
* `debit_per_day`: Required daily working time. Needed for calculation of overtime saldo.
* `default_daily_start_time`: Default daily starting time. Used for insertion of all-day entries. Format: HH:MM
* `first_task_of_day`: Task number to be used for the first entry per day, when no other task-no. is given.
* `max_mergeable_minutes_gap`: Maximum mergeable time gap between two entries, allowed to be merged. Prevents accidentally removing lunch breaks.
* Arbitrary commands for opening URLs of other project-management tools, e.g. `url.edit` / `url.log` / ...
* `clear_before_view`: Clears console before printing timesheet
* `cli_theme`: Theme for commandline timesheet viewer. There are several color themes built-in, allowing to view timesheets
  with various different color palettes in different CLI applications (bash, terminal, etc.).
* Date formatting: `format_week_of_year`, `format_day_of_week`
* `id_column`: Offset of index-column within CLI viewer


Build Instructions
------------------

* run: `./make.sh`
* or: `cmake CMakeLists.txt; make`

Running tests
-------------

In order to run tests, [Bats](https://github.com/sstephenson/bats) must be installed.

Run all tests: `./test.sh`


Changelog
---------

See: https://github.com/kstenschke/tictac-track/blob/master/CHANGELOG.md


Code Convention
---------------

The source code of tictac-track mostly follows the Google C++ Style Guide,
see: https://google.github.io/styleguide/cppguide.html


Bug Reporting and Feature Requests
----------------------------------

If you find a bug or have an enhancement request, please file an issue on the github repository.


Author and License
------------------

Written by Kay Stenschke in 2018.
Tictac-Track is licensed under the [New BSD License](http://opensource.org/licenses/BSD-3-Clause)

> Copyright (c), Kay Stenschke
> All rights reserved.
>
> Redistribution and use in source and binary forms, with or without modification,
> are permitted provided that the following conditions are met:
>
> 1. Redistributions of source code must retain the above copyright notice, this
>    list of conditions and the following disclaimer.
>
> 2. Redistributions in binary form must reproduce the above copyright notice,
>    this list of conditions and the following disclaimer in the documentation
>    and/or other materials provided with the distribution.
>
> 3. Neither the name of the copyright holder nor the names of its contributors
>    may be used to endorse or promote products derived from this software without
>    specific prior written permission.
>
> THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
> ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
> WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
> DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
> ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
> (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
>  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
> ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
> (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
> SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
