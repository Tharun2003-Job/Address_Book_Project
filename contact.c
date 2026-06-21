#include <stdio.h>   // Standard input-output library
#include <string.h>  // String handling functions
#include <ctype.h>   // Character handling functions
#include "contact.h" // Contact-related declarations
#include "file.h"    // File-related declarations

#define RESET "\033[0m"
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"

// Initialize address book by loading contacts from file
void initialize(AddressBook *addressBook)
{
    FILE *fptr = fopen("contacts.csv", "r"); // Open contacts file in read mode
    if (fptr == NULL)
    {
        addressBook->contactCount = 0; // No file → start with empty address book
        return;                        // if the file not found then start it by fresh
    }

    int totalContacts = 0;
    fscanf(fptr, "#%d\n", &totalContacts); // Read total contacts from first line

    if (totalContacts > MAX_CONTACTS) // Prevent overflow (maximum contacts is 100)
    {
        totalContacts = MAX_CONTACTS;
    }
    for (int i = 0; i < totalContacts; i++)
    {
        fscanf(fptr, "%[^,],%[^,],%[^\n]\n", // Read name, phone, email
               addressBook->contacts[i].name,
               addressBook->contacts[i].phone,
               addressBook->contacts[i].email);
    }

    addressBook->contactCount = totalContacts; // Update contact count
    fclose(fptr);                              // Close file
}

// Save all contacts to file before exit
void saveAndExit(AddressBook *addressBook)
{
    FILE *fptr = fopen("contacts.csv", "w"); // Open file in write mode
    if (fptr == NULL)
    {
        return; // Exit if file cannot open
    }

    fprintf(fptr, "#%d\n", addressBook->contactCount); // Write contact count

    // Then write each contact
    for (int i = 0; i < addressBook->contactCount; i++)
    {
        fprintf(fptr, "%s,%s,%s\n", // Write each contact
                addressBook->contacts[i].name,
                addressBook->contacts[i].phone,
                addressBook->contacts[i].email);
    }
    fclose(fptr); // Close file
}

// Create a new contact
void createContact(AddressBook *addressBook)
{
    if (addressBook->contactCount >= MAX_CONTACTS) // Checks if the address book is full
    {
        printf(RED "Address Book is FULL!\n" RESET);
        return;
    }

    // Temporary input buffers to store user input before vaidation
    char name_str[50], phone_str[20], email_str[50];

    while (1) // Loop until valid name
    {
        printf("Enter name: ");
        fgets(name_str, sizeof(name_str), stdin); // Read name
        name_str[strcspn(name_str, "\n")] = 0;    // Remove the new line

        if (validate_name(name_str)) // Validate name
            break;
        printf(RED "Invalid name! Only letters, spaces, and dots allowed.\n" RESET);
    }

    while (1) // Loop until valid phone
    {
        printf("Enter phone: ");
        fgets(phone_str, sizeof(phone_str), stdin);
        phone_str[strcspn(phone_str, "\n")] = 0;

        if (validate_phonenum(phone_str, addressBook)) // Validate phone
            break;
        printf(RED "Invalid phone! Only digits allowed and must be unique.\n" RESET);
    }

    while (1) // Loop until valid email
    {
        printf("Enter email: ");
        fgets(email_str, sizeof(email_str), stdin);
        email_str[strcspn(email_str, "\n")] = 0;

        if (validate_mail(email_str, addressBook)) // Validate email
            break;
        printf(RED "Invalid email! Must be valid format and unique.\n" RESET);
    }

    strcpy(addressBook->contacts[addressBook->contactCount].name, name_str);   // Store name
    strcpy(addressBook->contacts[addressBook->contactCount].phone, phone_str); // Store phone
    strcpy(addressBook->contacts[addressBook->contactCount].email, email_str); // Store email

    addressBook->contactCount++; // Increment contact count

    printf(GREEN "Contact added successfully!\n" RESET);
}

// Validate name (letters, space, dot only)
int validate_name(char name_str[])
{
    if (strlen(name_str) == 0) // Check empty string
    {
        return 0;
    }

    for (int i = 0; name_str[i] != '\0'; i++) // Check Upto Null Character
    {
        if (!isalpha(name_str[i]) && name_str[i] != ' ' && name_str[i] != '.') // Invalid char
        {
            return 0;
        }
    }

    return 1; // Name is valid
}

