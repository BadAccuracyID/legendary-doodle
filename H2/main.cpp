#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>

struct Drama {
    char title[32]{};
    char maleLead[32]{};
    char femaleLead[32]{};
    double rating = 0.0;
    int year = 0;

    Drama *next = nullptr;
    Drama *prev = nullptr;
};

Drama *createDrama(const char *title, const char *maleLead, const char *femaleLead, double rating, int year) {
    auto *drama = (Drama *) malloc(sizeof(Drama));

    strcpy(drama->title, title);
    strcpy(drama->maleLead, maleLead);
    strcpy(drama->femaleLead, femaleLead);
    drama->rating = rating;
    drama->year = year;

    return drama;
}

struct DramaList {
    Drama *head = nullptr;
    Drama *tail = nullptr;

    void popHead() {
        if (head == nullptr) {
            return;
        }

        Drama *temp = head;
        head = head->next;
        head->prev = nullptr;
        free(temp);
    }

    void pushTail(Drama *drama) {
        if (head == nullptr) {
            head = drama;
            tail = drama;
        } else {
            drama->prev = tail;
            tail->next = drama;
            tail = drama;
        }
    }

    void remove(Drama *Drama) {
        if (head == nullptr) {
            return;
        }

        if (head == Drama) {
            popHead();
            return;
        }

        if (tail == Drama) {
            tail = tail->prev;
            tail->next = nullptr;
            free(Drama);
            return;
        }

        Drama->prev->next = Drama->next;
        Drama->next->prev = Drama->prev;
        free(Drama);
    }

    int getSize() const {
        int size = 0;
        auto *current = head;
        while (current != nullptr) {
            size++;
            current = current->next;
        }
        return size;
    }
};

DramaList dramaList;

struct UserData {
    char username[32]{};
    char password[32]{};

    bool hasAdminRights = false;

    DramaList watchLater;
    DramaList queue;

    UserData *next = nullptr;
    UserData *prev = nullptr;
};

UserData *createUserData(const char *username, const char *password, bool hasAdminRights) {
    auto *userData = (UserData *) malloc(sizeof(UserData));

    strcpy(userData->username, username);
    strcpy(userData->password, password);

    userData->hasAdminRights = hasAdminRights;

    return userData;
}

UserData currentUser{};

struct UserList {
    UserData *head = nullptr;
    UserData *tail = nullptr;

    void pushTail(UserData *data) {
        if (head == nullptr) {
            head = data;
            tail = data;
        } else {
            data->prev = tail;
            tail->next = data;
            tail = data;
        }
    }
};

UserList userList;

void clearScreen();

void homePage();

void loginMenu();

void registerMenu();

void loggedInMenu();

bool isAlphanumeric(char password[32]);

void addDrama();

void printDramaListTable();

void updateDrama();

void deleteDrama();

void viewDramaList();

bool isCancel(char input[32]);

bool isTitleExists(char title[32]);

void addWatchLater();

void addQueue();

void removeFromWatchLater();

void removeFromQueue();

void printQueueTable();

void printWatchLaterTable();

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void printLDLogo() {
    printf("                                  .......,*...                           \n"
           "                           ......&..........,%&&&*&.......                      \n"
           "                         ...%.,.../(..&/,     .....,..(..,*....                 \n"
           "                      ..#,......#*,..............   ..../%.....                 \n"
           "                   .,/(.....(, .*....................  ...*                     \n"
           "                  .,#,...,,. .........................  ,...% .                 \n"
           "              ...&/((&#*,...............*&@@%...........  ,*,,....              \n"
           "             ..(,*.... ./,.%,...... /&@@@@@ .............  .*,/...              \n"
           "              ......./ .....*** &@@@@@&#.................. ..(,&                \n"
           "                 (,.,....,..(@@@@@@/./@@@@@@@@@#*.......,*...,*.(               \n"
           "                 %,,,..,,,.,.@@@@@@@#&......,@@@@@@@@@%.... .,/,%               \n"
           "               ..%,,. ..,..,,,,../@@@@@@@@.../@@@@@#...... ..*,*(,              \n"
           "               ..#... ..,,..,..........,,.(@@@@@ ......... ..../.,.             \n"
           "                ./.... .. ......... .,&@@@@@%............. .*.,% /.             \n"
           "                .  ...,#........... *@@@(.....,...........,(..#..,..            \n"
           "                    .*. ,................,......,.......,&,,..                  \n"
           "                     *.,.(...*......,.......,..,.. .  .&,..                     \n"
           "                       ..,,,* . .*..,......    ...,%(...(,..                    \n"
           "                        .,**#,*#,..(.#...,,,#&%###...*,*,...                    \n"
           "                         . ...&&/.*&,.*&.......(#.                              \n"
           "                                 ..*...,,..                                     \n"
           "                                   .......                                      \n"
           "                                                                                \n"
           "                               RedJackets 23-1                                 \n"
           "       Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nullam sit.      \n");
}

