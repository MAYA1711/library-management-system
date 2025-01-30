#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <windows.h>
#include <conio.h>

// Function declarations
int checkEmail(const char* email);
int checkPhone(const char* phone);
int checkUsername(const char* username, int isStaff);
void getInput(char* input, int maxLen, const char* prompt, int (*validationFunc)(const char*));
void getPass(char* password);
void encrypt(char* password);
void decrypt(char* password);
void loadDB();
void saveDB();
void cleanup();
void adminLogin();
void staffLogin();
void memberLogin();
void adminMenu();
void staffMenu();
void memberMenu();
void addStaff();
void showStaff();
void removeStaff();
void showReports();
void activeMembers();
void deletedMembers();
void inactiveMembers();
void activeStaff();
void removedStaff();
void allBooks();
void availBooks();
void borrowedBooks();
void overdueBooks();
void borrowHistory();
void currentLoans();
void addMember();
void renewMember();
void showMembers();
void deleteMember();
void addBook();
void staffReports();
void settings();
void memberSettings();
void borrowBook();
void returnBook();
void myBooks();
void showBooks();
void logout();

// Structures for linked lists
typedef struct Staff {
    char username[50];
    char password[100];
    char name[100];
    char surname[100];
    char phone[20];
    char email[100];
    char hireDate[20];
    int isDeleted;
    char deletionDate[20];
    struct Staff* next;
} Staff;

typedef struct Member {
    char firstName[50];
    char lastName[50];
    char email[50];
    char phone[20];
    char username[50];
    char password[100];
    char registrationDate[20];
    int isDeleted;
    char deletionDate[20];
    struct Member* next;
} Member;

typedef struct Book {
    char id[10];
    char title[100];
    char author[100];
    char publisher[100];
    int publicationYear;
    char registrationDate[20];
    int available;
    struct Book* next;
} Book;

typedef struct Transaction {
    char bookId[10];
    char memberId[10];
    char borrowDate[20];
    char dueDate[20];
    int returned;
    struct Transaction* next;
} Transaction;

// Global linked list heads
Staff* staffHead = NULL;
Member* memberHead = NULL;
Book* bookHead = NULL;
Transaction* transactionHead = NULL;

// Global variables for current user session
char currentUser[50] = "";
int isStaff = 0;  // 0 for member, 1 for staff, 2 for admin

// Define sleep function for Windows
void sleep(int seconds) {
    Sleep(seconds * 1000);
}

// Custom strptime implementation for Windows
char* strptime(const char* str, const char* format, struct tm* tm) {
    int year, month, day;
    sscanf(str, "%d-%d-%d", &year, &month, &day);
    tm->tm_year = year - 1900;
    tm->tm_mon = month - 1;
    tm->tm_mday = day;
    tm->tm_hour = 0;
    tm->tm_min = 0;
    tm->tm_sec = 0;
    return (char*)str + 10;
}

// Function to encrypt password
void encrypt(char* password) {
    for(int i = 0; password[i] != '\0'; i++) {
        password[i] = password[i] + 3;
    }
}

// Function to decrypt password
void decrypt(char* password) {
    for(int i = 0; password[i] != '\0'; i++) {
        password[i] = password[i] - 3;
    }
}

// Function to load all data from files to linked lists
void loadDB() {
    printf("Opening staff.txt...\n");
    FILE *staffFile = fopen("staff.txt", "r");
    if (staffFile) {
        printf("Reading staff data...\n");
        char line[500];
        while (fgets(line, sizeof(line), staffFile)) {
            Staff* newStaff = (Staff*)malloc(sizeof(Staff));
            if (!newStaff) continue;

            // Remove newline if present
            line[strcspn(line, "\n")] = 0;

            // Parse the line using strtok
            char* token = strtok(line, "|");
            if (token) strcpy(newStaff->username, token);

            token = strtok(NULL, "|");
            if (token) strcpy(newStaff->password, token);

            token = strtok(NULL, "|");
            if (token) strcpy(newStaff->name, token);

            token = strtok(NULL, "|");
            if (token) strcpy(newStaff->surname, token);

            token = strtok(NULL, "|");
            if (token) strcpy(newStaff->phone, token);

            token = strtok(NULL, "|");
            if (token) strcpy(newStaff->email, token);

            token = strtok(NULL, "|");
            if (token) strcpy(newStaff->hireDate, token);

            token = strtok(NULL, "|");
            if (token) newStaff->isDeleted = atoi(token);

            token = strtok(NULL, "|");
            if (token) strcpy(newStaff->deletionDate, token);

            newStaff->next = staffHead;
            staffHead = newStaff;
        }
        fclose(staffFile);
        printf("Staff data loaded successfully.\n");
    } else {
        printf("Creating new staff.txt file...\n");
        staffFile = fopen("staff.txt", "w");
        if (staffFile) {
            fclose(staffFile);
            printf("staff.txt created successfully.\n");
        } else {
            printf("Error: Could not create staff.txt\n");
        }
    }

    printf("Opening members.txt...\n");
    FILE *memberFile = fopen("members.txt", "r");
    if (memberFile) {
        printf("Reading member data...\n");
        char line[500];
        while (fgets(line, sizeof(line), memberFile)) {
            Member* newMember = (Member*)malloc(sizeof(Member));
            if (!newMember) continue;

            // Remove newline if present
            line[strcspn(line, "\n")] = 0;

            // Parse the line using strtok
            char* token = strtok(line, "|");
            if (token) strcpy(newMember->firstName, token);

            token = strtok(NULL, "|");
            if (token) strcpy(newMember->lastName, token);

            token = strtok(NULL, "|");
            if (token) strcpy(newMember->email, token);

            token = strtok(NULL, "|");
            if (token) strcpy(newMember->phone, token);

            token = strtok(NULL, "|");
            if (token) strcpy(newMember->username, token);

            token = strtok(NULL, "|");
            if (token) strcpy(newMember->password, token);

            token = strtok(NULL, "|");
            if (token) strcpy(newMember->registrationDate, token);

            token = strtok(NULL, "|");
            if (token) newMember->isDeleted = atoi(token);

            token = strtok(NULL, "|");
            if (token) strcpy(newMember->deletionDate, token);

            newMember->next = memberHead;
            memberHead = newMember;
        }
        fclose(memberFile);
        printf("Member data loaded successfully.\n");
    } else {
        printf("Creating new members.txt file...\n");
        memberFile = fopen("members.txt", "w");
        if (memberFile) {
            fclose(memberFile);
            printf("members.txt created successfully.\n");
        } else {
            printf("Error: Could not create members.txt\n");
        }
    }

    printf("Opening books.txt...\n");
    FILE *bookFile = fopen("books.txt", "r");
    if (bookFile) {
        printf("Reading book data...\n");
        char line[500];
        while (fgets(line, sizeof(line), bookFile)) {
            Book* newBook = (Book*)malloc(sizeof(Book));
            if (!newBook) continue;

            // Remove newline if present
            line[strcspn(line, "\n")] = 0;

            // Parse the line using strtok
            char* token = strtok(line, "|");
            if (token) strcpy(newBook->id, token);

            token = strtok(NULL, "|");
            if (token) strcpy(newBook->title, token);

            token = strtok(NULL, "|");
            if (token) strcpy(newBook->author, token);

            token = strtok(NULL, "|");
            if (token) newBook->publicationYear = atoi(token);

            token = strtok(NULL, "|");
            if (token) strcpy(newBook->registrationDate, token);

            token = strtok(NULL, "|");
            if (token) newBook->available = atoi(token);

            newBook->next = bookHead;
            bookHead = newBook;
        }
        fclose(bookFile);
        printf("Book data loaded successfully.\n");
    } else {
        printf("Creating new books.txt file...\n");
        bookFile = fopen("books.txt", "w");
        if (bookFile) {
            fclose(bookFile);
            printf("books.txt created successfully.\n");
        } else {
            printf("Error: Could not create books.txt\n");
        }
    }

    printf("Opening transactions.txt...\n");
    FILE *transactionFile = fopen("transactions.txt", "r");
    if (transactionFile) {
        printf("Reading transaction data...\n");
        char line[500];
        while (fgets(line, sizeof(line), transactionFile)) {
            Transaction* newTransaction = (Transaction*)malloc(sizeof(Transaction));
            if (!newTransaction) continue;

            // Remove newline if present
            line[strcspn(line, "\n")] = 0;

            // Parse the line using strtok
            char* token = strtok(line, "|");
            if (token) strcpy(newTransaction->bookId, token);

            token = strtok(NULL, "|");
            if (token) strcpy(newTransaction->memberId, token);

            token = strtok(NULL, "|");
            if (token) strcpy(newTransaction->borrowDate, token);

            token = strtok(NULL, "|");
            if (token) strcpy(newTransaction->dueDate, token);

            token = strtok(NULL, "|");
            if (token) newTransaction->returned = atoi(token);

            newTransaction->next = transactionHead;
            transactionHead = newTransaction;
        }
        fclose(transactionFile);
        printf("Transaction data loaded successfully.\n");
    } else {
        printf("Creating new transactions.txt file...\n");
        transactionFile = fopen("transactions.txt", "w");
        if (transactionFile) {
            fclose(transactionFile);
            printf("transactions.txt created successfully.\n");
        } else {
            printf("Error: Could not create transactions.txt\n");
        }
    }
}

