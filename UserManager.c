#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <regex.h>
#include <gtk/gtk.h>

#define MAX_STR 256
#define DATA_FILE "user_manager_data.txt"
#define MAX_USERS 100
#define MAX_GROUPS 50

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

// GUI structure
typedef struct {
    GtkWidget *window;
    GtkWidget *user_username;
    GtkWidget *user_name;
    GtkWidget *user_email;
    GtkWidget *user_password;
    GtkWidget *user_expiration;
    GtkWidget *group_name;
    GtkWidget *assign_username;
    GtkWidget *assign_group;
    Data *data;
} AppWidgets;

// Function prototypes
void load_data(Data *data);
void save_data(Data *data);
int validate_password(const char *password, char *message);
int validate_date(const char *date);
void add_user(Data *data);
void delete_user(Data *data);
void add_group(Data *data);
void delete_group(Data *data);
void change_user_info(Data *data);
void change_account_info(Data *data);
void assign_user_to_group(Data *data);
void cli_menu();
void init_gui(int argc, char *argv[], Data *data);
void gui_add_user(GtkWidget *widget, gpointer data);
void gui_delete_user(GtkWidget *widget, gpointer data);
void gui_update_user(GtkWidget *widget, gpointer data);
void gui_add_group(GtkWidget *widget, gpointer data);
void gui_delete_group(GtkWidget *widget, gpointer data);
void gui_assign_user(GtkWidget *widget, gpointer data);

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
        printf("Error opening file for writing!\n");
        return;
    }

    for (int i = 0; i < data->user_count; i++) {
        User *user = &data->users[i];
        fprintf(file, "USER:%s:%s:%s:%s:%s:", user->username, user->name,
                user->email, user->password, user->expiration_date);
        for (int j = 0; j < user->group_count; j++) {
            fprintf(file, "%s%s", user->groups[j],
                    j < user->group_count - 1 ? "," : "");
        }
        fprintf(file, "\n");
    }

    for (int i = 0; i < data->group_count; i++) {
        Group *group = &data->groups[i];
        fprintf(file, "GROUP:%s:", group->name);
        for (int j = 0; j < group->member_count; j++) {
            fprintf(file, "%s%s", group->members[j],
                    j < group->member_count - 1 ? "," : "");
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

// Validate password
int validate_password(const char *password, char *message) {
    if (strlen(password) < 8) {
        strcpy(message, "Password must be at least 8 characters long.");
        return 0;
    }
    int has_upper = 0, has_lower = 0, has_digit = 0;
    for (const char *p = password; *p; p++) {
        if (isupper(*p)) has_upper = 1;
        if (islower(*p)) has_lower = 1;
        if (isdigit(*p)) has_digit = 1;
    }
    if (!has_upper) {
        strcpy(message, "Password must contain at least one uppercase letter.");
        return 0;
    }
    if (!has_lower) {
        strcpy(message, "Password must contain at least one lowercase letter.");
        return 0;
    }
    if (!has_digit) {
        strcpy(message, "Password must contain at least one digit.");
        return 0;
    }
    return 1;
}

// Validate date
int validate_date(const char *date) {
    regex_t regex;
    int reti = regcomp(&regex, "^[0-9]{4}-[0-9]{2}-[0-9]{2}$", REG_EXTENDED);
    if (reti) return 0;
    reti = regexec(&regex, date, 0, NULL, 0);
    regfree(&regex);
    if (reti) return 0;

    int year, month, day;
    sscanf(date, "%d-%d-%d", &year, &month, &day);
    if (month < 1 || month > 12 || day < 1 || day > 31) return 0;
    return 1;
}

// Add user
void add_user(Data *data) {
    char username[MAX_STR], name[MAX_STR], email[MAX_STR], password[MAX_STR], expiration[MAX_STR];
    char message[MAX_STR];

    printf("Enter username: ");
    fgets(username, MAX_STR, stdin);
    username[strcspn(username, "\n")] = 0;

    for (int i = 0; i < data->user_count; i++) {
        if (strcmp(data->users[i].username, username) == 0) {
            printf("User already exists!\n");
            return;
        }
    }

    printf("Enter full name: ");
    fgets(name, MAX_STR, stdin);
    name[strcspn(name, "\n")] = 0;

    printf("Enter email: ");
    fgets(email, MAX_STR, stdin);
    email[strcspn(email, "\n")] = 0;

    while (1) {
        printf("Enter password: ");
        fgets(password, MAX_STR, stdin);
        password[strcspn(password, "\n")] = 0;
        if (validate_password(password, message)) break;
        printf("%s\n", message);
    }

    printf("Enter password expiration date (YYYY-MM-DD, or leave blank for none): ");
    fgets(expiration, MAX_STR, stdin);
    expiration[strcspn(expiration, "\n")] = 0;

    User *user = &data->users[data->user_count];
    strcpy(user->username, username);
    strcpy(user->name, name);
    strcpy(user->email, email);
    strcpy(user->password, password);
    user->group_count = 0;

    if (expiration[0]) {
        if (validate_date(expiration)) {
            strcpy(user->expiration_date, expiration);
        } else {
            printf("Invalid date format. User added without expiration.\n");
            user->expiration_date[0] = 0;
        }
    } else {
        user->expiration_date[0] = 0;
    }

    data->user_count++;
    save_data(data);
    printf("User %s added successfully.\n", username);
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
        printf("User does not exist!\n");
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
    printf("User %s deleted successfully.\n", username);
}

// Add group
void add_group(Data *data) {
    char group_name[MAX_STR];
    printf("Enter group name: ");
    fgets(group_name, MAX_STR, stdin);
    group_name[strcspn(group_name, "\n")] = 0;

    for (int i = 0; i < data->group_count; i++) {
        if (strcmp(data->groups[i].name, group_name) == 0) {
            printf("Group already exists!\n");
            return;
        }
    }

    Group *group = &data->groups[data->group_count];
    strcpy(group->name, group_name);
    group->member_count = 0;
    data->group_count++;
    save_data(data);
    printf("Group %s created successfully.\n", group_name);
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
        printf("Group does not exist!\n");
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
    printf("Group %s deleted successfully.\n", group_name);
}

// Change user info
void change_user_info(Data *data) {
    char username[MAX_STR], name[MAX_STR], email[MAX_STR];
    printf("Enter username to modify: ");
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
        printf("User does not exist!\n");
        return;
    }

    printf("Leave blank to keep current value.\n");
    printf("Enter new full name [%s]: ", user->name);
    fgets(name, MAX_STR, stdin);
    name[strcspn(name, "\n")] = 0;

    printf("Enter new email [%s]: ", user->email);
    fgets(email, MAX_STR, stdin);
    email[strcspn(email, "\n")] = 0;

    if (name[0]) strcpy(user->name, name);
    if (email[0]) strcpy(user->email, email);
    save_data(data);
    printf("User %s information updated.\n", username);
}

// Change account info
void change_account_info(Data *data) {
    char username[MAX_STR], password[MAX_STR], expiration[MAX_STR], change_password[MAX_STR];
    char message[MAX_STR];

    printf("Enter username to modify: ");
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
        printf("User does not exist!\n");
        return;
    }

    printf("Change password? (y/n): ");
    fgets(change_password, MAX_STR, stdin);
    change_password[strcspn(change_password, "\n")] = 0;

    if (tolower(change_password[0]) == 'y') {
        while (1) {
            printf("Enter new password: ");
            fgets(password, MAX_STR, stdin);
            password[strcspn(password, "\n")] = 0;
            if (validate_password(password, message)) {
                strcpy(user->password, password);
                break;
            }
            printf("%s\n", message);
        }
    }

    printf("Enter new password expiration date (YYYY-MM-DD, or leave blank for none): ");
    fgets(expiration, MAX_STR, stdin);
    expiration[strcspn(expiration, "\n")] = 0;

    if (expiration[0]) {
        if (validate_date(expiration)) {
            strcpy(user->expiration_date, expiration);
        } else {
            printf("Invalid date format. Expiration not changed.\n");
        }
    } else {
        user->expiration_date[0] = 0;
    }

    save_data(data);
    printf("Account information for %s updated.\n", username);
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
        printf("User does not exist!\n");
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
        printf("Group does not exist!\n");
        return;
    }

    for (int i = 0; i < user->group_count; i++) {
        if (strcmp(user->groups[i], group_name) == 0) {
            printf("User %s is already in group %s.\n", username, group_name);
            return;
        }
    }

    strcpy(user->groups[user->group_count++], group_name);
    strcpy(group->members[group->member_count++], username);
    save_data(data);
    printf("User %s assigned to group %s.\n", username, group_name);
}

