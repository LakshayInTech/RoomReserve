#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

// Structure to store room details
struct Rooms {
    char roomNum[5];  // Room number (max 4 characters)
    char type[10];    // Type of room (e.g., "Single", "Double")
    int price;        // Price per night
};

// Structure to store booking details
struct Booking {
    char roomNum[5];  // Room number being booked
    char name[15];    // Customer's name
    char contact[11]; // Customer's contact number
    int totalDays;    // Number of days the room is booked for
    int paymentStatus;  // 0 for pending, 1 for paid
};

// Function prototypes for various functionalities
void manageRooms();
void manageBookings();
void addRooms();
void viewRooms();
void deleteRooms();
void bookRooms();
void isAvailable();
void booked_Rooms();
void generateInvoice();

int main() {
    int choice = -1;  // Initialize with a non-zero value to enter the loop
    while (choice != 0) {
        system("cls");  // Clear the screen
        printf("\n\t\t\t\t------- ROOM RESERVATION SYSTEM -------\n\n");
        printf("\t\t1. Manage ROOMS\n");
        printf("\t\t2. Manage BOOKINGS\n");
        printf("\t\t0. Exit\n\n");
        printf("Enter your choice : ");
        scanf("%d", &choice);

        // Clear the input buffer to avoid issues with leftover newline characters
        while (getchar() != '\n');

        // Handle the user's choice
        switch (choice) {
        case 1:
            manageRooms();  // Manage room operations (add, view, delete rooms)
            break;
        case 2:
            manageBookings();  // Manage booking operations (book, view, cancel bookings)
            break;
        case 0:
            printf("\nExiting the program... \n");  // Exit message
            return 0;
        default:
            printf("\nInvalid Choice. Try Again...\n\n");
            break;
        }
    }
    return 0;  // Exit the program
}

// Function to manage room operations
void manageRooms() {
    int ch;
    system("cls");
    while (1) {
        printf("\n\t\t\t\t------ MANAGE ROOMS ------\n\n");
        printf("\t\t1. Add Rooms\n");
        printf("\t\t2. View Rooms\n");
        printf("\t\t3. Delete Rooms\n");
        printf("\t\t0. Exit\n");
        printf("\nEnter your choice : ");
        scanf("%d", &ch);

        // Clear the input buffer to avoid issues with leftover newline characters
        while (getchar() != '\n');

        switch (ch) {
            case 1:
                addRooms();  // Add new rooms
                break;
            case 2:
                viewRooms();  // View all rooms
                break;
            case 3:
                deleteRooms();  // Delete a room
                break;
            case 0:
                return;  // Exit the manageRooms() function and return to the main menu
            default:
                printf("\nInvalid choice. Try again...\n");
                break;
        }
    }
}

// Function to manage booking operations
void manageBookings() {
    int ch;
    system("cls");
    while (1) {
        printf("\n\t\t\t\t--- Booking Management ---\n\n");
        printf("\t\t1. Book Rooms\n");
        printf("\t\t2. Check Available Rooms\n");
        printf("\t\t3. Check Booked Rooms\n");
        printf("\t\t4. Generate Invoice\n");
        printf("\t\t0. Exit\n");
        printf("\nEnter your choice : ");
        scanf("%d", &ch);

        // Clear the input buffer to avoid issues with leftover newline characters
        while (getchar() != '\n');

        switch (ch) {
            case 1:
                bookRooms();  // Book a room
                break;
            case 2:
                isAvailable();  // Check available rooms
                break;
            case 3:
                booked_Rooms();  // Check booked rooms
                break;
            case 4:
                generateInvoice();  // Generate invoice for a booking
                break;
            case 0:
                return;  // Exit the manageBookings() function and return to the main menu
            default:
                printf("\nInvalid choice. Try again...\n");
                break;
        }
    }
}

