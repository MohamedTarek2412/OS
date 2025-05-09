.TH TASK3 1 "May 09, 2025" "1.0" "User Manager Manual"
.SH NAME
task3 \- A simple user and group management tool
.SH SYNOPSIS
.B task3
.SH DESCRIPTION
The \fBtask3\fR program provides a command-line interface to manage users and groups.
It allows adding, deleting, modifying user information, changing account settings,
and assigning users to groups. Data is stored in a file named \fIusers_data.txt\fR.
.SH OPTIONS
This program uses a menu interface with the following options:
.TP
.B 1
Add a new user with username, name, email, password, and optional expiration date.
.TP
.B 2
Delete an existing user by username.
.TP
.B 2
Add a new group by name.
.TP
.B 4
Delete an existing group by name.
.TP
.B 5
Change a user's name and/or email.
.TP
.B 6
Change a user's password and/or expiration date.
.TP
.B 7
Assign a user to a specific group.
.TP
.B 8
Exit the program.
.SH FILES
.I users_data.txt
- Stores user and group data in a plain text format.
.SH AUTHOR
Written by [Your Name].
.SH BUGS
Report bugs to [Your Email].
.SH SEE ALSO
No additional references.