// Validate phone number
int validate_phonenum(char phonenum[], AddressBook *addressBook)
{
    int len = strlen(phonenum); // Get length

    if (len != 10) // Minimum length check
    {
        return 0;
    }

    for (int i = 0; i < len; i++) // Check each character
    {
        if (!isdigit(phonenum[i])) // Non-digit found
        {
            return 0;
        }
    }

    for (int i = 0; i < addressBook->contactCount; i++) // Check duplicates
    {
        if (strcmp(phonenum, addressBook->contacts[i].phone) == 0)
        {
            printf(RED "Phone number already exists!\n" RESET); // Duplicate found
            return 0;
        }
    }

    return 1; // Phone number is valid
}

// Validate email address
int validate_mail(char mail[], AddressBook *addressBook)
{
    int len = strlen(mail); // Get email length

    if (len < 6) // Minimum length check
        return 0;

    if (mail[0] == '.' || mail[0] == '@' || isdigit(mail[0])) // Invalid start
        return 0;

    int at_count = 0, at_index = -1; // '@' tracking variables

    for (int i = 0; i < len; i++)
    {
        if (!isalnum(mail[i]) && mail[i] != '.' && mail[i] != '@') // Invalid character
            return 0;

        if (mail[i] == '@') // '@' found
        {
            at_count++;   // Increment count
            at_index = i; // Store position
        }

        if (i < len - 1 && mail[i] == '.' && mail[i + 1] == '.') // Double dot
            return 0;
    }

    if (at_count != 1) // Must contain exactly one '@'
        return 0;

    if (at_index == 0 || at_index == len - 1) // '@' position check
        return 0;

    int dot_after_at = 0;                    // Dot flag after '@'
    for (int i = at_index + 1; i < len; i++) // Scan domain part
    {
        if (mail[i] == '.') // Dot found
        {
            dot_after_at = 1;
            break;
        }
    }

    if (!dot_after_at) // No dot after '@'
        return 0;

    for (int i = 0; i < addressBook->contactCount; i++) // Check duplicates
        if (strcmp(mail, addressBook->contacts[i].email) == 0)
        {
            printf(RED "Email is already found.\n" RESET);
            return 0; // Duplicate email
        }

    return 1; // Email is valid
}

void listContacts(AddressBook *addressBook, int sortCriteria)
{
    Contact temp; // temporary variable for swapping contacts

    // Outer loop to traverse each contact
    for (int i = 0; i < addressBook->contactCount - 1; i++)
    {
        // Inner loop to compare current contact with remaining contacts
        for (int j = i + 1; j < addressBook->contactCount; j++)
        {
            int result = 0; // store comparison result

            // Determine sorting criteria
            switch (sortCriteria)
            {
            case 1:
                result = strcasecmp(addressBook->contacts[i].name,
                                    addressBook->contacts[j].name); // compare names
                break;

            case 2:
                result = strcmp(addressBook->contacts[i].phone,
                                addressBook->contacts[j].phone); // compare phone numbers
                break;

            case 3:
                result = strcasecmp(addressBook->contacts[i].email,
                                    addressBook->contacts[j].email); // compare emails
                break;

            default:
                printf(RED "Invalid sorting criteria.\n" RESET); // invalid option entered
                return;
            }

            // Swap contacts if they are in wrong order
            if (result > 0)
            {
                temp = addressBook->contacts[i];
                addressBook->contacts[i] = addressBook->contacts[j];
                addressBook->contacts[j] = temp;
            }
        }
    }

    printf("Contact List:\n"); // display heading
    printf("S.No ");
    print_Top(); // print table headers

    // Print all contacts after sorting
    for (int i = 0; i < addressBook->contactCount; i++)
    {
        printf("%-5d ", i + 1);                // print serial number
        print_Contact_Details(addressBook, i); // print each contact
    }
}