void readFile() {
    FILE *file = fopen("user.txt", "r");
    if (file == nullptr) {
        printf("File not found!");
        clearScreen();
        printLDLogo();
        exit(1);
    }

    char bufferName[32]{};
    char bufferPass[32]{};

    while (fscanf(file, "%[^#]#%s\n", bufferName, bufferPass) != EOF) {
        bool isAdmin = strcmp(bufferName, "admin") == 0 && strcmp(bufferPass, "admin221") == 0;
        UserData *newUser = createUserData(bufferName, bufferPass, isAdmin);
        userList.pushTail(newUser);
    }

    fclose(file);

    file = fopen("drama.txt", "r");
    if (file == nullptr) {
        printf("File not found!");
        clearScreen();
        printLDLogo();
        exit(1);
    }

    char bufferTitle[32]{};
    char bufferMaleLead[32]{};
    char bufferFemaleLead[32]{};
    double bufferRating = 0.0;
    int bufferYear = 0;

    while (fscanf(file, "%[^#]#%[^#]#%[^#]#%lf#%d\n", bufferTitle, bufferMaleLead, bufferFemaleLead, &bufferRating,
                  &bufferYear) != EOF) {
        Drama *newDrama = createDrama(bufferTitle, bufferMaleLead, bufferFemaleLead, bufferRating, bufferYear);
        dramaList.pushTail(newDrama);
    }

    fclose(file);
}

void saveFile() {
    FILE *file = fopen("user.txt", "w");
    if (file == nullptr) {
        printf("File not found!");
        clearScreen();
        printLDLogo();
        exit(1);
    }

    UserData *temp = userList.head;
    while (temp != nullptr) {
        fprintf(file, "%s#%s\n", temp->username, temp->password);
        temp = temp->next;
    }

    fclose(file);

    file = fopen("drama.txt", "w");
    if (file == nullptr) {
        printf("File not found!");
        clearScreen();
        printLDLogo();
        exit(1);
    }

    Drama *tempDrama = dramaList.head;
    while (tempDrama != nullptr) {
        fprintf(file, "%s#%s#%s#%lf#%d\n", tempDrama->title, tempDrama->maleLead, tempDrama->femaleLead,
                tempDrama->rating, tempDrama->year);
        tempDrama = tempDrama->next;
    }

    fclose(file);
}

void homePage() {
    int choice = 0;
    do {
        clearScreen();
        printf("MyDramaList\n");
        printf("=============\n");
        printf("1. Login\n");
        printf("2. Register\n");
        printf("3. Exit\n");
        printf(">> ");

        scanf("%d", &choice);
    } while (choice < 1 || choice > 3);

    switch (choice) {
        case 1:
            loginMenu();
            break;
        case 2:
            registerMenu();
            break;
        case 3:
            clearScreen();
            printLDLogo();
            exit(0);
        default:
            break;
    }
}

void loginMenu() {
    clearScreen();

    char username[32];
    char password[32];

    printf("Enter your username ['cancel' to go back]: ");
    scanf("%s", username);
    if (strcmp(username, "cancel") == 0) {
        homePage();
        return;
    }

    printf("Enter your password: ");
    scanf("%s", password);

    UserData *current = userList.head;
    while (current != nullptr) {
        if (strcmp(current->username, username) == 0 && strcmp(current->password, password) == 0) {
            currentUser = *current;

            printf("Login successful!\n");
            if (current->hasAdminRights) {
                printf("\nYou have admin rights!\n\n");
            }
            printf("Press enter to continue...");
            getchar();
            getchar();
            loggedInMenu();
            return;
        }
        current = current->next;
    }

    printf("Login failed!\n");
    printf("Press enter to continue...");
    getchar();
    getchar();
    homePage();
}

