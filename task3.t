.\" Man page for task3
.TH TASK3 1 "May 08, 2025" "1.0" "User Manager Manual"
.SH NAME
task3 \- A user and group management utility
.SH SYNOPSIS
.B task3
.SH DESCRIPTION
.B task3
is a command-line and graphical utility for managing users and groups. It allows adding and deleting users and groups, changing user information, updating account details such as password expiration, and assigning users to groups.
.SH OPTIONS
None. The program starts with an interactive menu.
.SH MENU OPTIONS
.TP
.B 1. Add User
Add a new user with username, full name, email, password, and optional password expiration date.
.TP
.B 2. Delete User
Delete an existing user and remove them from all groups.
.TP
.B 3. Add Group
Create a new group.
.TP
.B 4. Delete Group
Delete an existing group and remove it from all users.
.TP
.B 5. Change User Information
Update a user's full name or email.
.TP
.B 6. Change Account Information
Update a user's password or password expiration date.
.TP
.B 7. Assign User to Group
Assign a user to an existing group.
.TP
.B 8. Launch GUI
Open the graphical user interface for user and group management.
.TP
.B 9. Exit
Exit the program.
.SH GUI
The graphical interface provides tabs for:
.TP
- User Management: Add, delete, or update users.
.TP
- Group Management: Add or delete groups.
.TP
- Assign to Group: Assign users to groups.
.SH FILES
.TP
.B user_manager_data.txt
Stores user and group data in a text format.
.SH AUTHOR
Written for a system administration project.
.SH BUGS
Report bugs to the project maintainer.
.SH SEE ALSO
.BR useradd (8),
.BR groupadd (8)