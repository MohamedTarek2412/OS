#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_USERNAME_LENGTH 50
#define MAX_NAME_LENGTH 100
#define MAX_EMAIL_LENGTH 100
#define MAX_PASSWORD_LENGTH 100
#define DATA_FILE "user_manager_data.txt"

typedef struct {
    char username[MAX_USERNAME_LENGTH];
    char name[MAX_NAME_LENGTH];
    char email[MAX_EMAIL_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    char expiration_date[MAX_USERNAME_LENGTH];
} User;

typedef struct {
    char name[MAX_USERNAME_LENGTH];
    char members[MAX_USERNAME_LENGTH][MAX_USERNAME_LENGTH];  // List of usernames
    int member_count;
} Group;

typedef struct {
    User users[MAX_USERNAME_LENGTH];
    Group groups[MAX_USERNAME_LENGTH];
    int user_count;
    int group_count;
} UserManager;

void save_data(UserManager *manager) {
    FILE *file = fopen(DATA_FILE, "w");
    if (file) {
        fwrite(manager, sizeof(UserManager), 1, file);
        fclose(file);
    }
}

void load_data(UserManager *manager) {
    FILE *file = fopen(DATA_FILE, "r");
    if (file) {
        fread(manager, sizeof(UserManager), 1, file);
        fclose(file);
    } else {
        manager->user_count = 0;
        manager->group_count = 0;
    }
}

int validate_password(const char *password) {
    int length = strlen(password);
    if (length < 8) return 0;

    int has_upper = 0, has_lower = 0, has_digit = 0;
    for (int i = 0; i < length; i++) {
        if (password[i] >= 'A' && password[i] <= 'Z') has_upper = 1;
        if (password[i] >= 'a' && password[i] <= 'z') has_lower = 1;
        if (password[i] >= '0' && password[i] <= '9') has_digit = 1;
    }
    return has_upper && has_lower && has_digit;
}

void add_user(UserManager *manager) {
    if (manager->user_count >= MAX_USERNAME_LENGTH) {
        printf("User limit reached!\n");
        return;
    }
    User user;
    printf("Enter username: ");
    scanf("%s", user.username);
    for (int i = 0; i < manager->user_count; i++) {
        if (strcmp(manager->users[i].username, user.username) == 0) {
            printf("User already exists!\n");
            return;
        }
    }
    printf("Enter full name: ");
    getchar();  // to consume newline
    fgets(user.name, MAX_NAME_LENGTH, stdin);
    user.name[strcspn(user.name, "\n")] = '\0';  // Remove newline
    printf("Enter email: ");
    fgets(user.email, MAX_EMAIL_LENGTH, stdin);
    user.email[strcspn(user.email, "\n")] = '\0';  // Remove newline
    printf("Enter password: ");
    scanf("%s", user.password);
    if (!validate_password(user.password)) {
        printf("Password is not strong enough!\n");
        return;
    }
    printf("Enter expiration date (YYYY-MM-DD): ");
    scanf("%s", user.expiration_date);

    manager->users[manager->user_count++] = user;
    save_data(manager);
    printf("User added successfully.\n");
}

void delete_user(UserManager *manager) {
    char username[MAX_USERNAME_LENGTH];
    printf("Enter username to delete: ");
    scanf("%s", username);
    int found = 0;
    for (int i = 0; i < manager->user_count; i++) {
        if (strcmp(manager->users[i].username, username) == 0) {
            for (int j = i; j < manager->user_count - 1; j++) {
                manager->users[j] = manager->users[j + 1];
            }
            manager->user_count--;
            found = 1;
            break;
        }
    }
    if (found) {
        save_data(manager);
        printf("User deleted successfully.\n");
    } else {
        printf("User not found!\n");
    }
}

void add_group(UserManager *manager) {
    if (manager->group_count >= MAX_USERNAME_LENGTH) {
        printf("Group limit reached!\n");
        return;
    }
    Group group;
    printf("Enter group name: ");
    scanf("%s", group.name);
    for (int i = 0; i < manager->group_count; i++) {
        if (strcmp(manager->groups[i].name, group.name) == 0) {
            printf("Group already exists!\n");
            return;
        }
    }
    group.member_count = 0;
    manager->groups[manager->group_count++] = group;
    save_data(manager);
    printf("Group added successfully.\n");
}

void delete_group(UserManager *manager) {
    char group_name[MAX_USERNAME_LENGTH];
    printf("Enter group name to delete: ");
    scanf("%s", group_name);
    int found = 0;
    for (int i = 0; i < manager->group_count; i++) {
        if (strcmp(manager->groups[i].name, group_name) == 0) {
            for (int j = i; j < manager->group_count - 1; j++) {
                manager->groups[j] = manager->groups[j + 1];
            }
            manager->group_count--;
            found = 1;
            break;
        }
    }
    if (found) {
        save_data(manager);
        printf("Group deleted successfully.\n");
    } else {
        printf("Group not found!\n");
    }
}

void assign_user_to_group(UserManager *manager) {
    char username[MAX_USERNAME_LENGTH], group_name[MAX_USERNAME_LENGTH];
    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter group name: ");
    scanf("%s", group_name);

    int user_index = -1, group_index = -1;
    for (int i = 0; i < manager->user_count; i++) {
        if (strcmp(manager->users[i].username, username) == 0) {
            user_index = i;
            break;
        }
    }
    for (int i = 0; i < manager->group_count; i++) {
        if (strcmp(manager->groups[i].name, group_name) == 0) {
            group_index = i;
            break;
        }
    }

    if (user_index == -1) {
        printf("User not found!\n");
        return;
    }
    if (group_index == -1) {
        printf("Group not found!\n");
        return;
    }
    Group *group = &manager->groups[group_index];
    for (int i = 0; i < group->member_count; i++) {
        if (strcmp(group->members[i], username) == 0) {
            printf("User is already in the group.\n");
            return;
        }
    }
    strcpy(group->members[group->member_count++], username);
    save_data(manager);
    printf("User assigned to group successfully.\n");
}

void cli_menu(UserManager *manager) {
    int choice;
    while (1) {
        printf("\nUser Manager Menu:\n");
        printf("1. Add User\n");
        printf("2. Delete User\n");
        printf("3. Add Group\n");
        printf("4. Delete Group\n");
        printf("5. Assign User to Group\n");
        printf("6. Exit\n");
        printf("Enter your choice (1-6): ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                add_user(manager);
                break;
            case 2:
                delete_user(manager);
                break;
            case 3:
                add_group(manager);
                break;
            case 4:
                delete_group(manager);
                break;
            case 5:
                assign_user_to_group(manager);
                break;
            case 6:
                printf("Exiting User Manager.\n");
                return;
            default:
                printf("Invalid choice. Try again.\n");
        }
    }
}

int main() {
    UserManager manager;
    load_data(&manager);
    cli_menu(&manager);
    return 0;
}