void registerMenu() {
    clearScreen();

    char username[32];
    char password[32];

    do {
        printf("Enter your username [5..12 characters | 'cancel' to go back]: ");
        scanf("%s", username);

        // check if uname already exists
        bool exists = false;
        UserData *current = userList.head;
        while (current != nullptr) {
            if (strcmp(current->username, username) == 0) {
                printf("Username already exists!\n");
                exists = true;
                break;
            }
            current = current->next;
        }

        if (exists) {
            continue;
        }
    } while (strlen(username) < 5 || strlen(username) > 12 || strcmp(username, "cancel") == 0);
    if (strcmp(username, "cancel") == 0) {
        homePage();
        return;
    }

    do {
        printf("Enter your password [5..12 alphanumeric characters | 'cancel' to go back]: ");
        scanf("%s", password);
    } while (strlen(password) < 5 || strlen(password) > 12 || strcmp(password, "cancel") == 0 ||
             !isAlphanumeric(password));
    if (strcmp(password, "cancel") == 0) {
        homePage();
        return;
    }

    UserData *newUser = createUserData(username, password, false);
    userList.pushTail(newUser);

    // check if admin account exists
    bool adminExists = false;
    UserData *current = userList.head;
    while (current != nullptr) {
        if (current->hasAdminRights) {
            adminExists = true;
            break;
        }
        current = current->next;
    }

    if (!adminExists) {
        UserData *newAdminUser = createUserData("admin", "admin221", true);
        userList.pushTail(newAdminUser);
    }

    saveFile();
    currentUser = *newUser;

    printf("Registration successful!\n");
    printf("Press enter to continue...");
    getchar();
    getchar();
    loggedInMenu();
}

bool isAlphanumeric(char password[32]) {
    bool hasAlpha = false;
    bool hasDigit = false;

    for (int i = 0; i < strlen(password); i++) {
        if (isalpha(password[i])) {
            hasAlpha = true;
        } else if (isdigit(password[i])) {
            hasDigit = true;
        } else {
            return false;
        }
    }

    return hasAlpha && hasDigit;
}

void loggedInMenu() {
    if (currentUser.hasAdminRights) {
        int choice = 0;
        do {
            clearScreen();
            printf("MyDramaList\n");
            printf("=============\n");
            printf("1. Add Drama\n");
            printf("2. Update Drama\n");
            printf("3. Delete Drama\n");
            printf("4. Drama List\n");
            printf("5. Logout\n");
            printf(">> ");

            scanf("%d", &choice);
        } while (choice < 1 || choice > 5);

        switch (choice) {
            case 1:
                addDrama();
                break;
            case 2:
                updateDrama();
                break;
            case 3:
                deleteDrama();
                break;
            case 4:
                viewDramaList();
                break;
            case 5:
                currentUser = {};
                homePage();
                break;
            default:
                break;
        }
    } else {
        int choice = 0;
        do {
            clearScreen();
            printf("MyDramaList\n");
            printf("=============\n");
            printf("1. Add Watch Later\n");
            printf("2. Watch Later\n");
            printf("3. Add queue\n");
            printf("4. Watch Queue\n");
            printf("5. Drama List\n");
            printf("6. Logout\n");
            printf(">> ");

            scanf("%d", &choice);
        } while (choice < 1 || choice > 6);

        switch (choice) {
            case 1:
                addWatchLater();
                break;
            case 2:
                removeFromWatchLater();
                break;
            case 3:
                addQueue();
                break;
            case 4:
                removeFromQueue();
                break;
            case 5:
                viewDramaList();
                break;
            case 6:
                currentUser = {};
                homePage();
                break;
            default:
                break;
        }
    }
}