// Function to add new rooms to the system
void addRooms() {
    char another;
    FILE *fp;
    struct Rooms info;

    fp = fopen("Rooms.txt", "a");  // Open file in append mode
    if (fp == NULL) {
        fprintf(stderr, "Error: Can't open file for writing.\n");
        return;
    }

    do {
        printf("\n====== ADD ROOMS ======\n\n");

        printf("Enter Room number: \n");
        scanf("%s", info.roomNum);
        printf("Enter the room type: \n");
        scanf("%s", info.type);
        printf("Enter price per night: \n");
        scanf("%d", &info.price);

        fwrite(&info, sizeof(struct Rooms), 1, fp);  // Write room details to the file

        printf("Record added successfully.\n");

        printf("Do you want to add another Room (y/n): ");
        scanf(" %c", &another);  // Note the space before %c to handle newline character
        
        // Clear the input buffer after each input to prevent issues with leftover newlines
        while (getchar() != '\n');
        
    } while (another == 'y' || another == 'Y');

    fclose(fp);  // Close the file
}

// Function to view all the rooms in the system
void viewRooms() {
    FILE *fp;
    struct Rooms info;
    fp = fopen("Rooms.txt", "r");

    if (fp == NULL) {
        printf("No rooms available or file error.\n");
        return;
    }

    printf("\n====== VIEW ROOMS ======\n\n");

    int roomsFound = 0;  // Flag to check if rooms exist in the file
    while (fread(&info, sizeof(struct Rooms), 1, fp)) {
        printf("\nRoom Number :  %s", info.roomNum);
        printf("\nRoom Type   :  %s", info.type);
        printf("\nRoom Price  :  %d\n", info.price);
        printf("----------------------------\n");
        roomsFound = 1;
    }

    if (!roomsFound) {
        printf("No rooms found in the file.\n");
    }

    fclose(fp);  // Close the file
}

// Function to delete a room from the system
void deleteRooms() {
    FILE *fp, *fp1;
    struct Rooms info;
    char room_Num[5];
    int found = 0;

    printf("Enter room number to delete: ");
    scanf("%s", room_Num);

    fp = fopen("Rooms.txt", "r");
    fp1 = fopen("temp.txt", "w");

    if (fp == NULL || fp1 == NULL) {
        printf("Error opening file.\n");
        return;
    }

    while (fread(&info, sizeof(struct Rooms), 1, fp)) {
        if (strcmp(info.roomNum, room_Num) != 0) {
            fwrite(&info, sizeof(struct Rooms), 1, fp1);  // Copy room details to temp file
        } else {
            found = 1;
        }
    }

    fclose(fp);
    fclose(fp1);

    if (found) {
        remove("Rooms.txt");  // Delete the original rooms file
        rename("temp.txt", "Rooms.txt");  // Rename the temp file as Rooms.txt
        printf("Room deleted successfully.\n");
    } else {
        printf("Room not found.\n");
    }
}

// Function to check available rooms (not booked)
void isAvailable() {
    FILE *fpRooms, *fpBookings;
    struct Rooms roomInfo;
    struct Booking booking;

    fpRooms = fopen("Rooms.txt", "r");
    if (fpRooms == NULL) {
        printf("No rooms available in the system.\n");
        return;
    }

    printf("\n====== AVAILABLE ROOMS ======\n");

    while (fread(&roomInfo, sizeof(struct Rooms), 1, fpRooms)) {
        int isBooked = 0;

        fpBookings = fopen("Bookings.txt", "r");
        if (fpBookings != NULL) {
            while (fread(&booking, sizeof(struct Booking), 1, fpBookings)) {
                if (strcmp(roomInfo.roomNum, booking.roomNum) == 0) {
                    isBooked = 1;
                    break;
                }
            }
            fclose(fpBookings);
        }

        if (!isBooked) {
            printf("\nRoom Number: %s\n", roomInfo.roomNum);
            printf("Room Type: %s\n", roomInfo.type);
            printf("Room Price: %d\n", roomInfo.price);
            printf("----------------------------\n");
        }
    }

    fclose(fpRooms);
}

