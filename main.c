/*Documentation
Name : Veluru Ramesh Tharun
Registration No : 25031_113
Start Date : 08-12-2025
End Date : 14-12-2025
Description : I have done the project called address book which has the operations like Create contact, Search contact, Edit contact, Delete contact,
List contact, Save and exit. To implement this project i have used the concepts like functions, strings, structures, pointers, loops, files. 
After completing this project i have gained more clarity about the all concepts that i have implemented in this project.
*/

#define RESET "\033[0m"
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"

#include <stdio.h>
#include "contact.h"
#include "file.h"

int main()
{
    int choice;              // Variable to store user menu choice
    AddressBook addressBook; // Structure to hold all contacts

    initialize(&addressBook); // Initialize address book and load contacts from file

    do
    {
        printf("\nAddress Book Menu:\n"); // Display menu
        printf("1. Create contact\n");
        printf("2. Search contact\n");
        printf("3. Edit contact\n");
        printf("4. Delete contact\n");
        printf("5. List all contacts\n");
        printf("6. Save and Exit\n");
        printf("Enter your choice: ");

        scanf("%d", &choice);
        getchar(); // Clear newline from input buffer

        switch (choice) // Perform action based on choice
        {
        case 1:
            createContact(&addressBook);
            break;
        case 2:
            searchContact(&addressBook);
            break;
        case 3:
            editContact(&addressBook);
            break;
        case 4:
            deleteContact(&addressBook);
            break;
        case 5:
        {
            int sortchoice;                    // Variable for sorting option
            printf("Select sort criteria:\n"); // Ask sorting preference
            printf("1.Sort by name\n");
            printf("2.Sort by phone\n");
            printf("3.Sort by email\n");
            scanf("%d", &sortchoice);               // Read sorting option
            listContacts(&addressBook, sortchoice); // Display sorted contacts
            break;
        }
        case 6:
        {
            int sortChoice;
            printf("Before saving, sort contacts by:\n");
            printf("1. Name\n");
            printf("2. Phone\n");
            printf("3. Email\n");
            printf("Enter choice: ");

            scanf("%d", &sortChoice);
            getchar();

            // Sort first in the file
            listContacts(&addressBook, sortChoice);

            // Then Save the file
            saveContactsToFile(&addressBook);

            printf(GREEN"Contacts saved successfully. Exiting...\n"RESET);
            break;
        }
        default:
            printf(RED"Invalid choice! Please try again.\n"RESET);
        }
    } while (choice != 6); // Repeat until exit option is chosen

    return 0; // Terminate program successfully
}