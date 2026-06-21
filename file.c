#include <stdio.h>          
#include "file.h"           

void saveContactsToFile(AddressBook *addressBook)
{
    FILE *fptr = fopen("contacts.csv", "w"); // Open contacts file in write mode
    if (fptr == NULL)                            // Check if file opening failed
    {
        printf("Error opening file!\n");     
        return;                                 
    }

    int validCount = 0;                          // Variable to count valid contacts

    for (int i = 0; i < addressBook->contactCount; i++) // Loop through all contacts
    {
        if (addressBook->contacts[i].name[0] != '\0')   // Check if contact is valid
        {
            validCount++;                        // Increment valid contact count
        }
    }

    fprintf(fptr, "#%d\n", validCount);        // Write total valid contacts at top

    for (int i = 0; i < addressBook->contactCount; i++) // Loop again to write details
    {
        if (addressBook->contacts[i].name[0] == '\0')   // Skip deleted/empty contacts
        {
            continue;                            // Move to next contact
        }
        fprintf(fptr, "%s,%s,%s\n",             // Write name, phone, and email
                addressBook->contacts[i].name,
                addressBook->contacts[i].phone,
                addressBook->contacts[i].email);
    }

    fclose(fptr);                                // Close the file
}
