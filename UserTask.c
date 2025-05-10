#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void add_user() {
    char username[50];
    printf("Enter username to add: ");
    scanf("%s", username);
    char cmd[100];
    snprintf(cmd, sizeof(cmd), "sudo useradd %s", username);
    system(cmd);
}

void delete_user() {
    char username[50];
    printf("Enter username to delete: ");
    scanf("%s", username);
    char cmd[100];
    snprintf(cmd, sizeof(cmd), "sudo userdel %s", username);
    system(cmd);
}

void add_group() {
    char group[50];
    printf("Enter group name to add: ");
    scanf("%s", group);
    char cmd[100];
    snprintf(cmd, sizeof(cmd), "sudo groupadd %s", group);
    system(cmd);
}

void delete_group() {
    char group[50];
    printf("Enter group name to delete: ");
    scanf("%s", group);
    char cmd[100];
    snprintf(cmd, sizeof(cmd), "sudo groupdel %s", group);
    system(cmd);
}

void modify_user_info() {
    char username[50], fullname[100];
    printf("Enter username to modify: ");
    scanf("%s", username);
    getchar(); // flush newline
    printf("Enter new full name: ");
    fgets(fullname, sizeof(fullname), stdin);
    fullname[strcspn(fullname, "\n")] = 0; // remove newline
    char cmd[200];
    snprintf(cmd, sizeof(cmd), "sudo usermod -c \"%s\" %s", fullname, username);
    system(cmd);
}

void change_account_info() {
    char username[50];
    int days;
    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter max password age (days): ");
    scanf("%d", &days);
    char cmd[100];
    snprintf(cmd, sizeof(cmd), "sudo chage -M %d %s", days, username);
    system(cmd);
}

void assign_user_to_group() {
    char username[50], group[50];
    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter group name: ");
    scanf("%s", group);
    char cmd[100];
    snprintf(cmd, sizeof(cmd), "sudo usermod -aG %s %s", group, username);
    system(cmd);
}

int main() {
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
