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
void cancelBooking();
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
        printf("\t\t4. Cancel Bookings\n");
        printf("\t\t5. Generate Invoice\n");
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
                cancelBooking();  // Cancel a booking
                break;
            case 5:
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

// Function to book a room
void bookRooms() {
    FILE *Br, *fp;
    struct Booking bookings;
    int roomAvailable = 1;  // Assume room is available

    Br = fopen("Bookings.txt", "a");
    if (Br == NULL) {
        printf("Error opening bookings file.\n");
        return;
    }

    // Get booking details
    printf("Enter Room Number: ");
    scanf("%s", bookings.roomNum);
    printf("Enter Customer Name: ");
    scanf("%s", bookings.name);
    printf("Enter Contact Number: ");
    scanf("%s", bookings.contact);
    printf("Enter Number of Days: ");
    scanf("%d", &bookings.totalDays);
    bookings.paymentStatus = 0;  // Initial payment status is pending

    // Check if room is available
    FILE *fpRooms = fopen("Rooms.txt", "r");
    struct Rooms room;
    int roomFound = 0;

    while (fread(&room, sizeof(struct Rooms), 1, fpRooms)) {
        if (strcmp(room.roomNum, bookings.roomNum) == 0) {
            roomFound = 1;
            break;
        }
    }

    fclose(fpRooms);

    if (roomFound) {
        // Check if the room is already booked
        FILE *fpBookings = fopen("Bookings.txt", "r");
        struct Booking bookedRoom;
        while (fread(&bookedRoom, sizeof(struct Booking), 1, fpBookings)) {
            if (strcmp(bookedRoom.roomNum, bookings.roomNum) == 0) {
                roomAvailable = 0;  // Room is already booked
                break;
            }
        }
        fclose(fpBookings);

        if (roomAvailable) {
            // Add booking to the bookings file
            fwrite(&bookings, sizeof(struct Booking), 1, Br);
            printf("Room booked successfully.\n");

            // Now remove the booked room from the Rooms.txt file
            FILE *fpTemp = fopen("temp.txt", "w");
            fpRooms = fopen("Rooms.txt", "r");

            if (fpRooms == NULL || fpTemp == NULL) {
                printf("Error opening rooms file.\n");
                fclose(fpRooms);
                fclose(fpTemp);
                return;
            }

            while (fread(&room, sizeof(struct Rooms), 1, fpRooms)) {
                // Copy only rooms that are not booked
                if (strcmp(room.roomNum, bookings.roomNum) != 0) {
                    fwrite(&room, sizeof(struct Rooms), 1, fpTemp);
                }
            }

            fclose(fpRooms);
            fclose(fpTemp);

            // Replace the original Rooms.txt with the updated file
            remove("Rooms.txt");
            rename("temp.txt", "Rooms.txt");

        } else {
            printf("Sorry, this room is already booked.\n");
        }
    } else {
        printf("Room not available in the system.\n");
    }

    fclose(Br);
}

// Function to check available rooms (not booked)
void isAvailable() {
    FILE *fp;
    struct Rooms info;
    fp = fopen("Rooms.txt", "r");

    if (fp == NULL) {
        printf("Error reading rooms file.\n");
        return;
    }

    printf("\nAvailable Rooms:\n");
    
    int roomsFound = 0;  // Flag to check if rooms exist in the file
    while (fread(&info, sizeof(struct Rooms), 1, fp)) {
        // Check if the room is already booked by checking in the Bookings file
        FILE *fpBookings = fopen("Bookings.txt", "r");
        struct Booking bookings;
        int isBooked = 0;

        while (fread(&bookings, sizeof(struct Booking), 1, fpBookings)) {
            if (strcmp(bookings.roomNum, info.roomNum) == 0) {
                isBooked = 1;  // Room is already booked
                break;
            }
        }

        fclose(fpBookings);

        // If the room is not booked, print the room details
        if (!isBooked) {
            printf("\nRoom Number: %s", info.roomNum);
            printf("\nRoom Type: %s", info.type);
            printf("\nRoom Price: %d\n", info.price);
            printf("----------------------------\n");
            roomsFound = 1;
        }
    }

    if (!roomsFound) {
        printf("No rooms available.\n");
    }

    fclose(fp);
}

// Function to view all booked rooms
void booked_Rooms() {
    FILE *Br;
    struct Booking bookings;
    Br = fopen("Bookings.txt", "r");

    // Check if the bookings file opened successfully
    if (Br == NULL) {
        printf("No bookings found.\n");
        return;
    }

    // Variable to track if any booking is found
    int found = 0;

    printf("\nBooked Rooms:\n");

    // Read through the bookings and display them
    while (fread(&bookings, sizeof(struct Booking), 1, Br)) {
        printf("\nRoom Number      : %s", bookings.roomNum);
        printf("\nCustomer Name    : %s", bookings.name);
        printf("\nCustomer Contact : %s", bookings.contact);
        printf("\nTotal Days       : %d", bookings.totalDays);
        printf("\n----------------------------\n");
        found = 1;  // Mark as found at least one booking
    }

    // If no bookings were found
    if (!found) {
        printf("No bookings found.\n");
    }

    fclose(Br);
}

