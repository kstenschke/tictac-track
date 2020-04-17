/*
  Copyright (c) 2018-2020, Kay Stenschke
  All rights reserved.
  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:
   * Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.
   * Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.
   * Neither the name of  nor the names of its contributors may be used to
     endorse or promote products derived from this software without specific
     prior written permission.
  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE.
*/

#include <ttt/class/app/app_help.h>

namespace tictac_track {

// Print version information
void AppHelp::PrintVersion() {
  std::cout
    << App::kAppName
    << " v" << TTT_VERSION_MAJOR << "."
    << TTT_VERSION_MINOR << "."
    << TTT_VERSION_PATCH;
}

// Print usage instructions
bool AppHelp::PrintHelp(bool withTitle, AppCommand::Commands command) {
  switch (command) {
    case AppCommand::Commands::Command_BrowseTimesheet:
      return PrintHelpOnBrowse();
    case AppCommand::Commands::Command_Comment:return PrintHelpOnComment();
    case AppCommand::Commands::Command_Csv:return PrintHelpOnCsv();
    case AppCommand::Commands::Command_Day:return PrintHelpOnDay();
    case AppCommand::Commands::Command_DisplayCalendarWeek:
      return PrintHelpOnDisplayCalendarWeek();
    case AppCommand::Commands::Command_DisplayDate:
      return PrintHelpOnDisplayDate();
    case AppCommand::Commands::Command_BrowseTaskUrl:
      return PrintHelpOnExternalTaskUrl();
    case AppCommand::Commands::Command_Help:return PrintHelpOnHelp();
    case AppCommand::Commands::Command_Merge:return PrintHelpOnMerge();
    case AppCommand::Commands::Command_Recalculate:
      return PrintHelpOnRecalculate();
    case AppCommand::Commands::Command_Resume:return PrintHelpOnResume();
    case AppCommand::Commands::Command_Remove:return PrintHelpOnRemove();
    case AppCommand::Commands::Command_Split:return PrintHelpOnSplit();
    case AppCommand::Commands::Command_Start:return PrintHelpOnStart();
    case AppCommand::Commands::Command_Stop:return PrintHelpOnStop();
    case AppCommand::Commands::Command_Task:return PrintHelpOnTask();
    case AppCommand::Commands::Command_Undo:return PrintHelpOnUndo();
    case AppCommand::Commands::Command_Version:
      std::cout << "version (V): Displays current version number.\n";

      return true;
    case AppCommand::Commands::Command_CsvDayTracks:
      std::cout << "csvdt: Output tracked items of current day as CSV.\n";

      return true;
    case AppCommand::Commands::Command_CsvRecentTaskNumbers:
      std::cout << "csvrtn: Output recent (up to) 50 tracked task numbers.\n";

      return true;
    case AppCommand::Commands::Command_View:return PrintHelpOnView();
    case AppCommand::Commands::Command_BrowseDayTasks:
      return PrintHelpOnBrowseDayTasks();
    case AppCommand::Commands::Command_ViewWeek:return PrintHelpOnWeek();
    case AppCommand::Commands::Command_Invalid:
    default:if (withTitle) PrintVersion();

      PrintOverview();
  }

  std::cout << "\n\n";

  return true;
}

void AppHelp::PrintOverview() {
  std::string exe = std::string(App::kAppExecutableName);

  std::cout
    << "\nUsage: " << exe << " <command> [args]"
    << "\nType '" << exe << " help <command>' "
       "for detailed help w/ usage examples for a specific command."
    << "\n"
    << "\nAvailable commands:"
    << "\n  1. Record work entries:"
    << "\n    start (s)         - Start tracking new work entry"
    << "\n    stop / pause (p)  - Stop currently ongoing entry"
    << "\n    resume (r)        - "
       "Start new work entry at current time, "
       "w/ task and comment from previous entry"
    << "\n    day (d)           - "
       "Add a full-day entry, "
       "useful to log days of e.g. absence, sickness, etc."
    << "\n"
    << "\n  2. Update recorded entries:"
    << "\n    start (s)         - Update start-time of given entry"
    << "\n    pause / stop (p)  - Set end-time of given or ongoing entry"
    << "\n    comment (c)       - "
       "Unset or append to comment of given entry/entries or latest entry"
    << "\n    task (t)          - "
       "Update task-number of given entry/entries or latest entry"
    << "\n    split (sp)        - "
       "Split given entry at given duration into two"
    << "\n    merge (m)         - Merge given entry w/ the following entry"
    << "\n    remove (rm)       - "
       "Remove given entry / given amout of latest entries / all entries"
    << "\n    undo (z)          - Undo last entry creation / modification"
    << "\n    recalculate (rc)  - "
       "Recalculate all duration sums (per entry, per task per day, per day)"
    << "\n    clear (cls)       - Empty the timesheet"
    << "\n"
    << "\n  3. View and export timesheet and tasks:"
    << "\n    browse (b)       - Open timesheet in web browser"
    << "\n    view (v)         - Display timesheet in commandline"
    << "\n    week (w)         - Display week out of timesheet in commandline"
    << "\n    csv              - Export timesheet to CSV file"
    << "\n    csvdt            - Output tracked items of current day as CSV"
    << "\n    csvrtn           - Output recent 30 tracked task numbers"
    << "\n    url (u)          - Open external task URL in web browser"
    << "\n    dayTasks (ud)    - "
       "Display tasks of day sequentially in CLI and web browser"
    << "\n"
    << "\n  4. Meta commands:"
    << "\n    calendarweek (W) - "
       "Display number of current- or calendar week at given offset of weeks"
    << "\n    date (D)         - "
       "Display current- or date at given offset of days"
    << "\n    help (h)         - Describe usage of this program"
    << "\n    version (V)      - Display installed version of this program";
}

bool AppHelp::PrintHelpOnBrowse() {
  std::cout << "browse (b): Open timesheet in web browser."
            << "\n";
  return true;
}

bool AppHelp::PrintHelpOnCsv() {
  std::cout
    << "csv: Exports timesheet to CSV file. "
       "The CSV is named automatically and stored to the current path.\n";

  return true;
}

bool AppHelp::PrintHelpOnComment() {
  std::cout
    << "comment (c): "
       "Appends the given text to the comment of the given, "
       "or latest timesheet entry."
    << "\n"
    << "\nUsage example 1: c                   - "
       "Remove the comment of the latest entry"
    << "\nUsage example 2: c Foo               - "
       "Append \"Foo\" to the comment of the latest entry"
    << "\nUsage example 3: c 3                 - "
       "Remove the comment from the entry with ID 3"
    << "\nUsage example 4: c \"Foo bar\"         - "
       "Append \"Foo bar\" to the comment of the latest entry"
    << "\nUsage example 5: c i=3               - "
       "Remove the comment from the entry with ID 3"
    << "\nUsage example 6: c i=3 \"Foo bar\"     - "
       "Append \"Foo bar\" to the comment with ID 3"
    << "\nUsage example 7: c 3 \"Foo bar\"       - "
       "Append \"Foo bar\" to the comment with ID 3"
    << "\nUsage example 8: c i=3,6,7           - "
       "Remove the comments of the entries with IDs 3, 6 and 7"
    << "\nUsage example 9: c i=3,6,7 \"Foo bar\" - "
       "Append \"Foo bar\" to the comments of the entries with IDs 3, 6 and 7"
    << "\n";

  return true;
}

bool AppHelp::PrintHelpOnDisplayCalendarWeek() {
  std::cout
    << "calendarweek (W): "
       "Display number of current- or calendar week at given offset of weeks"
    << "\n"
    << "\nUsage example 1:  W    - Display number of current calendar week"
    << "\nUsage example 2:  D -1 - Display number of previous calendar week"
    << "\nUsage example 3:  D 1  - Display number of next calendar week"
    << "\n";

  return true;
}

bool AppHelp::PrintHelpOnDisplayDate() {
  std::cout
    << "date (D): Display current- or date at given offset of days"
    << "\n"
    << "\nUsage example 1:  D    - Display date of current day"
    << "\nUsage example 2:  D -1 - Display yesterday's date"
    << "\nUsage example 3:  D 1  - Display tomorrow's date"
    << "\n";

  return true;
}

bool AppHelp::PrintHelpOnDay() {
  std::cout
    << "day (d): Add an all-day entry. "
       "All-day = spanning configured required hours/day)"
    << "\n"
    << "\nUseful to log full days of e.g. external work, conferences, sickness."
    << "\n"
    << "\nUsage example 1:  d                    - "
       "Add all-day entry at current day"
    << "\nUsage example 2:  d t=123              - "
       "Add all-day entry at current day, for task 123"
    << "\nUsage example 3:  d 123                - "
       "Add all-day entry at current day, for task 123"
    << "\nUsage example 4:  d c=foo              - "
       "Add all-day entry at current day, commented \"foo\""
    << "\nUsage example 5:  d 123 c=foo          - "
       "Add all-day entry at current day, for task 123, commented \"foo\""
    << "\nUsage example 6:  d c=\"foo bar\"        - "
       "Add all-day entry at current day, commented \"foo bar\""
    << "\nUsage example 7:  d -1                 - "
       "Add all-day entry at previous day"
    << "\nUsage example 8:  d -2 c=foo           - "
       "Add all-day entry two days in the past, commented \"foo\""
    << "\nUsage example 9:  d -3 c=\"foo bar\"     - "
       "Add all-day entry three days in the past, commented \"foo bar\""
    << "\nUsage example 10: d -3 123 c=\"foo bar\" - "
       "Add all-day entry three days in the past, "
       "for task 123, commented \"foo bar\""
    << "\n";

  return true;
}

bool AppHelp::PrintHelpOnExternalTaskUrl() {
  std::cout
    << "url (u): Opens configured task action URLs in web browser."
    << "\n"
    << "\nUsage example 1: u           - "
       "Open url.default in web browser, with \"#TASK#\" removed"
    << "\nUsage example 2: u edit      - "
       "Open url.edit in web browser, with \"#TASK#\" removed"
    << "\nUsage example 3: u 123       - "
       "Open url.default in web browser, with \"#TASK#\" replaced by 123"
    << "\nUsage example 4: u d         - "
       "Open url.default for all tasks in current day, "
       "with \"#TASK#\" replaced by task numbers"
    << "\nUsage example 5: u d -1      - "
       "Open url.default for all tasks in previous day, "
       "with \"#TASK#\" replaced by task numbers"
    << "\nUsage example 6: u d edit    - "
       "Open url.edit in web browser, for all tasks in current day, "
       "with \"#TASK#\" replaced by task numbers"
    << "\nUsage example 7: u d -1 edit - "
       "Open url.edit in web browser, for all tasks in previous day, "
       "with \"#TASK#\" replaced by task numbers"
    << "\nUsage example 8: u 123 edit  - "
       "Open url.edit in web browser, with \"#TASK#\" replaced by 123"
    << "\n";

  return true;
}

bool AppHelp::PrintHelpOnHelp() {
  std::cout << "help (h): Describes usage of this program or its commands."
            << "\n"
            << "\nUsage example: h [\"command\"]"
            << "\n";
  return true;
}

bool AppHelp::PrintHelpOnMerge() {
  std::cout
    << "merge (m): Merges two successive entries."
    << "\n"
    << "\nUsage example 1: m 5 - "
       "Remove entry 6, set end-time of 5 to that of 6,"
    << " set task to first given, merge comments if different"
    << "\nUsage example 2: m   - Merge last two entries"
    << "\n";

  return true;
}

bool AppHelp::PrintHelpOnRecalculate() {
  std::cout
    << "recalculate (rc): "
       "Calculates all duration sums anew "
       "(per entry, per task per day, per day)."
    << "\n  Helpful after manual editing of timesheet entries "
       "in an external editor."
    << "\n  Recalculate also updates all localizable labels "
       "(column titles, day of week, etc) "
       "into the configured language's translation."
    << "\n";

  return true;
}

bool AppHelp::PrintHelpOnRemove() {
  std::cout
    << "remove (rm): Removes timesheet entry/entries."
    << "\n"
    << "\nUsage example 1: rm     - Remove last entry"
    << "\nUsage example 2: rm 5   - Remove entry with ID 5"
    << "\nUsage example 3: rm i=5 - Remove entry with ID 5"
    << "\nUsage example 4: rm -5  - Remove last 5 entries"
    << "\nUsage example 5: rm a   - "
       "Remove all entries, initialize timesheet anew"
    << "\n";

  return true;
}

bool AppHelp::PrintHelpOnResume() {
  std::cout
    << "resume (r): "
       "Resumes last or given entry: "
       "starts a new entry w/ same task and comment."
    << "\n"
    << "\nUsage example 1: r       - "
       "Resume latest entry (has to be not ongoing anymore)."
    << "\nUsage example 2: r c=foo - "
       "Resume latest entry, append/add \"foo\" to/as comment."
    << "\nUsage example 3: r 1     - "
       "Resume entry with ID 1. Stop ongoing entry, if any."
    << "\nUsage example 4: r 1 foo - "
       "Resume entry with ID 1, append/add \"foo\" to/as comment. "
       "Stop ongoing entry, if any."
    << "\nUsage example 5: r 2     - "
       "Resume entry with ID 2. Stop ongoing entry, if any."
    << "\nUsage example 6: r -1    - "
       "Resume entry 1 place before latest entry. Stop ongoing entry, if any."
    << "\nUsage example 7: r -2    - "
       "Resume entry 2 paces before latest entry. Stop ongoing entry, if any."
    << "\n";

  return true;
}

bool AppHelp::PrintHelpOnSplit() {
  std::cout
    << "split (sp): Splits the given entry into two."
    << "\n"
    << "\nUsage example: sp 5 0:30 - "
       "Reduce end-time of entry 5 by 0:30, "
       "insert new entry with duration of 0:30 after it"
    << "\n";

  return true;
}

bool AppHelp::PrintHelpOnStart() {
  std::cout
    << "start (s): Adds a new entry to the timesheet report.\n"
    << "\nThe new entry's status is set to ongoing. "
       "If a previous entry has been ongoing when a new entry is started,"
    << "\nthe previous entry is being stopped, "
       "with the current time being recorded as that task's end-time."
    << "\nFor the first entry per day, a default task number to be used "
       "when none is given, can be configured in ttt.ini."
    << "\n"
    << "\nUsage example 1: s           - "
       "Start a new entry (w/o task-no. or comment) at the current time"
    << "\nUsage example 2: s Foo       - Start a new entry commented w/ \"Foo\""
    << "\nUsage example 3: s \"Foo bar\" - "
       "Start a new entry commented w/ \"Foo bar\""
    << "\nUsage example 4: s Foo 123   - "
       "Start a new entry commented w/ \"Foo\" and task number: 123"
    << "\nUsage example 5: s 123       - Start task 123 w/o comment"
    << "\nUsage example 6: s 3 08:30   - Update start-time of entry 3 to 08:30"
    << "\nUsage example 7: s i=3 08:30 - Update start-time of entry 3 to 08:30"
    << "\n";

  return true;
}

bool AppHelp::PrintHelpOnStop() {
  std::cout
    << "pause / stop (p): "
       "Stops the ongoing entry, storing the current time as it's end-time."
    << "\n"
    << "\nUsage example 1: p           - "
       "End currently ongoing entry, store current time as end-time"
    << "\nUsage example 2: p Foo       - "
       "End currently ongoing entry, append \"Foo\" to it's comment, "
       "store current time as end-time"
    << "\nUsage example 3: p \"Foo bar\" - "
       "End currently ongoing entry, append \"Foo bar\" to it's comment, "
       "store current time as end-time"
    << "\nUsage example 4: p 3 08:30   - Update end-time of entry 3 to 08:30"
    << "\nUsage example 5: p i=3 08:30 - Update end-time of entry 3 to 08:30"
    << "\n";

  return true;
}

bool AppHelp::PrintHelpOnTask() {
  std::cout
    << "task (t): Stores the given task number to the given or latest entry."
    << "\n"
    << "\nUsage example 1: t                    - "
       "Unset the task number of the latest entry"
    << "\nUsage example 2: t 123                - "
       "Set the task number of the latest entry to 123"
    << "\nUsage example 3: t 123 foo            - "
       "Set the task number of the latest entry to 123 "
       "and append \"foo\" to its comment"
    << "\nUsage example 4: t i=3                - "
       "Unset the task number of the the entry with ID 3"
    << "\nUsage example 5: t i=3 123            - "
       "Set the task number of the the entry with ID 3 to 123"
    << "\nUsage example 6: t i=3,5,9            - "
       "Unset the task number of the entries with IDs 3, 5 and 9"
    << "\nUsage example 7: t i=3,5,9 123        - "
       "Set the task number of the entries with IDs 3, 5 and 9 to 123"
    << "\nUsage example 8: t i=3 123 \" foo bar\" - "
       "Set the task number of entry with ID 3 to 123 "
       "and append \" foo bar\" to its comment"
    << "\nUsage example 9: t i=3,5,7 123 foo    - "
       "Set the task number of the entries with IDs 3, 5 and 7 to 123 "
       "and append \"foo\" to their comments"
    << "\n";

  return true;
}

bool AppHelp::PrintHelpOnUndo() {
  std::cout
    << "undo (z): Reverts the last operation "
       "(comment, remove, start, stop, task)."
    << "\n\nPlease note: Only one operation can be undone."
    << "\n";

  return true;
}

bool AppHelp::PrintHelpOnView() {
  std::cout
    << "view (v): Displays the timesheet in command-line."
    << "\n"
    << "\nUsage example 1:  v                  - Display full timesheet"
    << "\nUsage example 2:  v d                - Display entries of current day"
    << "\nUsage example 3:  v d -1             - "
       "Display entries of previous day"
    << "\nUsage example 4:  v w                - "
       "Display entries of current week"
    << "\nUsage example 5:  v w -1             - "
       "Display entries of previous week"
    << "\nUsage example 6:  v t=123            - Display entries of task 123"
    << "\nUsage example 7:  v d t=123          - "
       "Display entries of task 123 in current day"
    << "\nUsage example 8:  v d -1 t=123       - "
       "Display entries of task 123 in previous day"
    << "\nUsage example 9:  v w t=123          - "
       "Display entries of task 123 in current week"
    << "\nUsage example 10: v c=foo            - "
       "Display entries with \"foo\" inside the comment"
    << "\nUsage example 11: v d c=foo          - "
       "Display entries of current day with \"foo\" inside the comment"
    << "\nUsage example 12: v t=123 c=foo      - "
       "Display entries of task 123 with \"foo\" inside the comment"
    << "\nUsage example 13: v d t=123 c=foo    - "
       "Display entries of task 123 of current day "
       "with \"foo\" inside the comment"
    << "\nUsage example 14: v w -1 t=123 c=foo - "
       "Display entries of task 123 of previous week "
       "with \"foo\" inside the comment"
    << "\n";

  return true;
}
bool AppHelp::PrintHelpOnBrowseDayTasks() {
  std::cout
    << "dayTasks (ud): "
       "Display tasks of day sequentially in CLI and in web browser."
    << "\n"
    << "\nUsage example 1: ud    - "
       "Sequentially display entries per task of current day, "
       "opening rel. task-URL in browser"
    << "\nUsage example 1: ud -1 - "
       "Sequentially display entries per task of previous day, "
       "opening rel. task-URL in browser"
    << "\n";

  return true;
}

bool AppHelp::PrintHelpOnWeek() {
  std::cout
    << "week (w): Displays week out of timesheet in command-line."
    << "\n"
    << "\nUsage example 1:  w    - Display current week of timesheet"
    << "\nUsage example 2:  w -1 - Display previous week of timesheet"
    << "\n";

  return true;
}

void AppHelp::PrintUnknownArgumentMessage(const char *arg) {
  std::cout
    << "\nUnknown argument: \"" << arg
    << "\". Possible arguments are:\n";

  PrintHelp(false, AppCommand::Commands::Command_Invalid);
}

}  // namespace tictac_track
