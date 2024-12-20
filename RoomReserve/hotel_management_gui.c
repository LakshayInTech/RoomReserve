#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>

// Structure to hold room information
struct Room {
    char roomNum[10];
    char type[20];
    int price;
    char status[10]; // "Booked" or "Unbooked"
};

// Structure to hold customer details and the room selection list
struct BookingData {
    GtkWidget *entry_name;
    GtkWidget *entry_contact;
    GtkWidget *entry_days;
    GtkWidget *list_box;
};

// Function prototypes
static void manage_rooms(GtkWidget *widget, gpointer data);
static void book_room(GtkWidget *widget, gpointer data);
static void generate_invoices(GtkWidget *widget, gpointer data);
static void view_payment_records(GtkWidget *widget, gpointer data);
static void show_history(GtkWidget *widget, gpointer data);
static GtkWidget* create_button(const char *label_text, GCallback callback);
static void add_room(GtkWidget *widget, gpointer dialog);
static void view_rooms(GtkWidget *widget, gpointer dialog);
static void book_room_confirm(GtkWidget *widget, gpointer data);

// Main function
int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Room Reserve");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    GtkWidget *title_label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(title_label), "<span font='20'>Room Reserve</span>");
    gtk_box_pack_start(GTK_BOX(vbox), title_label, FALSE, FALSE, 20);

    GtkWidget *manage_btn = create_button("Manage Rooms", G_CALLBACK(manage_rooms));
    GtkWidget *book_btn = create_button("Book Room", G_CALLBACK(book_room));
    GtkWidget *invoice_btn = create_button("Generate Invoices", G_CALLBACK(generate_invoices));
    GtkWidget *payment_btn = create_button("View Payment Records", G_CALLBACK(view_payment_records));
    GtkWidget *history_btn = create_button("Show History", G_CALLBACK(show_history));

    gtk_box_pack_start(GTK_BOX(vbox), manage_btn, FALSE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX(vbox), book_btn, FALSE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX(vbox), invoice_btn, FALSE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX(vbox), payment_btn, FALSE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX(vbox), history_btn, FALSE, FALSE, 10);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}

// Function to create a button with label
static GtkWidget* create_button(const char *label_text, GCallback callback) {
    GtkWidget *button = gtk_button_new_with_label(label_text);
    gtk_widget_set_size_request(button, 200, 50);
    g_signal_connect(button, "clicked", callback, NULL);
    return button;
}

// Manage Rooms function
static void manage_rooms(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog = gtk_dialog_new_with_buttons("Manage Rooms", NULL, GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT, "Close", GTK_RESPONSE_CLOSE, NULL);
    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

    gtk_box_pack_start(GTK_BOX(content_area), box, FALSE, FALSE, 10);

    GtkWidget *add_button = gtk_button_new_with_label("Add Room");
    g_signal_connect(add_button, "clicked", G_CALLBACK(add_room), NULL);
    gtk_box_pack_start(GTK_BOX(box), add_button, FALSE, FALSE, 10);

    GtkWidget *view_button = gtk_button_new_with_label("View Rooms");
    g_signal_connect(view_button, "clicked", G_CALLBACK(view_rooms), NULL);
    gtk_box_pack_start(GTK_BOX(box), view_button, FALSE, FALSE, 10);

    gtk_widget_show_all(dialog);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

// Add Room function
static void add_room(GtkWidget *widget, gpointer dialog) {
    GtkWidget *add_dialog = gtk_dialog_new_with_buttons("Add Room", NULL, GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT, "Add", GTK_RESPONSE_OK, "Cancel", GTK_RESPONSE_CANCEL, NULL);
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(add_dialog))), box);

    GtkWidget *entry_room_num = gtk_entry_new();
    GtkWidget *entry_type = gtk_entry_new();
    GtkWidget *entry_price = gtk_entry_new();

    gtk_box_pack_start(GTK_BOX(box), gtk_label_new("Room Number:"), FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(box), entry_room_num, FALSE, FALSE, 5);

    gtk_box_pack_start(GTK_BOX(box), gtk_label_new("Room Type:"), FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(box), entry_type, FALSE, FALSE, 5);

    gtk_box_pack_start(GTK_BOX(box), gtk_label_new("Room Price:"), FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(box), entry_price, FALSE, FALSE, 5);

    gtk_widget_show_all(add_dialog);

    if (gtk_dialog_run(GTK_DIALOG(add_dialog)) == GTK_RESPONSE_OK) {
        struct Room room;
        strncpy(room.roomNum, gtk_entry_get_text(GTK_ENTRY(entry_room_num)), sizeof(room.roomNum) - 1);
        strncpy(room.type, gtk_entry_get_text(GTK_ENTRY(entry_type)), sizeof(room.type) - 1);
        room.price = atoi(gtk_entry_get_text(GTK_ENTRY(entry_price)));
        strncpy(room.status, "Unbooked", sizeof(room.status) - 1);

        FILE *file = fopen("Rooms.txt", "a");
        if (file) {
            fwrite(&room, sizeof(struct Room), 1, file);
            fclose(file);
        }
    }

    gtk_widget_destroy(add_dialog);
}

// View Rooms function
static void view_rooms(GtkWidget *widget, gpointer dialog) {
    GtkWidget *view_dialog = gtk_dialog_new_with_buttons("View Rooms", NULL, GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT, "Close", GTK_RESPONSE_CLOSE, NULL);
    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(view_dialog));
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

    gtk_box_pack_start(GTK_BOX(content_area), box, FALSE, FALSE, 10);

    FILE *file = fopen("Rooms.txt", "r");
    struct Room room;

    if (file) {
        while (fread(&room, sizeof(struct Room), 1, file) == 1) {
            GtkWidget *label = gtk_label_new(g_strdup_printf("Room %s: %s, Price: %d, Status: %s", room.roomNum, room.type, room.price, room.status));
            gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 5);
        }
        fclose(file);
    }

    gtk_widget_show_all(view_dialog);
    gtk_dialog_run(GTK_DIALOG(view_dialog));
    gtk_widget_destroy(view_dialog);
}

// Book Room function
static void book_room(GtkWidget *widget, gpointer data) {
    // Booking room logic
    GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Book room functionality goes here.");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

// Generate Invoices function
static void generate_invoices(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Generate invoices functionality.");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

// View Payment Records function
static void view_payment_records(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "View payment records functionality.");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

// Show History function
static void show_history(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Show history functionality.");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}
