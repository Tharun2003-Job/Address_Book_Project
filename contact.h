#ifndef CONTACT_H                 // Check if CONTACT_H is not defined
#define CONTACT_H                 // Define CONTACT_H to avoid multiple inclusion

#define MAX_CONTACTS 100           // Maximum number of contacts allowed

// Structure Definition
typedef struct
{
    char name[50];                 // Store contact name
    char phone[20];                // Store contact phone number
    char email[50];                // Store contact email ID
} Contact;                         // Structure for a single contact

// Nested Structure Definition
typedef struct
{
    Contact contacts[100];         // Array to store all contacts
    int contactCount;              // Number of contacts currently stored
} AddressBook;                     // Structure for address book

// All functions declarations to help to write the function definition in another file
void createContact(AddressBook *addressBook);             
void searchContact(AddressBook *addressBook);             
void editContact(AddressBook *addressBook);                
void deleteContact(AddressBook *addressBook);              
void listContacts(AddressBook *addressBook, int sortCriteria); 
void initialize(AddressBook *addressBook);                 
void saveContactsToFile(AddressBook *addressBook);         

int validate_name(char name[]);                            
int validate_phonenum(char phonenum[], AddressBook *addressBook); 
int validate_mail(char mail[], AddressBook *addressBook);  

void print_Top(void);                                      
void print_Contact_Details(AddressBook *addressBook, int index); 

int searchbyname(AddressBook *addressBook, int arr[]);     
int searchbynum(AddressBook *addressBook);                 
int searchbymail(AddressBook *addressBook);                

int read_int(void);                                        // Safely read integer input

#endif                              // End of CONTACT_H header guard