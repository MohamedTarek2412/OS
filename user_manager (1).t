.\" Man page for user_manager
.TH USER_MANAGER 1 "May 08, 2025" "1.0" "User Manager Manual"
.SH NAME
user_manager \- A simple user and group management utility
.SH SYNOPSIS
.B user_manager
.SH DESCRIPTION
.B user_manager
is a command-line tool to manage users and groups. It supports adding/deleting users and groups, updating user and account information, and assigning users to groups.
.SH OPTIONS
None. The program uses an interactive menu.
.SH MENU OPTIONS
.TP
.B 1. Add User
Add a user with username, name, email, password, and optional expiration date.
.TP
.B 2. Delete User
Delete a user and remove them from groups.
.TP
.B 3. Add Group
Create a new group.
.TP
.B 4. Delete Group
Delete a group and remove it from users.
.TP
.B 5. Change User Info
Update a user's name or email.
.TP
.B 6. Change Account Info
Update a user's password or expiration date.
.TP
.B 7. Assign User to Group
Assign a user to a group.
.TP
.B 8. Exit
Exit the program.
.SH FILES
.TP
.B users_data.txt
Stores user and group data in text format.
.SH AUTHOR
Written for a system administration project.
.SH BUGS
Report bugs to the project maintainer.
.SH SEE ALSO
.BR useradd (8),
.BR groupadd (8)