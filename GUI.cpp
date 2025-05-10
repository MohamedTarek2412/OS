#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Callback for Add User
void on_add_user_clicked(GtkButton *button, gpointer window) {
    GtkWidget *dialog = gtk_dialog_new_with_buttons("Add User", GTK_WINDOW(window), GTK_DIALOG_MODAL,
                                                    "_OK", GTKGenericContainer_OK, "_Cancel", GTK_RESPONSE_CANCEL, NULL);
    GtkWidget *entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Enter username");
    gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))), entry, TRUE, TRUE, 5);
    gtk_widget_show_all(dialog);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
        const char *username = gtk_entry_get_text(GTK_ENTRY(entry));
        if (strlen(username) > 0) {
            char cmd[100] = "sudo useradd ";
            strcat(cmd, username);
            int result = system(cmd);
            GtkWidget *msg_dialog = gtk_message_dialog_new(GTK_WINDOW(window), GTK_DIALOG_MODAL,
                result == 0 ? GTK_MESSAGE_INFO : GTK_MESSAGE_ERROR, GTK_BUTTONS_OK,
                result == 0 ? "User added!" : "Failed to add user.");
            gtk_dialog_run(GTK_DIALOG(msg_dialog));
            gtk_widget_destroy(msg_dialog);
        }
    }
    gtk_widget_destroy(dialog);
}

// Callback for Delete User
void on_delete_user_clicked(GtkButton *button, gpointer window) {
    GtkWidget *dialog = gtk_dialog_new_with_buttons("Delete User", GTK_WINDOW(window), GTK_DIALOG_MODAL,
                                                    "_OK", GTK_RESPONSE_OK, "_Cancel", GTK_RESPONSE_CANCEL, NULL);
    GtkWidget *entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Enter username");
    gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))), entry, TRUE, TRUE, 5);
    gtk_widget_show_all(dialog);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
        const char *username = gtk_entry_get_text(GTK_ENTRY(entry));
        if (strlen(username) > 0) {
            char cmd[100] = "sudo userdel ";
            strcat(cmd, username);
            int result = system(cmd);
            GtkWidget *msg_dialog = gtk_message_dialog_new(GTK_WINDOW(window), GTK_DIALOG_MODAL,
                result == 0 ? GTK_MESSAGE_INFO : GTK_MESSAGE_ERROR, GTK_BUTTONS_OK,
                result == 0 ? "User deleted!" : "Failed to delete user.");
            gtk_dialog_run(GTK_DIALOG(msg_dialog));
            gtk_widget_destroy(msg_dialog);
        }
    }
    gtk_widget_destroy(dialog);
}

// Callback for Add Group
void on_add_group_clicked(GtkButton *button, gpointer window) {
    GtkWidget *dialog = gtk_dialog_new_with_buttons("Add Group", GTK_WINDOW(window), GTK_DIALOG_MODAL,
                                                    "_OK", GTK_RESPONSE_OK, "_Cancel", GTK_RESPONSE_CANCEL, NULL);
    GtkWidget *entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Enter group name");
    gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))), entry, TRUE, TRUE, 5);
    gtk_widget_show_all(dialog);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
        const char *group = gtk_entry_get_text(GTK_ENTRY(entry));
        if (strlen(group) > 0) {
            char cmd[100] = "sudo groupadd ";
            strcat(cmd, group);
            int result = system(cmd);
            GtkWidget *msg_dialog = gtk_message_dialog_new(GTK_WINDOW(window), GTK_DIALOG_MODAL,
                result == 0 ? GTK_MESSAGE_INFO : GTK_MESSAGE_ERROR, GTK_BUTTONS_OK,
                result == 0 ? "Group added!" : "Failed to add group.");
            gtk_dialog_run(GTK_DIALOG(msg_dialog));
            gtk_widget_destroy(msg_dialog);
        }
    }
    gtk_widget_destroy(dialog);
}

// Callback for Delete Group
void on_delete_group_clicked(GtkButton *button, gpointer window) {
    GtkWidget *dialog = gtk_dialog_new_with_buttons("Delete Group", GTK_WINDOW(window), GTK_DIALOG_MODAL,
                                                    "_OK", GTK_RESPONSE_OK, "_Cancel", GTK_RESPONSE_CANCEL, NULL);
    GtkWidget *entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Enter group name");
    gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))), entry, TRUE, TRUE, 5);
    gtk_widget_show_all(dialog);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
        const char *group = gtk_entry_get_text(GTK_ENTRY(entry));
        if (strlen(group) > 0) {
            char cmd[100] = "sudo groupdel ";
            strcat(cmd, group);
            int result = system(cmd);
            GtkWidget *msg_dialog = gtk_message_dialog_new(GTK_WINDOW(window), GTK_DIALOG_MODAL,
                result == 0 ? GTK_MESSAGE_INFO : GTK_MESSAGE_ERROR, GTK_BUTTONS_OK,
                result == 0 ? "Group deleted!" : "Failed to delete group.");
            gtk_dialog_run(GTK_DIALOG(msg_dialog));
            gtk_widget_destroy(msg_dialog);
        }
    }
    gtk_widget_destroy(dialog);
}

// Main function
int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    // Create main window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Simple User Manager");
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create vertical box for buttons
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), box);
    gtk_container_set_border_width(GTK_CONTAINER(box), 10);

    // Create buttons
    GtkWidget *btn_add_user = gtk_button_new_with_label("Add User");
    GtkWidget *btn_delete_user = gtk_button_new_with_label("Delete User");
    GtkWidget *btn_add_group = gtk_button_new_with_label("Add Group");
    GtkWidget *btn_delete_group = gtk_button_new_with_label("Delete Group");

    // Connect buttons to callbacks
    g_signal_connect(btn_add_user, "clicked", G_CALLBACK(on_add_user_clicked), window);
    g_signal_connect(btn_delete_user, "clicked", G_CALLBACK(on_delete_user_clicked), window);
    g_signal_connect(btn_add_group, "clicked", G_CALLBACK(on_add_group_clicked), window);
    g_signal_connect(btn_delete_group, "clicked", G_CALLBACK(on_delete_group_clicked), window);

    // Add buttons to box
    gtk_box_pack_start(GTK_BOX(box), btn_add_user, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(box), btn_delete_user, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(box), btn_add_group, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(box), btn_delete_group, TRUE, TRUE, 5);

    // Show all widgets
    gtk_widget_show_all(window);

    // Start main loop
    gtk_main();

    return 0;
}