// Function to save all data from linked lists to files
void saveDB() {
    FILE *staffFile = fopen("staff.txt", "w");
    if (staffFile) {
        Staff* current = staffHead;
        while (current != NULL) {
            fprintf(staffFile, "%s|%s|%s|%s|%s|%s|%s|%d|%s\n",
                    current->username, current->password, current->name,
                    current->surname, current->phone, current->email,
                    current->hireDate, current->isDeleted, current->deletionDate);
            current = current->next;
        }
        fclose(staffFile);
    }

    // Similar saving for members, books, and transactions
    FILE *memberFile = fopen("members.txt", "w");
    if (memberFile) {
        Member* current = memberHead;
        while (current != NULL) {
            fprintf(memberFile, "%s|%s|%s|%s|%s|%s|%s|%d|%s\n",
                    current->firstName, current->lastName, current->email,
                    current->phone, current->username, current->password,
                    current->registrationDate, current->isDeleted, current->deletionDate);
            current = current->next;
        }
        fclose(memberFile);
    }

    FILE *bookFile = fopen("books.txt", "w");
    if (bookFile) {
        Book* current = bookHead;
        while (current != NULL) {
            fprintf(bookFile, "%s|%s|%s|%d|%s|%d\n",
                    current->id, current->title, current->author,
                    current->publicationYear, current->registrationDate,
                    current->available);
            current = current->next;
        }
        fclose(bookFile);
    }

    FILE *transactionFile = fopen("transactions.txt", "w");
    if (transactionFile) {
        Transaction* current = transactionHead;
        while (current != NULL) {
            fprintf(transactionFile, "%s|%s|%s|%s|%d\n",
                    current->bookId, current->memberId, current->borrowDate,
                    current->dueDate, current->returned);
            current = current->next;
        }
        fclose(transactionFile);
    }
}

// Function to cleanup resources before exit
void cleanup() {
    printf("Saving data...\n");
    saveDB();


    Staff* currentStaff = staffHead;
    while (currentStaff != NULL) {
        Staff* temp = currentStaff;
        currentStaff = currentStaff->next;
        free(temp);
    }

    // Free member list
    Member* currentMember = memberHead;
    while (currentMember != NULL) {
        Member* temp = currentMember;
        currentMember = currentMember->next;
        free(temp);
    }

    // Free book list
    Book* currentBook = bookHead;
    while (currentBook != NULL) {
        Book* temp = currentBook;
        currentBook = currentBook->next;
        free(temp);
    }

    // Free transaction list
    Transaction* currentTrans = transactionHead;
    while (currentTrans != NULL) {
        Transaction* temp = currentTrans;
        currentTrans = currentTrans->next;
        free(temp);
    }

    printf("Cleanup completed.\n");
}

// Function to validate password strength
int checkPass(const char* password) {
    int hasLetter = 0;
    int hasDigit = 0;
    int len = strlen(password);

    if (len < 8) {
        printf("Error: Password must be at least 8 characters long!\n");
        return 0;
    }

    // Check for letters and digits
    for (int i = 0; i < len; i++) {
        if (isalpha(password[i])) {
            hasLetter = 1;
        } else if (isdigit(password[i])) {
            hasDigit = 1;
        }
    }

    if (!hasLetter) {
        printf("Error: Password must contain at least one letter!\n");
        return 0;
    }

    if (!hasDigit) {
        printf("Error: Password must contain at least one number!\n");
        return 0;
    }

    return 1;
}

