#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int group_exists(const char *group) {
    char cmd[100];
    sprintf(cmd, "getent group %s > /dev/null 2>&1", group);
    return system(cmd) == 0;
}

int user_exists(const char *username) {
    char cmd[100];
    sprintf(cmd, "id %s > /dev/null 2>&1", username);
    return system(cmd) == 0;
}

void add_user() {
    char username[50];
    printf("Enter username to add: ");
    scanf("%s", username);
    while (getchar() != '\n'); // flush input
    char cmd[100] = "sudo useradd ";
    strcat(cmd, username);
    system(cmd);
}

void delete_user() {
    char username[50];
    printf("Enter username to delete: ");
    scanf("%s", username);
    while (getchar() != '\n');
    if (user_exists(username)) {
        char cmd[100] = "sudo userdel ";
        strcat(cmd, username);
        system(cmd);
    } else {
        printf("User does not exist!\n");
    }
}

void add_group() {
    char group[50];
    printf("Enter group name to add: ");
    scanf("%s", group);
    while (getchar() != '\n');
    if (!group_exists(group)) {
        char cmd[100] = "sudo groupadd ";
        strcat(cmd, group);
        system(cmd);
    } else {
        printf("Group already exists!\n");
    }
}

void delete_group() {
    char group[50];
    printf("Enter group name to delete: ");
    scanf("%s", group);
    while (getchar() != '\n');
    if (group_exists(group)) {
        char cmd[100] = "sudo groupdel ";
        strcat(cmd, group);
        system(cmd);
    } else {
        printf("Group does not exist!\n");
    }
}

void modify_user_info() {
    char username[50], fullname[100];
    printf("Enter username to modify: ");
    scanf("%s", username);
    while (getchar() != '\n'); // flush newline
    if (!user_exists(username)) {
        printf("User does not exist!\n");
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

void change_account_info() {
    char username[50];
    int days;
    printf("Enter username: ");
    scanf("%s", username);
    while (getchar() != '\n');
    if (!user_exists(username)) {
        printf("User does not exist!\n");
        return;
    }
    printf("Enter max password age (days): ");
    scanf("%d", &days);
    while (getchar() != '\n');

    char cmd[100];
    char daystr[10];
    sprintf(daystr, "%d", days);

    strcpy(cmd, "sudo chage -M ");
    strcat(cmd, daystr);
    strcat(cmd, " ");
    strcat(cmd, username);
    system(cmd);
}

void assign_user_to_group() {
    char username[50], group[50];
    printf("Enter username: ");
    scanf("%s", username);
    while (getchar() != '\n');
    if (!user_exists(username)) {
        printf("User does not exist!\n");
        return;
    }

    printf("Enter group name: ");
    scanf("%s", group);
    while (getchar() != '\n');
    if (group_exists(group)) {
        char cmd[100] = "sudo usermod -aG ";
        strcat(cmd, group);
        strcat(cmd, " ");
        strcat(cmd, username);
        system(cmd);
    } else {
        printf("Group does not exist!\n");
    }
}

int main(int argc, char *argv[]) {
    if (argc > 1 && strcmp(argv[1], "--help") == 0) {
        printf("Usage: ./task3\n");
        printf("A simple user & group manager menu program. Requires root access.\n");
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
            default: printf("Invalid choice!\n");
        }
    } while (choice != 8);

    return 0;
}
