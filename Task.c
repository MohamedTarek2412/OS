#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <regex.h>

#define MAX_STR 256
#define MAX_USERS 100
#define MAX_GROUPS 50
#define DATA_FILE "users_data.txt"

// Structures
typedef struct {
    char username[MAX_STR];
    char name[MAX_STR];
    char email[MAX_STR];
    char password[MAX_STR];
    char expiration_date[MAX_STR];
    char groups[MAX_GROUPS][MAX_STR];
    int group_count;
} User;

typedef struct {
    char name[MAX_STR];
    char members[MAX_USERS][MAX_STR];
    int member_count;
} Group;

typedef struct {
    User users[MAX_USERS];
    Group groups[MAX_GROUPS];
    int user_count;
    int group_count;
} Data;

// Function prototypes
void load_data(Data *data);
void save_data(Data *data);
int validate_password(const char *password);
int validate_date(const char *date);
void add_user(Data *data);
void delete_user(Data *data);
void add_group(Data *data);
void delete_group(Data *data);
void change_user_info(Data *data);
void change_account_info(Data *data);
void assign_user_to_group(Data *data);
void cli_menu();

// Load data from file
void load_data(Data *data) {
    FILE *file = fopen(DATA_FILE, "r");
    data->user_count = 0;
    data->group_count = 0;
    if (!file) return;

    char line[MAX_STR * 2];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;
        if (strncmp(line, "USER:", 5) == 0) {
            User *user = &data->users[data->user_count];
            user->group_count = 0;
            sscanf(line, "USER:%[^:]:%[^:]:%[^:]:%[^:]:%[^:]:%[^\n]",
                   user->username, user->name, user->email, user->password,
                   user->expiration_date, line);
            char *group = strtok(line, ",");
            while (group && user->group_count < MAX_GROUPS) {
                strcpy(user->groups[user->group_count++], group);
                group = strtok(NULL, ",");
            }
            data->user_count++;
        } else if (strncmp(line, "GROUP:", 6) == 0) {
            Group *group = &data->groups[data->group_count];
            group->member_count = 0;
            sscanf(line, "GROUP:%[^:]:%[^\n]", group->name, line);
            char *member = strtok(line, ",");
            while (member && group->member_count < MAX_USERS) {
                strcpy(group->members[group->member_count++], member);
                member = strtok(NULL, ",");
            }
            data->group_count++;
        }
    }
    fclose(file);
}