// Function to view all booked rooms
void bookRooms() {
    FILE *Br;
    struct Booking newBooking;

    // Input details for the new booking
    printf("Enter Room Number: ");
    scanf("%s", newBooking.roomNum);
    printf("Enter Customer Name: ");
    getchar(); // To clear newline character
    fgets(newBooking.name, sizeof(newBooking.name), stdin);
    strtok(newBooking.name, "\n"); // Remove trailing newline
    printf("Enter Contact Number: ");
    scanf("%s", newBooking.contact);
    printf("Enter Total Days of Stay: ");
    scanf("%d", &newBooking.totalDays);
    printf("Enter Payment Status (1 for Paid, 0 for Pending): ");
    scanf("%d", &newBooking.paymentStatus);

    // Open file to append the new booking
    Br = fopen("Bookings.txt", "ab");
    if (Br == NULL) {
        printf("Error: Unable to open bookings file.\n");
        return;
    }

    // Write the new booking to the file
    fwrite(&newBooking, sizeof(struct Booking), 1, Br);
    printf("\nRoom booked successfully!\n");

    fclose(Br);
}

// Function to display booked rooms
void booked_Rooms() {
    FILE *Br;
    struct Booking bookings;

    Br = fopen("Bookings.txt", "rb");
    if (Br == NULL) {
        printf("\nNo bookings have been made yet.\n");
        return;
    }

    printf("\n======= BOOKED ROOMS =======\n");
    printf("| Room No   | Customer Name        | Contact         | Days  | Payment    |\n");
    printf("---------------------------------------------------------------------------\n");

    int roomsFound = 0;
    while (fread(&bookings, sizeof(struct Booking), 1, Br)) {
        printf("| %-9s | %-20s | %-15s | %-5d | %-10s |\n",
               bookings.roomNum,
               bookings.name,
               bookings.contact,
               bookings.totalDays,
               bookings.paymentStatus ? "Paid" : "Pending");
        roomsFound = 1;
    }

    if (!roomsFound) {
        printf("\nNo rooms have been booked yet.\n");
    }

    printf("=======================================\n");

    fclose(Br);
}

// Function to generate invoice for a booking
void generateInvoice() {
    FILE *bookingFile, *roomFile;
    struct Booking booking;
    struct Rooms room;
    char roomNum[5];
    int bookingFound = 0;

    printf("Enter room number to generate invoice: ");
    scanf("%s", roomNum);

    // Open the bookings file
    bookingFile = fopen("Bookings.txt", "r");
    if (bookingFile == NULL) {
        printf("Error: Unable to open bookings file.\n");
        return;
    }

    // Search for the booking with the given room number
    while (fread(&booking, sizeof(struct Booking), 1, bookingFile)) {
        if (strcmp(booking.roomNum, roomNum) == 0) {
            bookingFound = 1;

            // Open the rooms file to find the corresponding room details
            roomFile = fopen("Rooms.txt", "r");
            if (roomFile == NULL) {
                printf("Error: Unable to open rooms file.\n");
                fclose(bookingFile);
                return;
            }

            int roomFound = 0;
            while (fread(&room, sizeof(struct Rooms), 1, roomFile)) {
                if (strcmp(room.roomNum, roomNum) == 0) {
                    roomFound = 1;

                    // Generate and display the invoice
                    printf("\n------ INVOICE ------\n");
                    printf("Room Number   : %s\n", room.roomNum);
                    printf("Room Type     : %s\n", room.type);
                    printf("Customer Name : %s\n", booking.name);
                    printf("Contact       : %s\n", booking.contact);
                    printf("Total Days    : %d\n", booking.totalDays);
                    printf("Price per Day : %d\n", room.price);
                    printf("Total Amount  : %d\n", booking.totalDays * room.price);
                    printf("Payment Status: %s\n", booking.paymentStatus ? "Paid" : "Pending");
                    printf("----------------------\n");

                    fclose(roomFile);
                    fclose(bookingFile);
                    return;
                }
            }

            fclose(roomFile);

            // If room details are not found
            if (!roomFound) {
                printf("Room details for room number %s not found in Rooms.txt.\n", roomNum);
            }

            break; // Break out of the loop after finding the booking
        }
    }

    fclose(bookingFile);

    // Handle case where no booking was found
    if (!bookingFound) {
        printf("No booking found for room number %s.\n", roomNum);
    }
}