// Function to safely get password with validation
void getPassWithValidation(char* password) {
    char tempPassword[100];
    int isValid = 0;

    while (!isValid) {
        printf("Enter password (min 8 chars, must include letters and numbers): ");

        // Get password with masking
        int i = 0;
        char ch;
        while (1) {
            ch = _getch();
            if (ch == 13) {
                tempPassword[i] = '\0';
                break;
            } else if (ch == 8) {
                if (i > 0) {
                    i--;
                    printf("\b \b");
                }
            } else if (ch == 27) {
                tempPassword[0] = '\0';
                break;
            } else if (i < sizeof(tempPassword) - 1) {
                tempPassword[i++] = ch;
                printf("*");
            }
        }
        printf("\n");

        // Validate password strength
        if (checkPass(tempPassword)) {
            strcpy(password, tempPassword);
            isValid = 1;
        } else {
            printf("Please try again.\n");
        }
    }
}

// Function to get password with masking
void getPass(char *password) {
    char ch;
    int index = 0;

    while (1) {
        ch = getch();

        if (ch == '\r' || ch == '\n') {
            break;
        }

        if (ch == '\b') {
            if (index > 0) {
                index--;
                printf("\b \b");
            }
        } else {
            password[index++] = ch;
            printf("*");
        }
    }
    password[index] = '\0';
    printf("\n");
}

