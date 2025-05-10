#include <stdio.h>
#include <string.h>

#define MAX 100

typedef struct {
    char username[50];
    char password[50];
    char expiration[20];
    char group[50];
} User;

typedef struct {
    char name[50];
} Group;

User users[MAX];
Group groups[MAX];
int user_count = 0, group_count = 0;

void add_user() {
    printf("Enter username: ");
    scanf("%s", users[user_count].username);
    printf("Enter password: ");
    scanf("%s", users[user_count].password);
    printf("Enter expiration date: ");
    scanf("%s", users[user_count].expiration);
    strcpy(users[user_count].group, "None");
    user_count++;
    printf("User added!\n");
}

void delete_user() {
    char name[50];
    printf("Enter username to delete: ");
    scanf("%s", name);
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].username, name) == 0) {
            for (int j = i; j < user_count - 1; j++)
                users[j] = users[j + 1];
            user_count--;
            printf("User deleted!\n");
            return;
        }
    }
    printf("User not found!\n");
}

void add_group() {
    printf("Enter group name: ");
    scanf("%s", groups[group_count].name);
    group_count++;
    printf("Group added!\n");
}

void delete_group() {
    char name[50];
    printf("Enter group name to delete: ");
    scanf("%s", name);
    for (int i = 0; i < group_count; i++) {
        if (strcmp(groups[i].name, name) == 0) {
            for (int j = i; j < group_count - 1; j++)
                groups[j] = groups[j + 1];
            group_count--;
            printf("Group deleted!\n");
            return;
        }
    }
    printf("Group not found!\n");
}

void change_user_info() {
    char name[50];
    printf("Enter username to change: ");
    scanf("%s", name);
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].username, name) == 0) {
            printf("New password: ");
            scanf("%s", users[i].password);
            printf("New expiration date: ");
            scanf("%s", users[i].expiration);
            printf("Updated!\n");
            return;
        }
    }
    printf("User not found!\n");
}

void assign_user_to_group() {
    char uname[50], gname[50];
    printf("Enter username: ");
    scanf("%s", uname);
    printf("Enter group name: ");
    scanf("%s", gname);

    int group_exists = 0;
    for (int i = 0; i < group_count; i++) {
        if (strcmp(groups[i].group_name, gname) == 0) {
            group_exists = 1;
            break;
        }
    }

    if (!group_exists) {
        printf("Group does not exist!\n");
        return;
    }

    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].username, uname) == 0) {
            strcpy(users[i].group, gname);
            printf("User assigned to group.\n");
            return;
        }
    }

    printf("User not found!\n");
}

void show_manual() {
    printf("Manual Page\n");
    printf("1: Add User\n");
    printf("2: Delete User\n");
    printf("3: Add Group\n");
    printf("4: Delete Group\n");
    printf("5: Change User Info\n");
    printf("6: Assign User to Group\n");
    printf("7: Show Manual\n");
    printf("8: Exit\n");
}

int main() {
    int choice;
    while (1) {
        printf("MENU \n");
        printf("1. Add User\n2. Delete User\n3. Add Group\n4. Delete Group\n5. Change User Info\n6. Assign User to Group\n7. Manual Page\n8. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: 
                add_user(); 
                break;
            case 2: 
                delete_user();
                break;
            case 3:
                add_group();
                break;
            case 4:
                delete_group();
                break;
            case 5:
             change_user_info();
              break;
            case 6: 
                assign_user_to_group();
                 break;
            case 7: 
                show_manual();
                break;
            case 8: 
                printf("Goodbye!\n"); 
                return 0;
            default: 
                printf("Invalid choice!\n");
        }
    }
}