void searchContact(AddressBook *addressBook)
{
    int option, count, index = -1; // Initialize the option and count , selected contact index
    int arr[100];                  // array to store indices of matched contacts
    char buf[20];                  // buffer to read user input safely

    printf("Search Contact by\n");
    printf("1. Name\n");
    printf("2. Phone number\n");
    printf("3. Mail ID\n");

    printf("Enter your option: ");
    fgets(buf, sizeof(buf), stdin); // Read input as a string
    sscanf(buf, "%d", &option);     // convert input string to integer

    if (option == 1) // if search by name selected
    {
        count = searchbyname(addressBook, arr); // find all matching names
        if (count == -1)                        // if no match found
        {
            printf("Name not found\n");
            return;
        }

        printf("\nS.No ");
        print_Top();
        for (int i = 0; i < count; i++) // loop through matched contacts
        {
            printf("%-5d ", i + 1);                     // print serial number
            print_Contact_Details(addressBook, arr[i]); // print contact details
        }

        if (count == 1)     // if only one match exists
            index = arr[0]; // directly select the contact
        else                // if multiple matches found
        {
            int serial; // variable to store selected serial number
            while (1)   // loop until valid input
            {
                printf("\nEnter serial number to select contact: ");
                fgets(buf, sizeof(buf), stdin);
                if (sscanf(buf, "%d", &serial) == 1 && serial >= 1 && serial <= count)
                {
                    index = arr[serial - 1]; // map serial number to contact index
                    break;                   // exit loop
                }
                printf(RED "Invalid serial number! Please enter 1 to %d\n" RESET, count);
            }
        }

        printf("\nSelected Contact:\n");
        printf("S.No ");
        print_Top();
        printf("%-5d ", 1);                        // print serial number as 1
        print_Contact_Details(addressBook, index); // print selected contact
    }
    else if (option == 2) // if search by phone number selected
    {
        index = searchbynum(addressBook); // search contact by phone
        if (index == -1)                  // if phone not found
        {
            printf(RED "Number not found\n" RESET);
            return;
        }
        printf("S.No ");
        print_Top();
        printf("%-5d ", 1);
        print_Contact_Details(addressBook, index);
    }
    else if (option == 3) // if search by email selected
    {
        index = searchbymail(addressBook); // search contact by email
        if (index == -1)                   // if email not found
        {
            printf(RED "Email not found\n" RESET);
            return;
        }
        printf("S.No ");
        print_Top();
        printf("%-5d ", 1);
        print_Contact_Details(addressBook, index);
    }
    else // if invalid option entered
    {
        printf(RED "Invalid option!\n" RESET);
    }
}

int searchbyname(AddressBook *addressBook, int arr[])
{
    char name[50]; // buffer to store name to search
    int count = 0; // count of matching contacts

    printf("Enter name to search: ");
    getchar(); // clear leftover newline from input buffer
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0; // remove trailing newline

    for (int i = 0; i < addressBook->contactCount; i++) // loop through all contacts
    {
        if (strstr(addressBook->contacts[i].name, name) != NULL) // check substring match
        {
            arr[count] = i; // store index of matched contact
            count++;        // increment match count
        }
    }

    if (count == 0) // if no matches found
        return -1;  // return -1 indicating failure

    return count; // return number of matches found
}

int searchbynum(AddressBook *addressBook)
{
    char phone[20];
    printf("Enter phone number: ");
    fgets(phone, sizeof(phone), stdin);
    phone[strcspn(phone, "\n")] = 0;

    for (int i = 0; i < addressBook->contactCount; i++) // loop through contacts
    {
        if (strcmp(addressBook->contacts[i].phone, phone) == 0) // exact match check
            return i;                                           // return index if phone matches
    }
    return -1; // return -1 if phone not found
}

int searchbymail(AddressBook *addressBook)
{
    char email[50];
    printf("Enter email ID: ");
    fgets(email, sizeof(email), stdin);
    email[strcspn(email, "\n")] = 0;

    for (int i = 0; i < addressBook->contactCount; i++) // loop through contacts
    {
        if (strcmp(addressBook->contacts[i].email, email) == 0) // exact match check
            return i;                                           // return index if email matches
    }
    return -1; // return -1 if email not found
}