// Function to check credentials
int checkCredentials(const char *filename, const char *username, const char *password) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file: %s\n", filename);
        return 0;
    }

    char fileUsername[50], filePassword[50];
    while (fscanf(file, "%s %s", fileUsername, filePassword) != EOF) {
        if (strcmp(username, fileUsername) == 0 && strcmp(password, filePassword) == 0) {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

// Admin login function
void adminLogin() {
    char username[50], password[50];
    const char ADMIN_USERNAME[] = "admin";
    const char ADMIN_PASSWORD[] = "12345";  // Changed back to original password

    printf("\n--- Admin Login ---\n");
    printf("Enter Username: ");
    scanf("%s", username);
    while(getchar() != '\n');  // Clear input buffer

    printf("Enter Password: ");
    getPass(password);

    if (strcmp(username, ADMIN_USERNAME) == 0 && strcmp(password, ADMIN_PASSWORD) == 0) {
        printf("Admin login successful! Redirecting to Admin Menu...\n");
        sleep(2);
        system("cls");
        strcpy(currentUser, username);
        isStaff = 2;  // Set as admin
        adminMenu();
    } else {
        printf("Invalid admin credentials!\n");
        sleep(2);
    }
}

// Staff login function
void staffLogin() {
    char username[50], password[50], decryptedPass[50];
    printf("\n--- Staff Login ---\n");
    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    getPass(password);

    Staff* current = staffHead;
    while (current != NULL) {
        if (!current->isDeleted && strcmp(current->username, username) == 0) {
            strcpy(decryptedPass, current->password);
            decrypt(decryptedPass);
            if (strcmp(password, decryptedPass) == 0) {
                printf("Staff login successful! Redirecting to Staff Menu...\n");
                sleep(2);
                system("cls");
                strcpy(currentUser, username);
                isStaff = 1;
                staffMenu();
                return;
            }
        }
        current = current->next;
    }
    printf("Invalid staff credentials!\n");
}

// Member login function
void memberLogin() {
    char username[50], password[50], decryptedPass[50];
    printf("\n--- Member Login ---\n");
    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    getPass(password);

    Member* current = memberHead;
    while (current != NULL) {
        if (!current->isDeleted && strcmp(current->username, username) == 0) {
            strcpy(decryptedPass, current->password);
            decrypt(decryptedPass);
            if (strcmp(password, decryptedPass) == 0) {
                printf("Member login successful! Redirecting to Member Menu...\n");
                sleep(2);
                system("cls");
                strcpy(currentUser, username);
                isStaff = 0;
                memberMenu();
                return;
            }
        }
        current = current->next;
    }
    printf("Invalid member credentials!\n");
}

// Admin menu function
void adminMenu() {
    int choice;

    while (1) {
        printf("\n--- Admin Menu ---\n");
        printf("1. Add New Staff\n");
        printf("2. View Staff List\n");
        printf("3. Delete Staff\n");
        printf("4. View Reports\n");
        printf("5. Logout\n");
        printf("Please enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addStaff();
                break;
            case 2:
                showStaff();
                break;
            case 3:
                removeStaff();
                break;
            case 4:
                showReports();
                break;
            case 5:
                printf("Logging out...\n");
                return;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}

// Function to add new staff with validation
void addStaff() {
    Staff* newStaff = (Staff*)malloc(sizeof(Staff));
    if (!newStaff) {
        printf("Error: Memory allocation failed!\n");
        return;
    }

    printf("\n--- Add New Staff ---\n");

    // Get and validate username first
    do {
        printf("Enter Username: ");
        scanf("%s", newStaff->username);
        while(getchar() != '\n');  // Clear input buffer

        if (!checkUsername(newStaff->username, 1)) {  // 1 for staff
            printf("Error: Username already exists. Please choose another.\n");
        }
    } while (!checkUsername(newStaff->username, 1));

    // Get and validate password
    getPassWithValidation(newStaff->password);

    printf("Enter First Name: ");
    scanf("%s", newStaff->name);
    while(getchar() != '\n');  // Clear input buffer

    printf("Enter Last Name: ");
    scanf("%s", newStaff->surname);
    while(getchar() != '\n');  // Clear input buffer

    // Get and validate phone
    do {
        getInput(newStaff->phone, sizeof(newStaff->phone), "Enter Phone Number: ", checkPhone);
        if (!checkPhone(newStaff->phone)) {
            printf("Invalid phone number format. Please try again.\n");
        }
    } while (!checkPhone(newStaff->phone));

    // Get and validate email
    do {
        getInput(newStaff->email, sizeof(newStaff->email), "Enter Email: ", checkEmail);
        if (!checkEmail(newStaff->email)) {
            printf("Invalid email format. Please try again.\n");
        }
    } while (!checkEmail(newStaff->email));

    // Encrypt password before saving
    encrypt(newStaff->password);

    // Set hire date
    time_t t = time(NULL);
    struct tm* tm = localtime(&t);
    strftime(newStaff->hireDate, sizeof(newStaff->hireDate), "%Y-%m-%d", tm);

    newStaff->isDeleted = 0;
    newStaff->deletionDate[0] = '\0';
    newStaff->next = NULL;

    // Add to linked list
    if (staffHead == NULL) {
        staffHead = newStaff;
    } else {
        newStaff->next = staffHead;  // Add at the beginning for faster insertion
        staffHead = newStaff;
    }

    // Save to file
    saveDB();

    printf("\nNew staff member '%s %s' has been successfully added!\n",
           newStaff->name, newStaff->surname);
    printf("They can now login with username '%s'\n", newStaff->username);
    sleep(2);  // Give user time to read the message
}

// Function to view staff list
void showStaff() {
    printf("\n--- Staff List ---\n\n");

    Staff* current = staffHead;
    int count = 0;

    while (current != NULL) {
        if (!current->isDeleted) {
            count++;
            printf("Staff #%d:\n", count);
            printf("Username: %s\n", current->username);
            printf("Name: %s %s\n", current->name, current->surname);
            printf("Phone: %s\n", current->phone);
            printf("Email: %s\n", current->email);
            printf("Hire Date: %s\n", current->hireDate);
            printf("------------------------\n\n");
        }
        current = current->next;
    }

    if (count == 0) {
        printf("No staff members found.\n");
    }

    printf("Press Enter to continue...");
    while(getchar() != '\n');
    getchar();
}

// Function to delete staff
void removeStaff() {
    char username[50];
    printf("\n--- Delete Staff ---\n");
    printf("Enter the username of the staff member to delete: ");
    scanf("%s", username);

    Staff* current = staffHead;
    Staff* prev = NULL;
    while (current != NULL) {
        if (strcmp(current->username, username) == 0) {
            if (prev == NULL) {
                staffHead = current->next;
            } else {
                prev->next = current->next;
            }
            current->isDeleted = 1;
            time_t t = time(NULL);
            struct tm* tm = localtime(&t);
            strftime(current->deletionDate, sizeof(current->deletionDate), "%Y-%m-%d", tm);
            printf("Staff member '%s' has been successfully deleted.\n", username);
            saveDB();
            return;
        }
        prev = current;
        current = current->next;
    }
    printf("Error: Staff member not found.\n");
}

// Function to view reports
void showReports() {
    int choice;

    while (1) {
        printf("\n--- Reports Menu ---\n");
        printf("1. Member Reports\n");
        printf("2. Staff Reports\n");
        printf("3. Book Reports\n");
        printf("4. Transaction Reports\n");
        printf("5. Return to Previous Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("\n--- Member Reports ---\n");
                printf("1. Active Members\n");
                printf("2. Deleted Members\n");
                printf("3. Inactive Members (>35 days)\n");
                int memberChoice;
                scanf("%d", &memberChoice);
                switch (memberChoice) {
                    case 1:
                        activeMembers();
                        break;
                    case 2:
                        deletedMembers();
                        break;
                    case 3:
                        inactiveMembers();
                        break;
                }
                break;
            case 2:
                printf("\n--- Staff Reports ---\n");
                printf("1. Active Staff\n");
                printf("2. Removed Staff\n");
                int staffChoice;
                scanf("%d", &staffChoice);
                switch (staffChoice) {
                    case 1:
                        activeStaff();
                        break;
                    case 2:
                        removedStaff();
                        break;
                }
                break;
            case 3:
                printf("\n--- Book Reports ---\n");
                printf("1. All Books\n");
                printf("2. Available Books\n");
                printf("3. Borrowed Books\n");
                printf("4. Overdue Books\n");
                int bookChoice;
                scanf("%d", &bookChoice);
                switch (bookChoice) {
                    case 1:
                        allBooks();
                        break;
                    case 2:
                        availBooks();
                        break;
                    case 3:
                        borrowedBooks();
                        break;
                    case 4:
                        overdueBooks();
                        break;
                }
                break;
            case 4:
                printf("\n--- Transaction Reports ---\n");
                printf("1. Borrowing History\n");
                printf("2. Current Loans\n");
                int transChoice;
                scanf("%d", &transChoice);
                switch (transChoice) {
                    case 1:
                        borrowHistory();
                        break;
                    case 2:
                        currentLoans();
                        break;
                }
                break;
            case 5:
                return;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}

void activeMembers() {
    printf("\n--- Active Members Report ---\n");
    Member* current = memberHead;
    int count = 0;

    while (current != NULL) {
        if (!current->isDeleted) {
            printf("\nMember #%d:\n", ++count);
            printf("Username: %s\n", current->username);
            printf("Name: %s %s\n", current->firstName, current->lastName);
            printf("Registration Date: %s\n", current->registrationDate);
            printf("------------------------\n");
        }
        current = current->next;
    }

    if (count == 0) {
        printf("No active members found.\n");
    }
}

void deletedMembers() {
    printf("\n--- Deleted Members Report ---\n");
    Member* current = memberHead;
    int count = 0;

    while (current != NULL) {
        if (current->isDeleted) {
            printf("\nMember #%d:\n", ++count);
            printf("Username: %s\n", current->username);
            printf("Name: %s %s\n", current->firstName, current->lastName);
            printf("Registration Date: %s\n", current->registrationDate);
            printf("Deletion Date: %s\n", current->deletionDate);
            printf("------------------------\n");
        }
        current = current->next;
    }

    if (count == 0) {
        printf("No deleted members found.\n");
    }
}

void inactiveMembers() {
    printf("\n--- Inactive Members Report (>35 days) ---\n");
    time_t now = time(NULL);
    struct tm* tm_now = localtime(&now);
    char now_str[20];
    strftime(now_str, sizeof(now_str), "%Y-%m-%d", tm_now);

    Member* current = memberHead;
    int count = 0;

    while (current != NULL) {
        if (!current->isDeleted) {
            // Calculate days since last activity
            struct tm tm_reg = {0};
            strptime(current->registrationDate, "%Y-%m-%d", &tm_reg);
            time_t reg_time = mktime(&tm_reg);
            double days = difftime(now, reg_time) / (24 * 60 * 60);

            if (days > 35) {
                printf("\nMember #%d:\n", ++count);
                printf("Username: %s\n", current->username);
                printf("Name: %s %s\n", current->firstName, current->lastName);
                printf("Registration Date: %s\n", current->registrationDate);
                printf("Days Inactive: %.0f\n", days);
                printf("------------------------\n");
            }
        }
        current = current->next;
    }

    if (count == 0) {
        printf("No inactive members found.\n");
    }
}

void allBooks() {
    printf("\n--- All Books Report ---\n");
    Book* current = bookHead;
    int count = 0;

    while (current != NULL) {
        printf("\nBook #%d:\n", ++count);
        printf("ID: %s\n", current->id);
        printf("Title: %s\n", current->title);
        printf("Author: %s\n", current->author);
        printf("Publisher: %s\n", current->publisher);
        printf("Publication Year: %d\n", current->publicationYear);
        printf("Registration Date: %s\n", current->registrationDate);
        printf("Status: %s\n", current->available ? "Available" : "Borrowed");
        printf("------------------------\n");
        current = current->next;
    }

    if (count == 0) {
        printf("No books found in the system.\n");
    }
}

void borrowedBooks() {
    printf("\n--- Currently Borrowed Books Report ---\n");
    Transaction* current = transactionHead;
    int count = 0;

    while (current != NULL) {
        if (!current->returned) {
            printf("\nTransaction #%d:\n", ++count);
            printf("Book ID: %s\n", current->bookId);
            printf("Borrowed By: %s\n", current->memberId);
            printf("Borrow Date: %s\n", current->borrowDate);
            printf("------------------------\n");
        }
        current = current->next;
    }

    if (count == 0) {
        printf("No books are currently borrowed.\n");
    }
}

void overdueBooks() {
    printf("\n--- Overdue Books Report ---\n");
    time_t now = time(NULL);
    struct tm* tm_now = localtime(&now);
    char now_str[20];
    strftime(now_str, sizeof(now_str), "%Y-%m-%d", tm_now);

    Transaction* current = transactionHead;
    int count = 0;

    while (current != NULL) {
        if (!current->returned) {
            // Calculate days since borrowing
            struct tm tm_borrow = {0};
            strptime(current->borrowDate, "%Y-%m-%d", &tm_borrow);
            time_t borrow_time = mktime(&tm_borrow);
            double days = difftime(now, borrow_time) / (24 * 60 * 60);

            if (days > 14) {
                printf("\nOverdue Book #%d:\n", ++count);
                printf("Book ID: %s\n", current->bookId);
                printf("Borrowed By: %s\n", current->memberId);
                printf("Borrow Date: %s\n", current->borrowDate);
                printf("Days Overdue: %.0f\n", days - 14);
                printf("------------------------\n");
            }
        }
        current = current->next;
    }

    if (count == 0) {
        printf("No overdue books found.\n");
    }
}

void borrowHistory() {
    printf("\n--- Complete Borrowing History Report ---\n");
    Transaction* current = transactionHead;
    int count = 0;

    while (current != NULL) {
        printf("\nTransaction #%d:\n", ++count);
        printf("Book ID: %s\n", current->bookId);
        printf("Borrowed By: %s\n", current->memberId);
        printf("Borrow Date: %s\n", current->borrowDate);
        if (current->returned) {
            printf("Return Date: %s\n", current->dueDate);
            printf("Status: Returned\n");
        } else {
            printf("Status: Not Returned\n");
        }
        printf("------------------------\n");
        current = current->next;
    }

    if (count == 0) {
        printf("No borrowing history found.\n");
    }
}

void currentLoans() {
    printf("\n--- Current Loans Report ---\n");
    Transaction* current = transactionHead;
    int count = 0;

    while (current != NULL) {
        if (!current->returned) {
            printf("\nLoan #%d:\n", ++count);
            printf("Book ID: %s\n", current->bookId);
            printf("Borrowed By: %s\n", current->memberId);
            printf("Borrow Date: %s\n", current->borrowDate);

            // Calculate days borrowed
            time_t now = time(NULL);
            struct tm tm_borrow = {0};
            strptime(current->borrowDate, "%Y-%m-%d", &tm_borrow);
            time_t borrow_time = mktime(&tm_borrow);
            double days = difftime(now, borrow_time) / (24 * 60 * 60);

            printf("Days Borrowed: %.0f\n", days);
            if (days > 14) {
                printf("Status: OVERDUE by %.0f days\n", days - 14);
            } else {
                printf("Status: Within borrowing period (%.0f days remaining)\n", 14 - days);
            }
            printf("------------------------\n");
        }
        current = current->next;
    }

    if (count == 0) {
        printf("No current loans found.\n");
    }
}

void activeStaff() {
    Staff* current = staffHead;
    int count = 0;

    printf("\n--- Active Staff Report ---\n");
    while (current != NULL) {
        if (!current->isDeleted) {
            printf("\nStaff Member #%d:\n", ++count);
            printf("Username: %s\n", current->username);
            printf("Name: %s %s\n", current->name, current->surname);
            printf("Email: %s\n", current->email);
            printf("Phone: %s\n", current->phone);
            printf("Registration Date: %s\n", current->hireDate);
            printf("------------------------\n");
        }
        current = current->next;
    }

    if (count == 0) {
        printf("No active staff members found.\n");
    }
}

void removedStaff() {
    Staff* current = staffHead;
    int count = 0;

    printf("\n--- Removed Staff Report ---\n");
    while (current != NULL) {
        if (current->isDeleted) {
            printf("\nStaff Member #%d:\n", ++count);
            printf("Username: %s\n", current->username);
            printf("Name: %s %s\n", current->name, current->surname);
            printf("Email: %s\n", current->email);
            printf("Phone: %s\n", current->phone);
            printf("Registration Date: %s\n", current->hireDate);
            printf("------------------------\n");
        }
        current = current->next;
    }

    if (count == 0) {
        printf("No removed staff members found.\n");
    }
}

void availBooks() {
    Book* current = bookHead;
    int count = 0;

    printf("\n--- Available Books Report ---\n");
    while (current != NULL) {
        if (current->available) {
            printf("\nBook #%d:\n", ++count);
            printf("ID: %s\n", current->id);
            printf("Title: %s\n", current->title);
            printf("Author: %s\n", current->author);
            printf("Publication Year: %d\n", current->publicationYear);
            printf("Registration Date: %s\n", current->registrationDate);
            printf("------------------------\n");
        }
        current = current->next;
    }

    if (count == 0) {
        printf("No available books found.\n");
    }
}

// Staff menu function
void staffMenu() {
    int choice;

    while (1) {
        printf("\n--- Staff Menu ---\n");
        printf("1. Register a New Member\n");
        printf("2. Renew Membership\n");
        printf("3. List All Members (Active, Former, and Inactive)\n");
        printf("4. Remove a Member\n");
        printf("5. Register a New Book\n");
        printf("6. View Reports\n");
        printf("7. Account Settings\n");
        printf("8. Logout\n");
        printf("Please enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addMember();
                break;
            case 2:
                renewMember();
                break;
            case 3:
                showMembers();
                break;
            case 4:
                deleteMember();
                break;
            case 5:
                addBook();
                break;
            case 6:
                staffReports();
                break;
            case 7:
                settings();
                break;
            case 8:
                logout();
                return;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}

// Function to register a new member
void addMember() {
    Member* newMember = (Member*)malloc(sizeof(Member));
    if (!newMember) {
        printf("Error: Memory allocation failed!\n");
        return;
    }

    printf("\n--- Register a New Member ---\n");

    // Get and validate username first
    do {
        printf("Enter Username: ");
        scanf("%s", newMember->username);
        while(getchar() != '\n');  // Clear input buffer

        if (!checkUsername(newMember->username, 0)) {  // 0 for member
            printf("Error: Username already exists. Please choose another.\n");
        }
    } while (!checkUsername(newMember->username, 0));

    // Get and validate password
    getPassWithValidation(newMember->password);

    printf("Enter First Name: ");
    scanf("%s", newMember->firstName);
    while(getchar() != '\n');  // Clear input buffer

    printf("Enter Last Name: ");
    scanf("%s", newMember->lastName);
    while(getchar() != '\n');  // Clear input buffer

    // Get and validate phone
    do {
        getInput(newMember->phone, sizeof(newMember->phone), "Enter Phone Number: ", checkPhone);
        if (!checkPhone(newMember->phone)) {
            printf("Invalid phone number format. Please try again.\n");
        }
    } while (!checkPhone(newMember->phone));

    // Get and validate email
    do {
        getInput(newMember->email, sizeof(newMember->email), "Enter Email: ", checkEmail);
        if (!checkEmail(newMember->email)) {
            printf("Invalid email format. Please try again.\n");
        }
    } while (!checkEmail(newMember->email));

    // Encrypt password before saving
    encrypt(newMember->password);

    // Set registration date
    time_t t = time(NULL);
    struct tm* tm = localtime(&t);
    strftime(newMember->registrationDate, sizeof(newMember->registrationDate), "%Y-%m-%d", tm);

    newMember->isDeleted = 0;
    newMember->deletionDate[0] = '\0';
    newMember->next = NULL;

    // Add to linked list
    if (memberHead == NULL) {
        memberHead = newMember;
    } else {
        newMember->next = memberHead;  // Add at the beginning for faster insertion
        memberHead = newMember;
    }

    // Save to file
    saveDB();

    printf("\nNew member '%s %s' has been successfully registered!\n",
           newMember->firstName, newMember->lastName);
    printf("You can now login with username '%s'\n", newMember->username);
    sleep(2);  // Give user time to read the message
}

// Function to renew membership
void renewMember() {
    char username[50];
    int found = 0;

    printf("\n--- Renew Membership ---\n");
    printf("Enter the Username of the Member: ");
    scanf("%s", username);

    // Search for member in memory
    Member* current = memberHead;
    while (current != NULL) {
        if (strcmp(current->username, username) == 0) {
            found = 1;
            if (current->isDeleted) {
                current->isDeleted = 0;
                time_t t = time(NULL);
                struct tm* tm = localtime(&t);
                strftime(current->registrationDate, sizeof(current->registrationDate), "%Y-%m-%d", tm);
                printf("Membership for '%s' has been successfully renewed.\n", username);
                saveDB();
                return;
            } else {
                printf("Member '%s' is already active.\n", username);
                return;
            }
        }
        current = current->next;
    }

    if (!found) {
        printf("Error: No member found with the username '%s'.\n", username);
    }
}

// Function to list all members
void showMembers() {
    char buffer[300];

    printf("\n--- List of All Members ---\n");

    FILE *file = fopen("members.txt", "r");
    if (!file) {
        printf("Error: Unable to open members profiles file.\n");
        return;
    }

    while (fgets(buffer, sizeof(buffer), file)) {
        char firstName[50], lastName[50], email[50], phone[15], username[50], status[20];
        sscanf(buffer, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%s", firstName, lastName, email, phone, username, status);
        printf("Name: %s %s | Email: %s | Phone: %s | Username: %s | Status: %s\n",
               firstName, lastName, email, phone, username, status);
    }

    fclose(file);
}

// Function to remove a member
void deleteMember() {
    char username[50], buffer[300];
    int found = 0;

    printf("\n--- Remove a Member ---\n");
    printf("Enter the Username of the Member to Remove: ");
    scanf("%s", username);

    FILE *file = fopen("members.txt", "r");
    FILE *tempFile = fopen("temp_members.txt", "w");
    if (!file || !tempFile) {
        printf("Error: Unable to open members profiles file.\n");
        return;
    }

    while (fgets(buffer, sizeof(buffer), file)) {
        char fileUsername[50];
        sscanf(buffer, "%*[^|]|%*[^|]|%*[^|]|%*[^|]|%[^|]|%s", fileUsername);

        if (strcmp(username, fileUsername) == 0) {
            found = 1;
            continue;
        }

        fprintf(tempFile, "%s", buffer);
    }

    fclose(file);
    fclose(tempFile);

    if (found) {
        remove("members.txt");
        rename("temp_members.txt", "members.txt");
        printf("Member with username '%s' has been successfully removed.\n", username);
    } else {
        remove("temp_members.txt");
        printf("Error: No member found with the username '%s'.\n", username);
    }
}

// Function to register a new book
void addBook() {
    Book* newBook = (Book*)malloc(sizeof(Book));
    if (!newBook) {
        printf("Error: Memory allocation failed!\n");
        return;
    }

    printf("\n--- Register New Book ---\n");
    printf("Enter book ID: ");
    scanf("%s", newBook->id);
    printf("Enter book title: ");
    scanf(" %[^\n]s", newBook->title);
    printf("Enter author name: ");
    scanf(" %[^\n]s", newBook->author);
    printf("Enter publisher: ");
    scanf(" %[^\n]s", newBook->publisher);
    printf("Enter publication year: ");
    scanf("%d", &newBook->publicationYear);

    // Get current date for registration date
    time_t t = time(NULL);
    struct tm* tm = localtime(&t);
    strftime(newBook->registrationDate, sizeof(newBook->registrationDate), "%Y-%m-%d", tm);

    newBook->available = 1;
    // Add to linked list
    newBook->next = bookHead;
    bookHead = newBook;

    printf("\nBook registered successfully!\n");
    saveDB();
}

// Function to borrow a book
void borrowBook() {
    char bookId[20];
    time_t now;
    struct tm dueDate;
    char dueDateStr[11];
    int bookFound = 0;
    char currentDate[11];

    printf("\n--- Borrow a Book ---\n");

    // Clear input buffer
    while(getchar() != '\n');

    printf("Enter Book ID (e.g., B001): ");
    scanf("%s", bookId);
    while(getchar() != '\n');  // Clear input buffer

    // Verify member exists and is active
    Member* currentMember = memberHead;
    while (currentMember != NULL) {
        if (strcmp(currentMember->username, currentUser) == 0) {
            if (currentMember->isDeleted) {
                printf("Error: Your account is not active.\n");
                return;
            }
            break;
        }
        currentMember = currentMember->next;
    }

    if (currentMember == NULL) {
        printf("Error: Member account not found. Please log in again.\n");
        return;
    }

    // Check if book exists and is available
    Book* currentBook = bookHead;
    while (currentBook != NULL) {
        if (strcmp(currentBook->id, bookId) == 0) {
            bookFound = 1;
            if (!currentBook->available) {
                printf("Error: Book is currently not available for borrowing.\n");
                return;
            }

            // Update book status
            currentBook->available = 0;

            // Get current date
            time(&now);
            strftime(currentDate, sizeof(currentDate), "%Y-%m-%d", localtime(&now));

            // Calculate due date (14 days from now)
            dueDate = *localtime(&now);
            dueDate.tm_mday += 14;
            mktime(&dueDate);
            strftime(dueDateStr, sizeof(dueDateStr), "%Y-%m-%d", &dueDate);

            // Create transaction record
            Transaction* newTransaction = (Transaction*)malloc(sizeof(Transaction));
            if (newTransaction == NULL) {
                printf("Error: Memory allocation failed.\n");
                currentBook->available = 1; // Revert book status
                return;
            }

            // Fill transaction details
            strcpy(newTransaction->bookId, bookId);
            strcpy(newTransaction->memberId, currentUser);
            strcpy(newTransaction->borrowDate, currentDate);
            strcpy(newTransaction->dueDate, dueDateStr);
            newTransaction->returned = 0;
            newTransaction->next = NULL;

            // Add transaction to linked list
            if (transactionHead == NULL) {
                transactionHead = newTransaction;
            } else {
                newTransaction->next = transactionHead;
                transactionHead = newTransaction;
            }

            // Save changes
            saveDB();

            printf("\nBook borrowed successfully!\n");
            printf("Book: %s\n", currentBook->title);
            printf("Due Date: %s\n", dueDateStr);
            printf("Please return the book by the due date to avoid fines.\n");
            sleep(2);
            return;
        }
        currentBook = currentBook->next;
    }

    if (!bookFound) {
        printf("Error: Book with ID '%s' not found.\n", bookId);
        return;
    }
}

// Function to return a book
void returnBook() {
    char bookId[10], memberId[10];
    int found = 0;

    printf("\n--- Return a Book ---\n");
    printf("Enter Book ID: ");
    scanf("%s", bookId);
    printf("Enter Member ID: ");
    scanf("%s", memberId);

    // Find the transaction
    Transaction* currentTransaction = transactionHead;
    while (currentTransaction != NULL) {
        if (strcmp(currentTransaction->bookId, bookId) == 0 &&
            strcmp(currentTransaction->memberId, memberId) == 0 &&
            !currentTransaction->returned) {
            found = 1;
            currentTransaction->returned = 1;

            // Update book status
            Book* currentBook = bookHead;
            while (currentBook != NULL) {
                if (strcmp(currentBook->id, bookId) == 0) {
                    currentBook->available = 1;
                    break;
                }
                currentBook = currentBook->next;
            }

            printf("Book returned successfully!\n");
            saveDB();
            return;
        }
        currentTransaction = currentTransaction->next;
    }

    if (!found) {
        printf("Error: No matching active loan found.\n");
    }
}

// Function to view available books
void showBooks() {
    printf("\n--- Available Books ---\n");
    Book* current = bookHead;
    int count = 0;

    while (current != NULL) {
        if (current->available) {
            printf("\nBook #%d:\n", ++count);
            printf("ID: %s\n", current->id);
            printf("Title: %s\n", current->title);
            printf("Author: %s\n", current->author);
            printf("Publisher: %s\n", current->publisher);
            printf("Publication Year: %d\n", current->publicationYear);
            printf("Registration Date: %s\n", current->registrationDate);
            printf("------------------------\n");
        }
        current = current->next;
    }

    if (count == 0) {
        printf("No books available at the moment.\n");
    }
}

// Function for staff to view reports
void staffReports() {
    printf("\n--- Reports Menu (Staff) ---\n");
    printf("1. List of Active Members\n");
    printf("2. List of All Books\n");
    printf("3. List of Available Books\n");
    printf("4. List of Currently Loaned Books\n");
    printf("5. Return to Staff Menu\n");

    int choice;
    printf("Please select a report to view: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            showMembers();
            break;
        case 2:
            allBooks();
            break;
        case 3:
            availBooks();
            break;
        case 4:
            borrowedBooks();
            break;
        case 5:
            printf("Returning to Staff Menu...\n");
            return;
        default:
            printf("Invalid choice. Please try again.\n");
    }
}

// Function to update account settings
void settings() {
    char currentUsername[50], newUsername[50], currentPassword[50], newPassword[50];
    char decryptedPass[50];
    int found = 0;

    printf("\n--- Account Settings ---\n");
    printf("Enter your current username: ");
    scanf("%s", currentUsername);

    Staff* current = staffHead;
    while (current != NULL) {
        if (strcmp(current->username, currentUsername) == 0) {
            found = 1;

          // Get current password
            printf("Enter your current password: ");
            getPass(currentPassword);

            strcpy(decryptedPass, current->password);
            decrypt(decryptedPass);

            if (strcmp(currentPassword, decryptedPass) != 0) {
                printf("Error: Incorrect current password.\n");
                return;
            }

         // Get new username
            printf("Enter new username (or press Enter to keep current): ");
            scanf(" %[^\n]s", newUsername);

            if (strlen(newUsername) > 0) {
                if (!checkUsername(newUsername, 1)) {
                    printf("Error: Username already exists.\n");
                    return;
                }
                strcpy(current->username, newUsername);
            }

            printf("Enter new password (or press Enter to keep current): ");
            getPassWithValidation(newPassword);

            if (strlen(newPassword) > 0) {
                encrypt(newPassword);
                strcpy(current->password, newPassword);
            }

            printf("Account settings updated successfully!\n");
            saveDB();
            return;
        }
        current = current->next;
    }

    if (!found) {
        printf("Error: Staff member not found.\n");
    }
}

// Function to logout
void logout() {
    printf("\nLogging out...\n");

    return;
}

// Member menu function
void memberMenu() {
    int choice;

    while (1) {
        printf("\n--- Member Menu ---\n");
        printf("1. Borrow a Book\n");
        printf("2. Return a Book\n");
        printf("3. View My Borrowed Books\n");
        printf("4. View Available Books\n");
        printf("5. Account Settings\n");
        printf("6. Logout\n");
        printf("Please enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                borrowBook();
                break;
            case 2:
                returnBook();
                break;
            case 3:
                myBooks();
                break;
            case 4:
                showBooks();
                break;
            case 5:
                memberSettings();
                break;
            case 6:
                printf("Logging out...\n");
                return;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}

// Function to view borrowed books by member
void myBooks() {
    printf("\n--- My Borrowed Books ---\n");

    int found = 0;
    Transaction* current = transactionHead;

    while (current != NULL) {
        if (strcmp(current->memberId, currentUser) == 0 && !current->returned) {
            if (!found) {
                printf("\nCurrently Borrowed Books:\n");
                printf("%-10s %-30s %-15s %-15s\n",
                       "Book ID", "Title", "Borrow Date", "Due Date");
                printf("--------------------------------------------------------\n");
            }

            // Find book details
            Book* book = bookHead;
            while (book != NULL) {
                if (strcmp(book->id, current->bookId) == 0) {
                    printf("%-10s %-30s %-15s %-15s\n",
                           book->id,
                           book->title,
                           current->borrowDate,
                           current->dueDate);
                    found = 1;
                    break;
                }
                book = book->next;
            }
        }
        current = current->next;
    }

    if (!found) {
        printf("You have no borrowed books.\n");
    }

    printf("\nPress Enter to continue...");
    while(getchar() != '\n');
    getchar();
}

// Function for member account settings
void memberSettings() {
    char currentUsername[50], buffer[300];
    char newPassword[50], confirmPassword[50];

    printf("\n--- Account Settings ---\n");
    printf("Enter your current username: ");
    scanf("%s", currentUsername);

    FILE *file = fopen("members.txt", "r");
    FILE *tempFile = fopen("temp_members.txt", "w");
    if (!file || !tempFile) {
        printf("Error: Unable to access member profiles.\n");
        return;
    }

    int found = 0;
    while (fgets(buffer, sizeof(buffer), file)) {
        char fileUsername[50];
        sscanf(buffer, "%*[^|]|%*[^|]|%*[^|]|%*[^|]|%[^|]|%s", fileUsername);

        if (strcmp(fileUsername, currentUsername) == 0) {
            found = 1;
            printf("Enter new password: ");
            getPassWithValidation(newPassword);
            printf("Confirm new password: ");
            getPassWithValidation(confirmPassword);

            if (strcmp(newPassword, confirmPassword) == 0) {
                fprintf(tempFile, "%.*s|%s\n", (int)(strchr(buffer, '|') - buffer), buffer, newPassword);
                printf("Password updated successfully!\n");
            } else {
                printf("Error: Passwords do not match.\n");
                fprintf(tempFile, "%s", buffer);
            }
        } else {
            fprintf(tempFile, "%s", buffer);
        }
    }

    fclose(file);
    fclose(tempFile);

    if (found) {
        remove("members.txt");
        rename("temp_members.txt", "members.txt");
    } else {
        remove("temp_members.txt");
        printf("Error: Username not found.\n");
    }
}

// Function to validate email format
int checkEmail(const char* email) {
    int atCount = 0;
    int dotCount = 0;
    int len = strlen(email);

    // Check minimum length
    if (len < 5) return 0;  // a@b.c minimum

    // Check for @ and . characters
    for (int i = 0; i < len; i++) {
        if (email[i] == '@') atCount++;
        if (email[i] == '.') dotCount++;
    }

    // Must have exactly one @ and at least one .
    if (atCount != 1 || dotCount < 1) return 0;

    // @ cannot be first or last character
    if (email[0] == '@' || email[len-1] == '@') return 0;

    // . cannot be first or last character
    if (email[0] == '.' || email[len-1] == '.') return 0;

    return 1;
}

// Function to validate phone number
int checkPhone(const char* phone) {
    int len = strlen(phone);

    // Check length (assuming 10-15 digits, may include - or space)
    if (len < 10 || len > 15) return 0;

    // Check each character
    for (int i = 0; i < len; i++) {
        if (!isdigit(phone[i]) && phone[i] != '-' && phone[i] != ' ') {
            return 0;
        }
    }

    return 1;
}

// Function to check if username already exists
int checkUsername(const char* username, int isStaff) {
    if (isStaff) {
        Staff* current = staffHead;
        while (current != NULL) {
            if (strcmp(current->username, username) == 0 && !current->isDeleted) {
                return 0;  // Username exists
            }
            current = current->next;
        }
    } else {
        Member* current = memberHead;
        while (current != NULL) {
            if (strcmp(current->username, username) == 0 && !current->isDeleted) {
                return 0;  // Username exists
            }
            current = current->next;
        }
    }
    return 1;  // Username is available
}

void getInput(char* input, int maxLen, const char* prompt, int (*validationFunc)(const char*)) {
    int isValid = 0;
    while (!isValid) {
        printf("%s", prompt);
        scanf("%s", input);

        if (strlen(input) >= maxLen) {
            printf("Error: Input too long (max %d characters)\n", maxLen-1);
            continue;
        }

        if (validationFunc && !validationFunc(input)) {
            printf("Error: Invalid format. Please try again.\n");
            continue;
        }

        isValid = 1;
    }
}

int main() {
    printf("Starting program...\n");
    loadDB();
    printf("Data loaded successfully.\n");

    int choice;
    do {
        printf("\n=== Library Management System ===\n");
        printf("1. Admin Login\n");
        printf("2. Staff Login\n");
        printf("3. Member Login\n");
        printf("4. Exit\n");
        printf("Enter your choice (1-5): ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
            case 1:
                adminLogin();
                break;
            case 2:
                staffLogin();
                break;
            case 3:
                memberLogin();
                break;
            case 4:
               printf("Thank you for using the Library Management System!\n");
                cleanup();
                return 0;
                break;

            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (1);

    return 0;
}
