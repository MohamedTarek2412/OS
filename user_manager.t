.\" Man page for user_manager
.TH USER_MANAGER 1 "May 07, 2025" "1.0" "User Manager Manual"
.SH NAME
user_manager \- A tool to manage users and groups
.SH SYNOPSIS
.B user_manager
.SH DESCRIPTION
.B user_manager
is a Python-based application that provides both command-line and graphical interfaces to manage users and groups. It allows administrators to add/delete users, add/delete groups, change user information, modify account details (e.g., password expiration), and assign users to groups. Data is stored in a JSON file for simplicity.
.SH OPTIONS
None. The program runs interactively, presenting a menu to select actions.
.SH USAGE
Run the program by executing:
.PP
.B python3 user_manager.py
.PP
The command-line menu provides the following options:
.IP "1. Add User"
Create a new user with username, full name, email, password, and optional password expiration.
.IP "2. Delete User"
Remove an existing user and their group memberships.
.IP "3. Add Group"
Create a new group.
.IP "4. Delete Group"
Remove an existing group and update user memberships.
.IP "5. Change User Information"
Update a user's full name or email.
.IP "6. Change Account Information"
Modify a user's password or password expiration date.
.IP "7. Assign User to Group"
Add a user to a specified group.
.IP "8. Launch GUI"
Open the graphical user interface for user and group management.
.IP "9. Exit"
Quit the program.
.PP
The GUI provides tabs for user management, group management, and assigning users to groups, with input fields and buttons for each action.
.SH FILES
.PP
.I user_manager_data.json
.RS
Stores user and group data in JSON format.
.RE
.SH NOTES
- Passwords must be at least 8 characters long, containing at least one uppercase letter, one lowercase letter, and one digit.
- The program does not manage system users; it uses a JSON file for data storage.
- The GUI requires Tkinter, which is included with standard Python installations on Ubuntu.
.SH AUTHOR
Written by Grok, created by xAI.
.SH BUGS
Report bugs to the project maintainer.