// CLI menu
void cli_menu() {
    Data data;
    load_data(&data);
    char choice[10];

    while (1) {
        printf("\nUser Manager Menu:\n");
        printf("1. Add User\n");
        printf("2. Delete User\n");
        printf("3. Add Group\n");
        printf("4. Delete Group\n");
        printf("5. Change User Information\n");
        printf("6. Change Account Information\n");
        printf("7. Assign User to Group\n");
        printf("8. Launch GUI\n");
        printf("9. Exit\n");
        printf("Enter your choice (1-9): ");
        fgets(choice, 10, stdin);
        choice[strcspn(choice, "\n")] = 0;

        if (strcmp(choice, "1") == 0) add_user(&data);
        else if (strcmp(choice, "2") == 0) delete_user(&data);
        else if (strcmp(choice, "3") == 0) add_group(&data);
        else if (strcmp(choice, "4") == 0) delete_group(&data);
        else if (strcmp(choice, "5") == 0) change_user_info(&data);
        else if (strcmp(choice, "6") == 0) change_account_info(&data);
        else if (strcmp(choice, "7") == 0) assign_user_to_group(&data);
        else if (strcmp(choice, "8") == 0) init_gui(0, NULL, &data);
        else if (strcmp(choice, "9") == 0) {
            printf("Exiting User Manager.\n");
            break;
        } else {
            printf("Invalid choice. Please try again.\n");
        }
    }
}