void addWatchLater() {
    clearScreen();
    printDramaListTable();
    getchar();

    char title[32];
    do {
        printf("Enter drama title [4..20 characters | 'cancel' to go back]: ");

        scanf("%[^\n]", title);
        getchar();

        if (isCancel(title)) {
            loggedInMenu();
            return;
        }
        if (!isTitleExists(title)) {
            printf("Drama title not found!\n");
            continue;
        }
    } while (strlen(title) < 4 || strlen(title) > 20 || strcasecmp(title, "cancel") == 0);

    Drama *current = dramaList.head;
    while (current != nullptr) {
        if (strcmp(current->title, title) == 0) {
            Drama *newDrama = createDrama(current->title, current->maleLead, current->femaleLead, current->rating,
                                          current->year);
            currentUser.watchLater.pushTail(newDrama);
            break;
        }
        current = current->next;
    }

    printf("Drama added to watch later!\n");
    printf("Press enter to continue...");
    getchar();
    getchar();
    loggedInMenu();
}

void removeFromWatchLater() {
    clearScreen();
    printWatchLaterTable();
    getchar();

    // get first one
    Drama *current = currentUser.watchLater.head;
    if (current == nullptr) {
        printf("Watch later is empty!\n");
        printf("Press enter to continue...");
        getchar();
        getchar();
        loggedInMenu();
        return;
    }

    printf("Press 'y' to watch '%s' or press 'cancel' to return: ", current->title);
    char choice[32];
    scanf("%s", choice);
    getchar();

    if (isCancel(choice)) {
        loggedInMenu();
        return;
    }

    if (strcmp(choice, "y") == 0) {
        currentUser.watchLater.popHead();
    }

    printf("Hope you enjoyed %s!\n", current->title);
    printf("Press enter to continue...");
    getchar();
    getchar();
    loggedInMenu();
}

void printWatchLaterTable() {
    printf("+----------------------------------------------------------------------------------------------+\n");
    printf("|                                            MyDrama List                                      |\n");
    printf("+-----+----------------------+-----------------+-----------------+--------+--------------------+\n");
    printf("| %-3s | %-20s | %-15s | %-15s | %-5s | %-18s |\n", "No.", "Title", "Male Lead", "Female Lead", "Rating",
           "Release Year");
    printf("+-----+----------------------+-----------------+-----------------+--------+--------------------+\n");

    Drama *current = currentUser.watchLater.head;
    int i = 1;

    while (current != nullptr) {
        printf("| %-3d | %-20s | %-15s | %-15s | %-6.1lf | %-18d |\n", i, current->title, current->maleLead,
               current->femaleLead, current->rating, current->year);
        printf("+-----+----------------------+-----------------+-----------------+--------+--------------------+\n");
        current = current->next;
        i++;
    }
}

void addQueue() {
    clearScreen();
    printDramaListTable();
    getchar();

    char title[32];
    do {
        printf("Enter drama title [4..20 characters | 'cancel' to go back]: ");

        scanf("%[^\n]", title);
        getchar();

        if (!isTitleExists(title)) {
            printf("Drama title not found!\n");
            continue;
        }
        if (isCancel(title)) {
            loggedInMenu();
            return;
        }
    } while (strlen(title) < 4 || strlen(title) > 20 || strcasecmp(title, "cancel") == 0);

    Drama *current = dramaList.head;
    while (current != nullptr) {
        if (strcmp(current->title, title) == 0) {
            Drama *newDrama = createDrama(current->title, current->maleLead, current->femaleLead, current->rating,
                                          current->year);
            currentUser.queue.pushTail(newDrama);
            break;
        }
        current = current->next;
    }

    printf("Drama added to queue!\n");
    printf("Press enter to continue...");
    getchar();
    getchar();
    loggedInMenu();
}

void removeFromQueue() {
    clearScreen();
    printQueueTable();
    getchar();

    // get first one
    Drama *current = currentUser.queue.head;
    if (current == nullptr) {
        printf("Queue is empty!\n");
        printf("Press enter to continue...");
        getchar();
        getchar();
        loggedInMenu();
        return;
    }

    printf("Press 'y' to watch '%s' or press 'cancel' to return: ", current->title);
    char choice[32];
    scanf("%s", choice);
    getchar();

    if (isCancel(choice)) {
        loggedInMenu();
        return;
    }

    if (strcmp(choice, "y") == 0) {
        currentUser.queue.popHead();
    }

    printf("Hope you enjoyed %s!\n", current->title);
    printf("Press enter to continue...");
    getchar();
    getchar();
    loggedInMenu();
}