// Function to cancel a booking
void cancelBooking() {
    FILE *Br, *fp, *fpRooms;
    struct Booking bookings;
    struct Rooms room;
    char roomNum[5];
    int found = 0;

    printf("Enter room number to cancel booking: ");
    scanf("%s", roomNum);

    // Open the Bookings.txt file to read bookings
    Br = fopen("Bookings.txt", "r");
    fp = fopen("temp.txt", "w");  // Temporary file to write the updated bookings

    if (Br == NULL || fp == NULL) {
        printf("Error opening file.\n");
        return;
    }

    // Loop through the bookings and write everything except the canceled booking to the temp file
    while (fread(&bookings, sizeof(struct Booking), 1, Br)) {
        if (strcmp(bookings.roomNum, roomNum) != 0) {
            fwrite(&bookings, sizeof(struct Booking), 1, fp);
        } else {
            found = 1;  // Mark that the booking was found and cancelled
        }
    }

    fclose(Br);
    fclose(fp);

    if (found) {
        // Remove the original bookings file and rename the temp file to bookings file
        remove("Bookings.txt");
        rename("temp.txt", "Bookings.txt");

        // Now, we need to add the room back to Rooms.txt to make it available again
        fpRooms = fopen("Rooms.txt", "a");  // Open Rooms.txt for appending

        if (fpRooms == NULL) {
            printf("Error opening rooms file.\n");
            return;
        }

        // Re-open Rooms.txt to read the room details
        FILE *fpRoomDetails = fopen("Rooms.txt", "r");
        if (fpRoomDetails == NULL) {
            printf("Error reading rooms file.\n");
            return;
        }

        // Loop through the rooms to find the canceled room
        while (fread(&room, sizeof(struct Rooms), 1, fpRoomDetails)) {
            if (strcmp(room.roomNum, roomNum) == 0) {
                // Room found, re-add it to Rooms.txt
                fwrite(&room, sizeof(struct Rooms), 1, fpRooms);
                printf("Room added back to available rooms.\n");
                break;
            }
        }

        fclose(fpRoomDetails);
        fclose(fpRooms);

        printf("Booking cancelled successfully. Room is now available again.\n");
    } else {
        printf("Booking not found.\n");
    }
}

// Function to generate invoice for a booking
// Function to generate invoice for a booking
void generateInvoice() {
    FILE *Br, *fpRooms;
    struct Booking bookings;
    struct Rooms roomInfo;
    char roomNum[5];
    int foundBooking = 0, foundRoom = 0;
    
    // Get the room number for the invoice
    printf("Enter room number to generate invoice: ");
    scanf("%s", roomNum);

    // Open bookings file to find the specific booking
    Br = fopen("Bookings.txt", "r");
    if (Br == NULL) {
        printf("No bookings found.\n");
        return;
    }

    // Look for the booking that matches the provided room number
    while (fread(&bookings, sizeof(struct Booking), 1, Br)) {
        if (strcmp(bookings.roomNum, roomNum) == 0) {
            foundBooking = 1;

            // Open rooms file to fetch room price based on room number
            fpRooms = fopen("Rooms.txt", "r");
            if (fpRooms == NULL) {
                printf("Error reading rooms file.\n");
                fclose(Br);
                return;
            }

            // Find the corresponding room in Rooms.txt to get the price
            while (fread(&roomInfo, sizeof(struct Rooms), 1, fpRooms)) {
                if (strcmp(roomInfo.roomNum, bookings.roomNum) == 0) {
                    // Generate the invoice using the room price and totalDays
                    printf("\n----- Invoice -----\n");
                    printf("Room Number            : %s\n", bookings.roomNum);
                    printf("Customer Name          : %s\n", bookings.name);
                    printf("Customer Contact       : %s\n", bookings.contact);
                    printf("Total Days             : %d\n", bookings.totalDays);
                    printf("Room Price (per night) : %d\n", roomInfo.price);
                    printf("Total Amount           : %d\n", bookings.totalDays * roomInfo.price);
                    
                    fclose(fpRooms); 
                    fclose(Br);
                    return;
                }
            }

            // If room not found in Rooms.txt
            if (!foundRoom) {
                printf("Room price not found for room number %s.\n", roomNum);
            }
            fclose(fpRooms);
            break;  // Exit after finding the booking
        }
    }

    // If no booking was found
    if (!foundBooking) {
        printf("Booking not found for room number %s.\n", roomNum);
    }

    fclose(Br);
}