void editContact(AddressBook *addressBook)
{
    char buf[50];
    int ch, edit;
    int arr[100], count, index = -1;

    printf("Search by: 1.Name 2.Phone 3.Email : ");
    fgets(buf, sizeof(buf), stdin);
    sscanf(buf, "%d", &ch);

    if (ch == 1) // search by name
    {
        count = searchbyname(addressBook, arr); // find matching names
        if (count > 1)
        {
            printf("\nMatched Contacts:\n");
            printf("S.No  Name              Phone           Email\n");

            for (int i = 0; i < count; i++)
            {
                printf("%-4d ", i + 1);
                print_Contact_Details(addressBook, arr[i]);
            }
        }

        if (count == -1) // if no match found
            return;

        if (count == 1)     // if only one match
            index = arr[0]; // select that contact
        else                // if multiple matches
        {
            int serial; // variable for serial selection
            while (1)   // loop until valid input
            {
                printf("Multiple matches found. Enter serial number: ");
                fgets(buf, sizeof(buf), stdin);
                if (sscanf(buf, "%d", &serial) == 1 && serial >= 1 && serial <= count)
                {
                    index = arr[serial - 1]; // map serial to actual index
                    break;
                }
                printf(RED "Invalid serial! Enter 1 to %d\n" RESET, count);
            }
        }
    }
    else if (ch == 2)                      // search by phone number
        index = searchbynum(addressBook);  // get index by phone
    else if (ch == 3)                      // search by email ID
        index = searchbymail(addressBook); // get index by email

    if (index == -1) // if contact not found
    {
        printf("Contact not found\n");
        return;
    }

    printf("Edit: 1.Name 2.Phone 3.Email : ");
    fgets(buf, sizeof(buf), stdin);
    sscanf(buf, "%d", &edit);

    if (edit == 1) // edit name
    {
        while (1) // loop until valid name
        {
            printf("New name: ");
            fgets(buf, sizeof(buf), stdin);
            buf[strcspn(buf, "\n")] = 0;
            if (validate_name(buf)) // validate name
                break;
            printf(RED "Invalid name!\n" RESET);
        }
        strcpy(addressBook->contacts[index].name, buf); // update name
    }
    else if (edit == 2) // edit phone number
    {
        while (1) // loop until valid phone
        {
            printf("New phone: ");
            fgets(buf, sizeof(buf), stdin);
            buf[strcspn(buf, "\n")] = 0;
            if (validate_phonenum(buf, addressBook)) // validate phone
                break;
            printf(RED "Invalid phone!\n" RESET);
        }
        strcpy(addressBook->contacts[index].phone, buf); // update phone
    }
    else if (edit == 3) // edit email ID
    {
        while (1) // loop until valid email
        {
            printf("New email: ");
            fgets(buf, sizeof(buf), stdin);
            buf[strcspn(buf, "\n")] = 0;
            if (validate_mail(buf, addressBook)) // validate email
                break;                           // exit loop if valid
            printf(RED "Invalid email!\n" RESET);
        }
        strcpy(addressBook->contacts[index].email, buf); // update email
    }

    printf(GREEN "Contact updated successfully!\n" RESET);
}

void deleteContact(AddressBook *addressBook)
{
    char buf[50];
    int ch, index = -1, arr[100], count;

    printf("Delete by: 1.Name 2.Phone 3.Email : ");

    fgets(buf, sizeof(buf), stdin);
    sscanf(buf, "%d", &ch);

    if (ch == 1) // delete by name
    {
        count = searchbyname(addressBook, arr); // search contacts by name
        if (count > 1)
        {
            printf("\nMatched Contacts:\n");
            printf("S.No  Name              Phone           Email\n");

            for (int i = 0; i < count; i++)
            {
                printf("%-4d ", i + 1);
                print_Contact_Details(addressBook, arr[i]);
            }
        }
        if (count == -1) // if no match found
            return;

        if (count == 1)     // if only one match
            index = arr[0]; // select that contact
        else                // if multiple matches found
        {
            int serial; // variable to store serial number
            while (1)   // loop until valid serial is entered
            {
                printf("Multiple matches found. Enter serial number: ");
                fgets(buf, sizeof(buf), stdin);
                if (sscanf(buf, "%d", &serial) == 1 && serial >= 1 && serial <= count)
                {
                    index = arr[serial - 1]; // map serial to actual contact index
                    break;
                }
                printf(RED "Invalid serial! Enter 1 to %d\n" RESET, count);
            }
        }
    }
    else if (ch == 2)                      // delete by phone number
        index = searchbynum(addressBook);  // get contact index by phone
    else if (ch == 3)                      // delete by email ID
        index = searchbymail(addressBook); // get contact index by email

    if (index == -1) // if contact not found
    {
        printf(RED "Contact not found\n" RESET);
        return;
    }

    for (int i = index; i < addressBook->contactCount - 1; i++)  // shift contacts left
        addressBook->contacts[i] = addressBook->contacts[i + 1]; // overwrite deleted contact

    addressBook->contactCount--;
    printf(GREEN "Contact deleted successfully!\n" RESET);
}

void print_Top()
{
    printf("%-25s %-15s %-30s\n", "Name", "Phone No", "Email");
    printf("---------------------------------------------------------------------\n");
}

void print_Contact_Details(AddressBook *addressBook, int index)
{
    printf("%-25s", addressBook->contacts[index].name);    // print contact name
    printf("%-15s", addressBook->contacts[index].phone);   // print contact phone
    printf("%-30s\n", addressBook->contacts[index].email); // print contact email
}