void printQueueTable() {
    printf("+----------------------------------------------------------------------------------------------+\n");
    printf("|                                            MyDrama List                                      |\n");
    printf("+-----+----------------------+-----------------+-----------------+--------+--------------------+\n");
    printf("| %-3s | %-20s | %-15s | %-15s | %-5s | %-18s |\n", "No.", "Title", "Male Lead", "Female Lead", "Rating",
           "Release Year");
    printf("+-----+----------------------+-----------------+-----------------+--------+--------------------+\n");

    Drama *current = currentUser.queue.head;
    int i = 1;

    while (current != nullptr) {
        printf("| %-3d | %-20s | %-15s | %-15s | %-6.1lf | %-18d |\n", i, current->title, current->maleLead,
               current->femaleLead, current->rating, current->year);
        printf("+-----+----------------------+-----------------+-----------------+--------+--------------------+\n");
        current = current->next;
        i++;
    }
}

void addDrama() {
    clearScreen();

    char title[32]{};
    char maleLead[32]{};
    char femaleLead[32]{};
    double rating;
    int year;
//DOU367;Doughnut Name's;Doughnut Desccc;large;69;0
//ADM367;Admin Doughnut;oughnut Desc;large;100;0
    do {
        printf("Enter drama title [4..20 characters | 'cancel' to go back]: ");

        scanf("%[^\n]", title);
        getchar();

        if (isCancel(title)) {
            loggedInMenu();
            return;
        }
        if (isTitleExists(title)) {
            printf("Title already exists!\n");
            continue;
        }
    } while (strlen(title) < 4 || strlen(title) > 20 || strcasecmp(title, "cancel") == 0);

    do {
        printf("Enter drama male lead [6..15 characters | 'cancel' to go back]: ");

        scanf("%[^\n]", maleLead);
        getchar();
        if (isCancel(maleLead)) {
            loggedInMenu();
            return;
        }
    } while (strlen(maleLead) < 6 || strlen(maleLead) > 15 || strcasecmp(maleLead, "cancel") == 0);

    do {
        printf("Enter drama female lead [6..15 characters | 'cancel' to go back]: ");

        scanf("%[^\n]", femaleLead);
        getchar();
        if (isCancel(femaleLead)) {
            loggedInMenu();
            return;
        }
    } while (strlen(femaleLead) < 6 || strlen(femaleLead) > 15 || strcasecmp(femaleLead, "cancel") == 0);

    do {
        printf("Enter drama rating [2.5..5.0 | '0' to go back]: ");

        scanf("%lf", &rating);
        getchar();
        if (rating == 0) {
            loggedInMenu();
            return;
        }
    } while (rating < 2.5 || rating > 5.0 || rating == 0);

    do {
        printf("Enter drama release year [2012 - 2022 | '0' to go back]: ");

        scanf("%d", &year);
        getchar();
        if (year == 0) {
            loggedInMenu();
            return;
        }
    } while (year < 2012 || year > 2022 || year == 0);

    Drama *newDrama = createDrama(title, maleLead, femaleLead, rating, year);
    dramaList.pushTail(newDrama);

    saveFile();
    printf("Drama added successfully!\n");
    printf("Press enter to continue...");
    getchar();
    getchar();
    loggedInMenu();
}

void printDramaListTable() {
    printf("+----------------------------------------------------------------------------------------------+\n");
    printf("|                                            MyDrama List                                      |\n");
    printf("+-----+----------------------+-----------------+-----------------+--------+--------------------+\n");
    printf("| %-3s | %-20s | %-15s | %-15s | %-5s | %-18s |\n", "No.", "Title", "Male Lead", "Female Lead", "Rating",
           "Release Year");
    printf("+-----+----------------------+-----------------+-----------------+--------+--------------------+\n");

    Drama *current = dramaList.head;
    int i = 1;
    while (current != nullptr) {
        printf("| %-3d | %-20s | %-15s | %-15s | %-6.1lf | %-18d |\n", i, current->title, current->maleLead,
               current->femaleLead, current->rating, current->year);
        printf("+-----+----------------------+-----------------+-----------------+--------+--------------------+\n");
        current = current->next;
        i++;
    }
}