// GUI functions
void gui_add_user(GtkWidget *widget, gpointer data) {
    AppWidgets *app = (AppWidgets *)data;
    const char *username = gtk_entry_get_text(GTK_ENTRY(app->user_username));
    const char *name = gtk_entry_get_text(GTK_ENTRY(app->user_name));
    const char *email = gtk_entry_get_text(GTK_ENTRY(app->user_email));
    const char *password = gtk_entry_get_text(GTK_ENTRY(app->user_password));
    const char *expiration = gtk_entry_get_text(GTK_ENTRY(app->user_expiration));
    char message[MAX_STR];

    if (!username[0] || !password[0]) {
        gtk_message_dialog_new(GTK_WINDOW(app->window), GTK_DIALOG_MODAL,
                               GTK_MESSAGE_ERROR, GTK_BUTTONS_OK,
                               "Username and password are required.");
        return;
    }

    for (int i = 0; i < app->data->user_count; i++) {
        if (strcmp(app->data->users[i].username, username) == 0) {
            gtk_message_dialog_new(GTK_WINDOW(app->window), GTK_DIALOG_MODAL,
                                   GTK_MESSAGE_ERROR, GTK_BUTTONS_OK,
                                   "User already exists!");
            return;
        }
    }

    if (!validate_password(password, message)) {
        gtk_message_dialog_new(GTK_WINDOW(app->window), GTK_DIALOG_MODAL,
                               GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "%s", message);
        return;
    }

    User *user = &app->data->users[app->data->user_count];
    strcpy(user->username, username);
    strcpy(user->name, name);
    strcpy(user->email, email);
    strcpy(user->password, password);
    user->group_count = 0;

    if (expiration[0]) {
        if (validate_date(expiration)) {
            strcpy(user->expiration_date, expiration);
        } else {
            gtk_message_dialog_new(GTK_WINDOW(app->window), GTK_DIALOG_MODAL,
                                   GTK_MESSAGE_ERROR, GTK_BUTTONS_OK,
                                   "Invalid date format.");
            return;
        }
    } else {
        user->expiration_date[0] = 0;
    }

    app->data->user_count++;
    save_data(app->data);
    gtk_message_dialog_new(GTK_WINDOW(app->window), GTK_DIALOG_MODAL,
                           GTK_MESSAGE_INFO, GTK_BUTTONS_OK,
                           "User %s added successfully.", username);
    gtk_entry_set_text(GTK_ENTRY(app->user_username), "");
    gtk_entry_set_text(GTK_ENTRY(app->user_name), "");
    gtk_entry_set_text(GTK_ENTRY(app->user_email), "");
    gtk_entry_set_text(GTK_ENTRY(app->user_password), "");
    gtk_entry_set_text(GTK_ENTRY(app->user_expiration), "");
}

