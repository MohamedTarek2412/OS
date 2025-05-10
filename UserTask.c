#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Check if user exists
int user_exists(const char *username) {
    char cmd[100] = "getent passwd ";
    strcat(cmd, username);
    return system(cmd) == 0;
}

// Check if group exists
int group_exists(const char *group) {
    char cmd[100] = "getent group ";
    strcat(cmd, group);
    return system(cmd) == 0;
}

// Add user
void add_user() {
    char username[50];
    char password[50];
    char cmd[200];

    printf("Enter username to add: ");
    scanf("%s", username);
    while (getchar() != '\n');

    if (user_exists(username)) {
        printf("User '%s' already exists.\n", username);
        return;
    }

    printf("Enter password for user: ");
    scanf("%s", password);
    while (getchar() != '\n');

    strcpy(cmd, "sudo useradd ");
    strcat(cmd, username);
    system(cmd);

    char pass_cmd[200] = "echo '";
    strcat(pass_cmd, username);
    strcat(pass_cmd, ":");
    strcat(pass_cmd, password);
    strcat(pass_cmd, "' | sudo chpasswd");
    system(pass_cmd);

    printf("User '%s' created with password.\n", username);
}

// Delete user
void delete_user() {
    char username[50];
    printf("Enter username to delete: ");
    scanf("%s", username);
    while (getchar() != '\n');

    if (!user_exists(username)) {
        printf("User '%s' does not exist.\n", username);
        return;
    }

    char cmd[100] = "sudo userdel ";
    strcat(cmd, username);
    system(cmd);
}

// Add group
void add_group() {
    char group[50];
    printf("Enter group name to add: ");
    scanf("%s", group);
    while (getchar() != '\n');

    if (group_exists(group)) {
        printf("Group '%s' already exists.\n", group);
        return;
    }

    char cmd[100] = "sudo groupadd ";
    strcat(cmd, group);
    system(cmd);
}

// Delete group
void delete_group() {
    char group[50];
    printf("Enter group name to delete: ");
    scanf("%s", group);
    while (getchar() != '\n');

    if (!group_exists(group)) {
        printf("Group '%s' does not exist.\n", group);
        return;
    }

    char cmd[100] = "sudo groupdel ";
    strcat(cmd, group);
    system(cmd);
}

// Modify user info
void modify_user_info() {
    char username[50], fullname[100];
    printf("Enter username to modify: ");
    scanf("%s", username);
    while (getchar() != '\n');

    if (!user_exists(username)) {
        printf("User '%s' does not exist.\n", username);
        return;
    }

    printf("Enter new full name: ");
    fgets(fullname, sizeof(fullname), stdin);
    fullname[strcspn(fullname, "\n")] = 0;

    char cmd[200] = "sudo usermod -c \"";
    strcat(cmd, fullname);
    strcat(cmd, "\" ");
    strcat(cmd, username);
    system(cmd);
}

// Change account info (password expiration)
void change_account_info() {
    char username[50];
    int days;
    char days_str[10];

    printf("Enter username: ");
    scanf("%s", username);
    while (getchar() != '\n');

    if (!user_exists(username)) {
        printf("User '%s' does not exist.\n", username);
        return;
    }

    printf("Enter max password age (days): ");
    scanf("%d", &days);
    while (getchar() != '\n');

    sprintf(days_str, "%d", days);  // Only exception for safe int-to-string
    char cmd[100] = "sudo chage -M ";
    strcat(cmd, days_str);
    strcat(cmd, " ");
    strcat(cmd, username);
    system(cmd);
}

// Assign user to group
void assign_user_to_group() {
    char username[50], group[50];
    printf("Enter username: ");
    scanf("%s", username);
    while (getchar() != '\n');

    if (!user_exists(username)) {
        printf("User '%s' does not exist.\n", username);
        return;
    }

    printf("Enter group name: ");
    scanf("%s", group);
    while (getchar() != '\n');

    if (!group_exists(group)) {
        printf("Group '%s' does not exist.\n", group);
        return;
    }

    char cmd[100] = "sudo usermod -aG ";
    strcat(cmd, group);
    strcat(cmd, " ");
    strcat(cmd, username);
    system(cmd);
}

// Print Man Page
void print_man_page() {
    printf("Usage: ./user_manager\n");
    printf("Description: Manage users and groups via menu interface.\n");
    printf("Options:\n");
    printf("1. Add User - Add a new user to the system.\n");
    printf("2. Delete User - Remove an existing user from the system.\n");
    printf("3. Add Group - Create a new group on the system.\n");
    printf("4. Delete Group - Remove an existing group from the system.\n");
    printf("5. Modify User Info - Change user's full name or description.\n");
    printf("6. Change Account Info (Password Expiration) - Set password expiration limits.\n");
    printf("7. Assign User to Group - Add a user to an existing group.\n");
    printf("8. Exit - Exit the program.\n");
}

// Main menu
int main(int argc, char *argv[]) {
    // Display man page if user requests --help
    if (argc > 1 && strcmp(argv[1], "--help") == 0) {
        print_man_page();
        return 0;
    }

    int choice;
    do {
        printf("\n--- User Manager Menu ---\n");
        printf("1. Add User\n");
        printf("2. Delete User\n");
        printf("3. Add Group\n");
        printf("4. Delete Group\n");
        printf("5. Modify User Info\n");
        printf("6. Change Account Info (Password Expiration)\n");
        printf("7. Assign User to Group\n");
        printf("8. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        while (getchar() != '\n'); // clear input buffer

        switch (choice) {
            case 1: add_user(); break;
            case 2: delete_user(); break;
            case 3: add_group(); break;
            case 4: delete_group(); break;
            case 5: modify_user_info(); break;
            case 6: change_account_info(); break;
            case 7: assign_user_to_group(); break;
            case 8: printf("Exiting...\n"); break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 8);

    return 0;
}