void updateDrama() {
    clearScreen();
    printDramaListTable();

    int index;
    do {
        printf("Enter drama index to update [1..%d | '0' to go back]: ", dramaList.getSize());

        scanf("%d", &index);
        getchar();
        if (index == 0) {
            loggedInMenu();
            return;
        }
    } while (index < 1 || index > dramaList.getSize() || index == 0);

    Drama *current = dramaList.head;
    int i = 1;
    while (current != nullptr) {
        if (i == index) {
            break;
        }
        current = current->next;
        i++;
    }

    // mintain ulang datane
    char title[32];
    char maleLead[32];
    char femaleLead[32];
    double rating;
    int year;

    do {
        printf("Enter drama title [4..20 characters | 'cancel' to go back]: ");

        scanf("%[^\n]", title);
        getchar();
        if (isCancel(title)) {
            loggedInMenu();
            return;
        }
    } while (strlen(title) < 4 || strlen(title) > 20 || strcasecmp(title, "cancel") == 0);

    do {
        printf("Enter drama male lead [6..15 characters | 'cancel' to go back]: ");

        scanf("%[^\n]", maleLead);
        getchar();
        if (isCancel(maleLead)) {
            loggedInMenu();
            return;
        }
    } while (strlen(maleLead) < 6 || strlen(maleLead) > 15 || strcasecmp(maleLead, "cancel") == 0);

    do {
        printf("Enter drama female lead [6..15 characters | 'cancel' to go back]: ");

        scanf("%[^\n]", femaleLead);
        getchar();
        if (isCancel(femaleLead)) {
            loggedInMenu();
            return;
        }
    } while (strlen(femaleLead) < 6 || strlen(femaleLead) > 15 || strcasecmp(femaleLead, "cancel") == 0);

    do {
        printf("Enter drama rating [2.5..5.0 | 'cancel' to go back]: ");

        scanf("%lf", &rating);
        getchar();
        if (rating == 0) {
            loggedInMenu();
            return;
        }
    } while (rating < 2.5 || rating > 5.0 || rating == 0);

    do {
        printf("Enter drama release year [2012 - 2022 | 'cancel' to go back]: ");

        scanf("%d", &year);
        getchar();
        if (year == 0) {
            loggedInMenu();
            return;
        }
    } while (year < 2012 || year > 2022 || year == 0);

    strcpy(current->title, title);
    strcpy(current->maleLead, maleLead);
    strcpy(current->femaleLead, femaleLead);
    current->rating = rating;
    current->year = year;
    saveFile();

    printf("Drama updated successfully!\n");
    printf("Press enter to continue...");
    getchar();
    getchar();
    loggedInMenu();
}

void deleteDrama() {
    clearScreen();
    printDramaListTable();
    getchar();

    char title[32];
    do {
        printf("Enter drama title [4..20 characters | 'cancel' to go back]: ");

        scanf("%[^\n]", title);
        getchar();

        if (isCancel(title)) {
            loggedInMenu();
            return;
        }
        if (!isTitleExists(title)) {
            printf("Drama title not found!\n");
            continue;
        }
    } while (strlen(title) < 4 || strlen(title) > 20 || strcasecmp(title, "cancel") == 0);

    Drama *current = dramaList.head;
    while (current != nullptr) {
        if (strcmp(current->title, title) == 0) {
            dramaList.remove(current);
            break;
        }
        current = current->next;
    }

    saveFile();

    printf("Drama deleted successfully!\n");
    printf("Press enter to continue...");
    getchar();
    getchar();
    loggedInMenu();
}

void viewDramaList() {
    clearScreen();
    printDramaListTable();

    printf("Press enter to go back...");
    getchar();
    getchar();
    loggedInMenu();
}

bool isTitleExists(char title[32]) {
    Drama *current = dramaList.head;
    while (current != nullptr) {
        if (strcmp(current->title, title) == 0) {
            return true;
        }
        current = current->next;
    }

    return false;
}

bool isCancel(char input[32]) {
    return strcasecmp(input, "cancel") == 0;
}

int main() {
    readFile();
    homePage();

    return 0;
}