void gui_delete_user(GtkWidget *widget, gpointer data) {
    AppWidgets *app = (AppWidgets *)data;
    const char *username = gtk_entry_get_text(GTK_ENTRY(app->user_username));

    int index = -1;
    for (int i = 0; i < app->data->user_count; i++) {
        if (strcmp(app->data->users[i].username, username) == 0) {
            index = i;
            break;
        }
    }
    if (index == -1) {
        gtk_message_dialog_new(GTK_WINDOW(app->window), GTK_DIALOG_MODAL,
                               GTK_MESSAGE_ERROR, GTK_BUTTONS_OK,
                               "User does not exist!");
        return;
    }

    for (int i = 0; i < app->data->group_count; i++) {
        Group *group = &app->data->groups[i];
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

    for (int i = index; i < app->data->user_count - 1; i++) {
        app->data->users[i] = app->data->users[i + 1];
    }
    app->data->user_count--;
    save_data(app->data);
    gtk_message_dialog_new(GTK_WINDOW(app->window), GTK_DIALOG_MODAL,
                           GTK_MESSAGE_INFO, GTK_BUTTONS_OK,
                           "User %s deleted successfully.", username);
    gtk_entry_set_text(GTK_ENTRY(app->user_username), "");
}

void gui_update_user(GtkWidget *widget, gpointer data) {
    AppWidgets *app = (AppWidgets *)data;
    const char *username = gtk_entry_get_text(GTK_ENTRY(app->user_username));
    const char *name = gtk_entry_get_text(GTK_ENTRY(app->user_name));
    const char *email = gtk_entry_get_text(GTK_ENTRY(app->user_email));
    const char *password = gtk_entry_get_text(GTK_ENTRY(app->user_password));
    const char *expiration = gtk_entry_get_text(GTK_ENTRY(app->user_expiration));
    char message[MAX_STR];

    User *user = NULL;
    for (int i = 0; i < app->data->user_count; i++) {
        if (strcmp(app->data->users[i].username, username) == 0) {
            user = &app->data->users[i];
            break;
        }
    }
    if (!user) {
        gtk_message_dialog_new(GTK_WINDOW(app->window), GTK_DIALOG_MODAL,
                               GTK_MESSAGE_ERROR, GTK_BUTTONS_OK,
                               "User does not exist!");
        return;
    }

    if (name[0]) strcpy(user->name, name);
    if (email[0]) strcpy(user->email, email);
    if (password[0]) {
        if (!validate_password(password, message)) {
            gtk_message_dialog_new(GTK_WINDOW(app->window), GTK_DIALOG_MODAL,
                                   GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "%s", message);
            return;
        }
        strcpy(user->password, password);
    }
    if (expiration[0]) {
        if (validate_date(expiration)) {
            strcpy(user->expiration_date, expiration);
        } else {
            gtk_message_dialog_new(GTK_WINDOW(app->window), GTK_DIALOG_MODAL,
                                   GTK_MESSAGE_ERROR, GTK_BUTTONS_OK,
                                   "Invalid date format.");
            return;
        }
    } else if (strcmp(expiration, "") == 0) {
        user->expiration_date[0] = 0;
    }

    save_data(app->data);
    gtk_message_dialog_new(GTK_WINDOW(app->window), GTK_DIALOG_MODAL,
                           GTK_MESSAGE_INFO, GTK_BUTTONS_OK,
                           "User %s updated successfully.", username);
    gtk_entry_set_text(GTK_ENTRY(app->user_username), "");
    gtk_entry_set_text(GTK_ENTRY(app->user_name), "");
    gtk_entry_set_text(GTK_ENTRY(app->user_email), "");
    gtk_entry_set_text(GTK_ENTRY(app->user_password), "");
    gtk_entry_set_text(GTK_ENTRY(app->user_expiration), "");
}

void gui_add_group(GtkWidget *widget, gpointer data) {
    AppWidgets *app = (AppWidgets *)data;
    const char *group_name = gtk_entry_get_text(GTK_ENTRY(app->group_name));

    for (int i = 0; i < app->data->group_count; i++) {
        if (strcmp(app->data->groups[i].name, group_name) == 0) {
            gtk_message_dialog_new(GTK_WINDOW(app->window), GTK_DIALOG_MODAL,
                                   GTK_MESSAGE_ERROR, GTK_BUTTONS_OK,
                                   "Group already exists!");
            return;
        }
    }

    Group *group = &app->data->groups[app->data->group_count];
    strcpy(group->name, group_name);
    group->member_count = 0;
    app->data->group_count++;
    save_data(app->data);
    gtk_message_dialog_new(GTK_WINDOW(app->window), GTK_DIALOG_MODAL,
                           GTK_MESSAGE_INFO, GTK_BUTTONS_OK,
                           "Group %s created successfully.", group_name);
    gtk_entry_set_text(GTK_ENTRY(app->group_name), "");
}

void gui_delete_group(GtkWidget *widget, gpointer data) {
    AppWidgets *app = (AppWidgets *)data;
    const char *group_name = gtk_entry_get_text(GTK_ENTRY(app->group_name));

    int index = -1;
    for (int i = 0; i < app->data->group_count; i++) {
        if (strcmp(app->data->groups[i].name, group_name) == 0) {
            index = i;
            break;
        }
    }
    if (index == -1) {
        gtk_message_dialog_new(GTK_WINDOW(app->window), GTK_DIALOG_MODAL,
                               GTK_MESSAGE_ERROR, GTK_BUTTONS_OK,
                               "Group does not exist!");
        return;
    }

    for (int i = 0; i < app->data->user_count; i++) {
        User *user = &app->data->users[i];
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

    for (int i = index; i < app->data->group_count - 1; i++) {
        app->data->groups[i] = app->data->groups[i + 1];
    }
    app->data->group_count--;
    save_data(app->data);
    gtk_message_dialog_new(GTK_WINDOW(app->window), GTK_DIALOG_MODAL,
                           GTK_MESSAGE_INFO, GTK_BUTTONS_OK,
                           "Group %s deleted successfully.", group_name);
    gtk_entry_set_text(GTK_ENTRY(app->group_name), "");
}

void gui_assign_user(GtkWidget *widget, gpointer data) {
    AppWidgets *app = (AppWidgets *)data;
    const char *username = gtk_entry_get_text(GTK_ENTRY(app->assign_username));
    const char *group_name = gtk_entry_get_text(GTK_ENTRY(app->assign_group));

    User *user = NULL;
    for (int i = 0; i < app->data->user_count; i++) {
        if (strcmp(app->data->users[i].username, username) == 0) {
            user = &app->data->users[i];
            break;
        }
    }
    if (!user) {
        gtk_message_dialog_new(GTK_WINDOW(app->window), GTK_DIALOG_MODAL,
                               GTK_MESSAGE_ERROR, GTK_BUTTONS_OK,
                               "User does not exist!");
        return;
    }

    Group *group = NULL;
    for (int i = 0; i < app->data->group_count; i++) {
        if (strcmp(app->data->groups[i].name, group_name) == 0) {
            group = &app->data->groups[i];
            break;
        }
    }
    if (!group) {
        gtk_message_dialog_new(GTK_WINDOW(app->window), GTK_DIALOG_MODAL,
                               GTK_MESSAGE_ERROR, GTK_BUTTONS_OK,
                               "Group does not exist!");
        return;
    }

    for (int i = 0; i < user->group_count; i++) {
        if (strcmp(user->groups[i], group_name) == 0) {
            gtk_message_dialog_new(GTK_WINDOW(app->window), GTK_DIALOG_MODAL,
                                   GTK_MESSAGE_ERROR, GTK_BUTTONS_OK,
                                   "User %s is already in group %s.", username, group_name);
            return;
        }
    }

    strcpy(user->groups[user->group_count++], group_name);
    strcpy(group->members[group->member_count++], username);
    save_data(app->data);
    gtk_message_dialog_new(GTK_WINDOW(app->window), GTK_DIALOG_MODAL,
                           GTK_MESSAGE_INFO, GTK_BUTTONS_OK,
                           "User %s assigned to group %s.", username, group_name);
    gtk_entry_set_text(GTK_ENTRY(app->assign_username), "");
    gtk_entry_set_text(GTK_ENTRY(app->assign_group), "");
}

// Initialize GUI
void init_gui(int argc, char *argv[], Data *data) {
    gtk_init(&argc, &argv);
    AppWidgets *app = g_new(AppWidgets, 1);
    app->data = data;

    app->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(app->window), "User Manager");
    gtk_window_set_default_size(GTK_WINDOW(app->window), 400, 300);
    g_signal_connect(app->window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *notebook = gtk_notebook_new();
    gtk_container_add(GTK_CONTAINER(app->window), notebook);

    // User Management Tab
    GtkWidget *user_frame = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(user_frame), 5);
    gtk_grid_set_column_spacing(GTK_GRID(user_frame), 5);
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), user_frame, gtk_label_new("User Management"));

    gtk_grid_attach(GTK_GRID(user_frame), gtk_label_new("Username:"), 0, 0, 1, 1);
    app->user_username = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(user_frame), app->user_username, 1, 0, 1, 1);

    gtk_grid_attach(GTK_GRID(user_frame), gtk_label_new("Full Name:"), 0, 1, 1, 1);
    app->user_name = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(user_frame), app->user_name, 1, 1, 1, 1);

    gtk_grid_attach(GTK_GRID(user_frame), gtk_label_new("Email:"), 0, 2, 1, 1);
    app->user_email = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(user_frame), app->user_email, 1, 2, 1, 1);

    gtk_grid_attach(GTK_GRID(user_frame), gtk_label_new("Password:"), 0, 3, 1, 1);
    app->user_password = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(app->user_password), FALSE);
    gtk_grid_attach(GTK_GRID(user_frame), app->user_password, 1, 3, 1, 1);

    gtk_grid_attach(GTK_GRID(user_frame), gtk_label_new("Expiration (YYYY-MM-DD):"), 0, 4, 1, 1);
    app->user_expiration = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(user_frame), app->user_expiration, 1, 4, 1, 1);

    GtkWidget *add_user_btn = gtk_button_new_with_label("Add User");
    g_signal_connect(add_user_btn, "clicked", G_CALLBACK(gui_add_user), app);
    gtk_grid_attach(GTK_GRID(user_frame), add_user_btn, 0, 5, 1, 1);

    GtkWidget *delete_user_btn = gtk_button_new_with_label("Delete User");
    g_signal_connect(delete_user_btn, "clicked", G_CALLBACK(gui_delete_user), app);
    gtk_grid_attach(GTK_GRID(user_frame), delete_user_btn, 1, 5, 1, 1);

    GtkWidget *update_user_btn = gtk_button_new_with_label("Update User");
    g_signal_connect(update_user_btn, "clicked", G_CALLBACK(gui_update_user), app);
    gtk_grid_attach(GTK_GRID(user_frame), update_user_btn, 2, 5, 1, 1);

    // Group Management Tab
    GtkWidget *group_frame = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(group_frame), 5);
    gtk_grid_set_column_spacing(GTK_GRID(group_frame), 5);
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), group_frame, gtk_label_new("Group Management"));

    gtk_grid_attach(GTK_GRID(group_frame), gtk_label_new("Group Name:"), 0, 0, 1, 1);
    app->group_name = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(group_frame), app->group_name, 1, 0, 1, 1);

    GtkWidget *add_group_btn = gtk_button_new_with_label("Add Group");
    g_signal_connect(add_group_btn, "clicked", G_CALLBACK(gui_add_group), app);
    gtk_grid_attach(GTK_GRID(group_frame), add_group_btn, 0, 1, 1, 1);

    GtkWidget *delete_group_btn = gtk_button_new_with_label("Delete Group");
    g_signal_connect(delete_group_btn, "clicked", G_CALLBACK(gui_delete_group), app);
    gtk_grid_attach(GTK_GRID(group_frame), delete_group_btn, 1, 1, 1, 1);

    // Assign to Group Tab
    GtkWidget *assign_frame = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(assign_frame), 5);
    gtk_grid_set_column_spacing(GTK_GRID(assign_frame), 5);
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), assign_frame, gtk_label_new("Assign to Group"));

    gtk_grid_attach(GTK_GRID(assign_frame), gtk_label_new("Username:"), 0, 0, 1, 1);
    app->assign_username = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(assign_frame), app->assign_username, 1, 0, 1, 1);

    gtk_grid_attach(GTK_GRID(assign_frame), gtk_label_new("Group Name:"), 0, 1, 1, 1);
    app->assign_group = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(assign_frame), app->assign_group, 1, 1, 1, 1);

    GtkWidget *assign_btn = gtk_button_new_with_label("Assign");
    g_signal_connect(assign_btn, "clicked", G_CALLBACK(gui_assign_user), app);
    gtk_grid_attach(GTK_GRID(assign_frame), assign_btn, 0, 2, 2, 1);

    gtk_widget_show_all(app->window);
    gtk_main();
    g_free(app);
}

int main(int argc, char *argv[]) {
    cli_menu();
    return 0;
}