// Save data to file
void save_data(Data *data) {
    FILE *file = fopen(DATA_FILE, "w");
    if (!file) {
        printf("Error opening file!\n");
        return;
    }

    for (int i = 0; i < data->user_count; i++) {
        User *user = &data->users[i];
        fprintf(file, "USER:%s:%s:%s:%s:%s:", user->username, user->name,
                user->email, user->password, user->expiration_date);
        for (int j = 0; j < user->group_count; j++) {
            fprintf(file, "%s%s", user->groups[j], j < user->group_count - 1 ? "," : "");
        }
        fprintf(file, "\n");
    }

    for (int i = 0; i < data->group_count; i++) {
        Group *group = &data->groups[i];
        fprintf(file, "GROUP:%s:", group->name);
        for (int j = 0; j < group->member_count; j++) {
            fprintf(file, "%s%s", group->members[j], j < group->member_count - 1 ? "," : "");
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

// Validate password
int validate_password(const char *password) {
    if (strlen(password) < 8) return 0;
    int has_upper = 0, has_lower = 0, has_digit = 0;
    for (const char *p = password; *p; p++) {
        if (isupper(*p)) has_upper = 1;
        if (islower(*p)) has_lower = 1;
        if (isdigit(*p)) has_digit = 1;
    }
    return has_upper && has_lower && has_digit;
}

// Validate date
int validate_date(const char *date) {
    regex_t regex;
    int reti = regcomp(&regex, "^[0-9]{4}-[0-9]{2}-[0-9]{2}$", REG_EXTENDED);
    if (reti) return 0;
    reti = regexec(&regex, date, 0, NULL, 0);
    regfree(&regex);
    return !reti;
}

// Add user
void add_user(Data *data) {
    char username[MAX_STR], name[MAX_STR], email[MAX_STR], password[MAX_STR], expiration[MAX_STR];

    printf("Enter username: ");
    fgets(username, MAX_STR, stdin);
    username[strcspn(username, "\n")] = 0;

    for (int i = 0; i < data->user_count; i++) {
        if (strcmp(data->users[i].username, username) == 0) {
            printf("User exists!\n");
            return;
        }
    }

    printf("Enter full name: ");
    fgets(name, MAX_STR, stdin);
    name[strcspn(name, "\n")] = 0;

    printf("Enter email: ");
    fgets(email, MAX_STR, stdin);
    email[strcspn(email, "\n")] = 0;

    printf("Enter password (min 8 chars, upper, lower, digit): ");
    fgets(password, MAX_STR, stdin);
    password[strcspn(password, "\n")] = 0;
    if (!validate_password(password)) {
        printf("Invalid password!\n");
        return;
    }

    printf("Enter expiration date (YYYY-MM-DD, or blank): ");
    fgets(expiration, MAX_STR, stdin);
    expiration[strcspn(expiration, "\n")] = 0;

    User *user = &data->users[data->user_count];
    strcpy(user->username, username);
    strcpy(user->name, name);
    strcpy(user->email, email);
    strcpy(user->password, password);
    user->group_count = 0;

    if (expiration[0] && !validate_date(expiration)) {
        printf("Invalid date!\n");
        user->expiration_date[0] = 0;
    } else {
        strcpy(user->expiration_date, expiration);
    }

    data->user_count++;
    save_data(data);
    printf("User added.\n");
}

// Delete user
void delete_user(Data *data) {
    char username[MAX_STR];
    printf("Enter username to delete: ");
    fgets(username, MAX_STR, stdin);
    username[strcspn(username, "\n")] = 0;

    int index = -1;
    for (int i = 0; i < data->user_count; i++) {
        if (strcmp(data->users[i].username, username) == 0) {
            index = i;
            break;
        }
    }
    if (index == -1) {
        printf("User not found!\n");
        return;
    }

    for (int i = 0; i < data->group_count; i++) {
        Group *group = &data->groups[i];
        for (int j = 0; j < group->member_count; j++) {
            if (strcmp(group->members[j], username) == 0) {
                for (int k = j; k < group->member_count - 1; k++) {
                    strcpy(group->members[k], group->members[k + 1]);
                }
                group->member_count--;
                break;
            }
        }
    }

    for (int i = index; i < data->user_count - 1; i++) {
        data->users[i] = data->users[i + 1];
    }
    data->user_count--;
    save_data(data);
    printf("User deleted.\n");
}

// Add group
void add_group(Data *data) {
    char group_name[MAX_STR];
    printf("Enter group name: ");
    fgets(group_name, MAX_STR, stdin);
    group_name[strcspn(group_name, "\n")] = 0;

    for (int i = 0; i < data->group_count; i++) {
        if (strcmp(data->groups[i].name, group_name) == 0) {
            printf("Group exists!\n");
            return;
        }
    }

    Group *group = &data->groups[data->group_count];
    strcpy(group->name, group_name);
    group->member_count = 0;
    data->group_count++;
    save_data(data);
    printf("Group added.\n");
}

// Delete group
void delete_group(Data *data) {
    char group_name[MAX_STR];
    printf("Enter group name to delete: ");
    fgets(group_name, MAX_STR, stdin);
    group_name[strcspn(group_name, "\n")] = 0;

    int index = -1;
    for (int i = 0; i < data->group_count; i++) {
        if (strcmp(data->groups[i].name, group_name) == 0) {
            index = i;
            break;
        }
    }
    if (index == -1) {
        printf("Group not found!\n");
        return;
    }

    for (int i = 0; i < data->user_count; i++) {
        User *user = &data->users[i];
        for (int j = 0; j < user->group_count; j++) {
            if (strcmp(user->groups[j], group_name) == 0) {
                for (int k = j; k < user->group_count - 1; k++) {
                    strcpy(user->groups[k], user->groups[k + 1]);
                }
                user->group_count--;
                break;
            }
        }
    }

    for (int i = index; i < data->group_count - 1; i++) {
        data->groups[i] = data->groups[i + 1];
    }
    data->group_count--;
    save_data(data);
    printf("Group deleted.\n");
}

// Change user info
void change_user_info(Data *data) {
    char username[MAX_STR], name[MAX_STR], email[MAX_STR];
    printf("Enter username: ");
    fgets(username, MAX_STR, stdin);
    username[strcspn(username, "\n")] = 0;

    User *user = NULL;
    for (int i = 0; i < data->user_count; i++) {
        if (strcmp(data->users[i].username, username) == 0) {
            user = &data->users[i];
            break;
        }
    }
    if (!user) {
        printf("User not found!\n");
        return;
    }

    printf("New name (blank to keep %s): ", user->name);
    fgets(name, MAX_STR, stdin);
    name[strcspn(name, "\n")] = 0;

    printf("New email (blank to keep %s): ", user->email);
    fgets(email, MAX_STR, stdin);
    email[strcspn(email, "\n")] = 0;

    if (name[0]) strcpy(user->name, name);
    if (email[0]) strcpy(user->email, email);
    save_data(data);
    printf("Info updated.\n");
}

// Change account info
void change_account_info(Data *data) {
    char username[MAX_STR], password[MAX_STR], expiration[MAX_STR];
    printf("Enter username: ");
    fgets(username, MAX_STR, stdin);
    username[strcspn(username, "\n")] = 0;

    User *user = NULL;
    for (int i = 0; i < data->user_count; i++) {
        if (strcmp(data->users[i].username, username) == 0) {
            user = &data->users[i];
            break;
        }
    }
    if (!user) {
        printf("User not found!\n");
        return;
    }

    printf("New password (blank to keep current): ");
    fgets(password, MAX_STR, stdin);
    password[strcspn(password, "\n")] = 0;
    if (password[0] && !validate_password(password)) {
        printf("Invalid password!\n");
        return;
    }

    printf("New expiration date (YYYY-MM-DD, or blank): ");
    fgets(expiration, MAX_STR, stdin);
    expiration[strcspn(expiration, "\n")] = 0;

    if (password[0]) strcpy(user->password, password);
    if (expiration[0] && !validate_date(expiration)) {
        printf("Invalid date!\n");
    } else {
        strcpy(user->expiration_date, expiration);
    }

    save_data(data);
    printf("Account updated.\n");
}

// Assign user to group
void assign_user_to_group(Data *data) {
    char username[MAX_STR], group_name[MAX_STR];
    printf("Enter username: ");
    fgets(username, MAX_STR, stdin);
    username[strcspn(username, "\n")] = 0;

    User *user = NULL;
    for (int i = 0; i < data->user_count; i++) {
        if (strcmp(data->users[i].username, username) == 0) {
            user = &data->users[i];
            break;
        }
    }
    if (!user) {
        printf("User not found!\n");
        return;
    }

    printf("Enter group name: ");
    fgets(group_name, MAX_STR, stdin);
    group_name[strcspn(group_name, "\n")] = 0;

    Group *group = NULL;
    for (int i = 0; i < data->group_count; i++) {
        if (strcmp(data->groups[i].name, group_name) == 0) {
            group = &data->groups[i];
            break;
        }
    }
    if (!group) {
        printf("Group not found!\n");
        return;
    }

    for (int i = 0; i < user->group_count; i++) {
        if (strcmp(user->groups[i], group_name) == 0) {
            printf("User already in group!\n");
            return;
        }
    }

    strcpy(user->groups[user->group_count++], group_name);
    strcpy(group->members[group->member_count++], username);
    save_data(data);
    printf("User assigned to group.\n");
}

// CLI menu
void cli_menu() {
    Data data;
    load_data(&data);
    char choice[10];

    while (1) {
        printf("\nUser Manager:\n");
        printf("1. Add User\n");
        printf("2. Delete User\n");
        printf("3. Add Group\n");
        printf("4. Delete Group\n");
        printf("5. Change User Info\n");
        printf("6. Change Account Info\n");
        printf("7. Assign User to Group\n");
        printf("8. Exit\n");
        printf("Choice (1-8): ");
        fgets(choice, 10, stdin);
        choice[strcspn(choice, "\n")] = 0;

        if (strcmp(choice, "1") == 0) add_user(&data);
        else if (strcmp(choice, "2") == 0) delete_user(&data);
        else if (strcmp(choice, "3") == 0) add_group(&data);
        else if (strcmp(choice, "4") == 0) delete_group(&data);
        else if (strcmp(choice, "5") == 0) change_user_info(&data);
        else if (strcmp(choice, "6") == 0) change_account_info(&data);
        else if (strcmp(choice, "7") == 0) assign_user_to_group(&data);
        else if (strcmp(choice, "8") == 0) {
            printf("Exiting.\n");
            break;
        } else {
            printf("Invalid choice!\n");
        }
    }
}

int main() {
    cli_menu();
    return 0;
}