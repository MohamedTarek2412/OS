#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int user_exists(const char *username) {
    char cmd[100] = "getent passwd ";
    strcat(cmd, username);
    return system(cmd) == 0;
}

int group_exists(const char *group) {
    char cmd[100] = "getent group ";
    strcat(cmd, group);
    return system(cmd) == 0;
}

void add_user() {
    char username[50];
    char password[50];
    char fullname[100];
    char days[10];
    char cmd[300];
    char pass_cmd[300];
    char chage_cmd[200];

    printf("Enter username to add: ");
    scanf("%s", username);
    while (getchar() != '\n');

    if (user_exists(username)) {
        printf("User '%s' already exists.\n", username);
        return;
    }

    printf("Enter full name: ");
    fgets(fullname, sizeof(fullname), stdin);
    fullname[strcspn(fullname, "\n")] = 0;

    printf("Enter password: ");
    scanf("%s", password);
    while (getchar() != '\n');

    printf("Enter max password age (days): ");
    scanf("%s", days);
    while (getchar() != '\n');

    strcpy(cmd, "sudo useradd -c '");
    strcat(cmd, fullname);
    strcat(cmd, "' ");
    strcat(cmd, username);
    system(cmd);

    strcpy(pass_cmd, "echo '");
    strcat(pass_cmd, username);
    strcat(pass_cmd, ":");
    strcat(pass_cmd, password);
    strcat(pass_cmd, "' | sudo chpasswd");
    system(pass_cmd);

    strcpy(chage_cmd, "sudo chage -M ");
    strcat(chage_cmd, days);
    strcat(chage_cmd, " ");
    strcat(chage_cmd, username);
    system(chage_cmd);

    printf("User '%s' created.\n", username);
}

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

void modify_user_info() {
    char old_username[50], new_username[50], fullname[100];
    printf("Enter current username: ");
    scanf("%s", old_username);
    while (getchar() != '\n');

    if (!user_exists(old_username)) {
        printf("User '%s' does not exist.\n", old_username);
        return;
    }

    printf("Enter new full name: ");
    fgets(fullname, sizeof(fullname), stdin);
    fullname[strcspn(fullname, "\n")] = 0;

    char cmd[200] = "sudo usermod -c \"";
    strcat(cmd, fullname);
    strcat(cmd, "\" ");
    strcat(cmd, old_username);
    system(cmd);

    printf("Enter new username (or press enter to skip): ");
    fgets(new_username, sizeof(new_username), stdin);
    new_username[strcspn(new_username, "\n")] = 0;

    if (strlen(new_username) > 0) {
        char rename_cmd[200] = "sudo usermod -l ";
        strcat(rename_cmd, new_username);
        strcat(rename_cmd, " ");
        strcat(rename_cmd, old_username);
        system(rename_cmd);
    }
}

void change_account_info() {
    char username[50];
    char password[50];
    char days[10];
    printf("Enter username: ");
    scanf("%s", username);
    while (getchar() != '\n');

    if (!user_exists(username)) {
        printf("User '%s' does not exist.\n", username);
        return;
    }

    printf("Enter new password: ");
    scanf("%s", password);
    while (getchar() != '\n');

    printf("Enter max password age (days): ");
    scanf("%s", days);
    while (getchar() != '\n');

    char pass_cmd[200] = "echo '";
    strcat(pass_cmd, username);
    strcat(pass_cmd, ":");
    strcat(pass_cmd, password);
    strcat(pass_cmd, "' | sudo chpasswd");
    system(pass_cmd);

    char chage_cmd[100] = "sudo chage -M ";
    strcat(chage_cmd, days);
    strcat(chage_cmd, " ");
    strcat(chage_cmd, username);
    system(chage_cmd);
}

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

int main(int argc, char *argv[]) {
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
        while (getchar() != '\n');

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
