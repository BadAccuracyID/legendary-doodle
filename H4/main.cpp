#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>

struct LoginData {
    char email[256]{};
    char password[256]{};

    bool isAdmin = false;

    LoginData *left = nullptr;
    LoginData *right = nullptr;
};

LoginData *loginDataRoot;
LoginData currentUser{};

LoginData *createLoginData(const char *email, const char *password, bool isAdmin) {
    auto *loginData = (LoginData *) malloc(sizeof(LoginData));
    strcpy(loginData->email, email);
    strcpy(loginData->password, password);
    loginData->isAdmin = isAdmin;
    return loginData;
}

void insertLoginData(LoginData *loginData) {
    if (loginDataRoot == nullptr) {
        loginDataRoot = loginData;
        return;
    }

    LoginData *current = loginDataRoot;
    while (true) {
        if (strcmp(loginData->email, current->email) < 0) {
            if (current->left == nullptr) {
                current->left = loginData;
                return;
            }
            current = current->left;
        } else {
            if (current->right == nullptr) {
                current->right = loginData;
                return;
            }
            current = current->right;
        }
    }
}

LoginData *searchLoginData(const char *email) {
    LoginData *current = loginDataRoot;
    while (current != nullptr) {
        if (strcasecmp(email, current->email) == 0) {
            return current;
        } else if (strcasecmp(email, current->email) < 0) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    return nullptr;
}

/*
 * ==================
 * #                #
 * #    DOUGHNUT    #
 * #                #
 * ==================
 */
struct Doughnut {
    char name[32]{};
    char description[64]{};
    char size[16]{};
    int price = 0;
    int sold = 0;

    char code[16]{};

    Doughnut *left = nullptr;
    Doughnut *right = nullptr;
};

Doughnut *doughnutRoot;

Doughnut *
createDoughnut(const char *name, const char *description, const char *size, int price, int sold, const char *code) {
    auto *doughnut = (Doughnut *) malloc(sizeof(Doughnut));
    strcpy(doughnut->name, name);
    strcpy(doughnut->description, description);
    strcpy(doughnut->size, size);
    doughnut->price = price;
    doughnut->sold = sold;
    strcpy(doughnut->code, code);
    return doughnut;
}

void insertDoughnut(Doughnut *doughnut) {
    if (doughnutRoot == nullptr) {
        doughnutRoot = doughnut;
        return;
    }

    Doughnut *current = doughnutRoot;
    while (true) {
        if (strcmp(doughnut->code, current->code) < 0) {
            if (current->left == nullptr) {
                current->left = doughnut;
                return;
            }
            current = current->left;
        } else {
            if (current->right == nullptr) {
                current->right = doughnut;
                return;
            }
            current = current->right;
        }
    }
}

Doughnut *searchDoughnut(const char *code) {
    Doughnut *current = doughnutRoot;
    while (current != nullptr) {
        if (strcmp(code, current->code) == 0) {
            return current;
        } else if (strcmp(code, current->code) < 0) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    return nullptr;
}

Doughnut *deleteDoughnutMecha(Doughnut *root, const char *code) {
    if (root == nullptr) {
        return nullptr;
    }

    if (strcmp(code, root->code) < 0) {
        root->left = deleteDoughnutMecha(root->left, code);
    } else if (strcmp(code, root->code) > 0) {
        root->right = deleteDoughnutMecha(root->right, code);
    } else {
        if (root->left == nullptr) {
            Doughnut *temp = root->right;
            free(root);
            return temp;
        } else if (root->right == nullptr) {
            Doughnut *temp = root->left;
            free(root);
            return temp;
        }

        Doughnut *temp = root->right;
        while (temp->left != nullptr) {
            temp = temp->left;
        }

        strcpy(root->code, temp->code);
        strcpy(root->name, temp->name);
        strcpy(root->description, temp->description);
        strcpy(root->size, temp->size);
        root->price = temp->price;

        root->right = deleteDoughnutMecha(root->right, temp->code);
    }
    return root;
}

void deleteDoughnut(const char *code) {
    doughnutRoot = deleteDoughnutMecha(doughnutRoot, code);
}

void printDoughnutAdminMecha(Doughnut *root) {
    if (root == nullptr) {
        return;
    }

    printDoughnutAdminMecha(root->left);
    printf("Doughnut Code: %s\n", root->code);
    printf("Doughnut Name: %s\n", root->name);
    printf("\n");
    printDoughnutAdminMecha(root->right);
}

void printDoughnutAdmin() {
    printDoughnutAdminMecha(doughnutRoot);
}

void printDoughnutCustomerMecha(Doughnut *root) {
    if (root == nullptr) {
        return;
    }

    printDoughnutCustomerMecha(root->left);
    printf("| %-6s | %-24s | %-32s | %-8s | %-5d |\n", root->code, root->name, root->description, root->size,
           root->price);
    printDoughnutCustomerMecha(root->right);
}

void printDoughnutCustomer() {
    printDoughnutCustomerMecha(doughnutRoot);
}

/*
 * ==================
 * #                #
 * #  BASIC STUFF   #
 * #                #
 * ==================
 */
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

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

/*
 * ==================
 * #                #
 * #     STUFF      #
 * #                #
 * ==================
 */
void mainPage();

void loginPage();

void registerPage();

void adminPage();

void addDoughnutPage();

void removeDoughnutPage();

void customerPage();

void viewDoughnutPage();

void searchDoughnutPage();

void bestSellerPage();


bool printDoughnuts();


bool isCancel(char input[32]);

bool isCancel(int input);

bool isEmailRegistered(char input[256]);

bool isValidEmail(char input[256]);

bool isValidPassword(char input[256]);

bool isDoughnutExists(char input[32]);

/*
 * ==================
 * #                #
 * #    File OPs    #
 * #                #
 * ==================
 */

void readUsers() {
    FILE *file = fopen("account.txt", "r");
    if (file == nullptr) {
        return;
    }

    char bufferEmail[256];
    char bufferPassword[256];
    while (fscanf(file, "%[^;];%s\n", bufferEmail, bufferPassword) != EOF) {
        bool isAdmin = strcmp(bufferEmail, "admin@redjack.com") == 0 && strcmp(bufferPassword, "admin123") == 0;
        auto loginData = createLoginData(bufferEmail, bufferPassword, isAdmin);
        insertLoginData(loginData);
    }
    fclose(file);
}

void readDoughnuts() {
    FILE *file = fopen("doughnuts.txt", "r");
    if (file == nullptr) {
        return;
    }

    char bufferCode[32];
    char bufferName[256];
    char bufferDescription[256];
    char bufferSize[32];
    int bufferPrice;
    int bufferSold;
    while (fscanf(file, "%[^;];%[^;];%[^;];%[^;];%d;%d\n", bufferCode, bufferName, bufferDescription, bufferSize,
                  &bufferPrice, &bufferSold) != EOF) {
        auto doughnut = createDoughnut(bufferName, bufferDescription, bufferSize, bufferPrice, bufferSold, bufferCode);
        insertDoughnut(doughnut);
    }
    fclose(file);
}

void saveUsersMecha(LoginData *root, FILE *file) {
    if (root == nullptr) {
        return;
    }

    saveUsersMecha(root->left, file);
    fprintf(file, "%s;%s\n", root->email, root->password);
    saveUsersMecha(root->right, file);
}

void saveUsers() {
    FILE *file = fopen("account.txt", "w");
    if (file == nullptr) {
        return;
    }

    saveUsersMecha(loginDataRoot, file);
    fclose(file);
}

void saveDoughnutsMecha(Doughnut *root, FILE *file) {
    if (root == nullptr) {
        return;
    }

    saveDoughnutsMecha(root->left, file);
    fprintf(file, "%s;%s;%s;%s;%d;%d\n", root->code, root->name, root->description, root->size, root->price,
            root->sold);
    saveDoughnutsMecha(root->right, file);
}

void saveDoughnuts() {
    FILE *file = fopen("doughnuts.txt", "w");
    if (file == nullptr) {
        return;
    }

    saveDoughnutsMecha(doughnutRoot, file);
    fclose(file);
}

/*
 * ==================
 * #                #
 * #  ACTUAL STUFF  #
 * #                #
 * ==================
 */
void mainPage() {
    clearScreen();

    int choice = 0;
    do {
        printf("KRIJJPY KREME\n");
        printf("===============\n");
        printf("1. Login\n");
        printf("2. Register\n");
        printf("3. Exit\n");
        printf(">> ");

        scanf("%d", &choice);
    } while (choice < 1 || choice > 3);

    switch (choice) {
        case 1: {
            loginPage();
            break;
        }
        case 2: {
            registerPage();
            break;
        }
        case 3: {
            clearScreen();
            printLDLogo();
            exit(0);
        }
        default: {
            break;
        }
    }
}

void loginPage() {
    clearScreen();
    char email[256]{};
    char password[256]{};

    printf("Login your account here!\n");
    printf("========================\n");
    printf("Type '0' to go back\n");

    do {
        printf("Email: ");
        scanf("%s", email);
        if (isCancel(email)) {
            mainPage();
            return;
        }
        if (!isValidEmail(email)) {
            printf("Email must ends with @redjack.com!\n");
            continue;
        }

        break;
    } while (true);

    do {
        printf("Password: ");
        scanf("%s", password);
        if (isCancel(password)) {
            mainPage();
            return;
        }
        break;
    } while (true);

    if (!isEmailRegistered(email)) {
        printf("Login failed! Invalid email or password!\n");
        printf("Press enter to continue...");
        getchar();
        getchar();
        loginPage();
        return;
    }

    auto loginData = searchLoginData(email);
    if (strcmp(loginData->password, password) != 0) {
        printf("Login failed! Invalid email or password!\n");
        printf("Press enter to continue...");
        getchar();
        getchar();
        loginPage();
        return;
    }

    bool isAdmin = loginData->isAdmin;

    printf("\nLogin successful!\n");
    if (isAdmin) {
        printf("You have admin rights!\n");
    }
    printf("Press enter to continue...");
    getchar();
    getchar();

    currentUser = *loginData;
    if (isAdmin) {
        adminPage();
        return;
    }

    customerPage();
}

void registerPage() {
    clearScreen();
    char email[256]{};
    char password[256]{};

    printf("Register your account here!\n");
    printf("===========================\n");
    printf("Type '0' to go back\n");

    do {
        printf("Email [ends with '@redjack.com']: ");
        scanf("%s", email);
        if (isCancel(email)) {
            mainPage();
            return;
        }
        if (!isValidEmail(email)) {
            printf("Email must ends with @redjack.com!\n");
            continue;
        }
        if (isEmailRegistered(email)) {
            printf("Email is already registered!\n");
            continue;
        }

        break;
    } while (true);

    do {
        printf("Password [5 .. 10 alphanumeric characters]: ");
        scanf("%s", password);
        if (isCancel(password)) {
            mainPage();
            return;
        }
        if (!isValidPassword(password)) {
            printf("Password must be 5 to 10 alphanumeric characters long!\n");
            continue;
        }

        break;
    } while (true);

    auto loginData = createLoginData(email, password, false);
    insertLoginData(loginData);
    saveUsers();

    printf("\nRegister successful!\n");
    printf("Press enter to continue...");
    getchar();
    getchar();

    currentUser = *loginData;
    customerPage();
}

void adminPage() {
    int choice = 0;
    do {
        clearScreen();
        printf("Admin Menu\n");
        printf("===============\n");
        printf("1. Add Doughnut\n");
        printf("2. Remove Doughnut\n");
        printf("3. Logout\n");
        printf(">> ");

        scanf("%d", &choice);
    } while (choice < 0 || choice > 3);

    switch (choice) {
        case 1: {
            addDoughnutPage();
            break;
        }
        case 2: {
            removeDoughnutPage();
            break;
        }
        case 3: {
            currentUser = {};
            mainPage();
            break;
        }
        default: {
            break;
        }
    }
}

void addDoughnutPage() {
    clearScreen();

    char name[32]{};
    char description[64]{};
    char size[16]{};
    int price = 0;

    char code[16]{};

    printf("Add doughnut here!\n");
    printf("====================\n");
    printf("Type '0' to go back\n");

    do {
        printf("Doughnut name [6 .. 20 characters]: ");
        getchar();
        scanf("%[^\n]", name);
        getchar();

        if (isCancel(name)) {
            adminPage();
            return;
        }
        if (strlen(name) < 6 || strlen(name) > 20) {
            printf("Name must be 6 to 20 characters!\n");
            continue;
        }

        break;
    } while (true);

    do {
        printf("Doughnut description [10 .. 30 characters]: ");
        scanf("%[^\n]", description);
        getchar();

        if (isCancel(description)) {
            adminPage();
            return;
        }
        if (strlen(description) < 10 || strlen(description) > 30) {
            printf("Description must be 10 to 30 characters!\n");
            continue;
        }

        break;
    } while (true);

    do {
        printf("Doughnut size ['personal' | 'medium' | 'large']: ");
        scanf("%s", size);
        getchar();

        if (isCancel(size)) {
            adminPage();
            return;
        }
        if (strcasecmp(size, "personal") != 0 && strcasecmp(size, "medium") != 0 && strcasecmp(size, "large") != 0) {
            printf("Size must be 'personal', 'medium', or 'large'!\n");
            continue;
        }

        break;
    } while (true);

    do {
        printf("Doughnut price [5 .. 100]: ");
        scanf("%d", &price);
        getchar();

        if (isCancel(price)) {
            adminPage();
            return;
        }
        if (price < 5 || price > 100) {
            printf("Doughnut price must be 5 to 100!\n");
            continue;
        }

        break;
    } while (true);

    // generate code
    // first 3 characters of doughnut name in uppercase
    code[0] = (char) toupper(name[0]);
    code[1] = (char) toupper(name[1]);
    code[2] = (char) toupper(name[2]);

    // 3 random number 0-9
    for (int i = 0; i < 3; i++) {
        int randomNum = rand() % 10;
        sprintf(code + 3 + i, "%d", randomNum);
    }

    auto doughnut = createDoughnut(name, description, size, price, 0, code);
    insertDoughnut(doughnut);
    saveDoughnuts();

    printf("\nDoughnut added successfully!\n");
    printf("Press enter to continue...");
    getchar();
    adminPage();
}

void removeDoughnutPage() {
    clearScreen();

    char code[16]{};
    if (!printDoughnuts()) {
        adminPage();
        return;
    }

    do {
        printf("\nDoughnut code: ");
        scanf("%s", code);
        getchar();

        if (isCancel(code)) {
            adminPage();
            return;
        }
        if (!isDoughnutExists(code)) {
            printf("Doughnut code doesn't exists!\n");
            continue;
        }

        break;
    } while (true);

    deleteDoughnut(code);
    saveDoughnuts();

    printf("\nDoughnut removed successfully!\n");
    printf("Press enter to continue...");
    getchar();
    adminPage();
}

void customerPage() {
    int choice = 0;
    do {
        clearScreen();
        printf("Customer Menu\n");
        printf("===============\n");
        printf("1. View Doughnut\n");
        printf("2. Search Doughnut\n");
        printf("3. Best Seller Doughnut\n");
        printf("4. Logout\n");
        printf(">> ");

        scanf("%d", &choice);
    } while (choice < 1 || choice > 4);

    switch (choice) {
        case 1: {
            viewDoughnutPage();
            break;
        }
        case 2: {
            searchDoughnutPage();
            break;
        }
        case 3: {
            bestSellerPage();
            break;
        }
        default: {
            currentUser = {};
            mainPage();
            break;
        }
    }
}

void viewDoughnutPage() {
    clearScreen();

    if (!printDoughnuts()) {
        customerPage();
        return;
    }

    printf("Press enter to continue...");
    getchar();
    getchar();
    customerPage();
}

void searchDoughnutPage() {
    clearScreen();

    char code[32]{};
    printf("Search doughnut here!\n");
    printf("====================\n");
    printf("Type '0' to go back\n");

    do {
        printf("Keyword: ");
        scanf("%s", code);
        getchar();

        if (isCancel(code)) {
            customerPage();
            return;
        }
        if (!isDoughnutExists(code)) {
            printf("Doughnut doesn't exists!\n");
            continue;
        }

        break;
    } while (true);

    auto doughnut = searchDoughnut(code);
    printf("\n");
    printf("%s details:\n", code);
    printf("--------------------\n");
    printf("Name: %s\n", doughnut->name);
    printf("Description: %s\n", doughnut->description);
    printf("Size: %s\n", doughnut->size);
    printf("Price: %d\n", doughnut->price);
    printf("\n");

    printf("\nPress enter to continue...");
    getchar();
    customerPage();
}

Doughnut *getBestSeller() {
    auto current = doughnutRoot;
    if (current == nullptr) {
        return nullptr;
    }

    Doughnut *candidate;
    int max = 0;
    while (current != nullptr) {
        if (current->sold > max || max == 0 || current->sold == max) {
            max = current->sold;
            candidate = current;
            current = current->right;
        } else {
            current = current->left;
        }
    }

    return candidate;
}

void bestSellerPage() {
    clearScreen();

    printf("BEST SELLER OF THE MONTH!\n");
    printf("\n\n");

    auto doughnut = getBestSeller();
    if (doughnut == nullptr) {
        printf("No doughnut sold yet!\n");
        printf("Press enter to continue...");
        getchar();
        getchar();
        customerPage();
        return;
    }

    printf("Name: %s\n", doughnut->name);
    printf("Description: %s\n", doughnut->description);
    printf("Size: %s\n", doughnut->size);
    printf("Price: %d\n", doughnut->price);
    printf("Sold: %d\n", doughnut->sold);
    printf("\n");

    printf("Press enter to continue...");
    getchar();
    getchar();
    customerPage();
}

bool printDoughnuts() {
    if (doughnutRoot == nullptr) {
        printf("No doughnuts available!\n");
        printf("Press enter to continue...");
        getchar();
        getchar();
        return false;
    }

    if (currentUser.isAdmin) {
        printf("List of doughnuts:\n");
        printf("==================\n");
        printf("Type '0' to go back\n");
        printDoughnutAdmin();
    } else {
        printf("List of doughnuts - KRIJJPY KREME\n");
        printf("===========================================================================================\n");
        printf("| %-6s | %-24s | %-32s | %-8s | %-5s |\n", "Code", "Name", "Description", "Size", "Price");
        printf("===========================================================================================\n");
        printDoughnutCustomer();
        printf("===========================================================================================\n");
    }

    return true;
}


bool isCancel(char input[32]) {
    return strcasecmp(input, "0") == 0 || input[0] == '0';
}

bool isCancel(int input) {
    return input == 0;
}

bool isEmailRegistered(char input[256]) {
    auto data = searchLoginData(input);
    return data != nullptr;
}

bool isValidEmail(char input[256]) {
    // must ends with @redjack.com
    char *emailCom = strcasestr(input, "@redjack.com");
    if (emailCom == nullptr) {
        return false;
    }

    return true;
}

bool isValidPassword(char input[256]) {
    // 5-10 characters
    if (strlen(input) < 5 || strlen(input) > 10) {
        return false;
    }

    // must be alphanumeric
    bool hasAlpha = false;
    bool hasDigit = false;

    for (int i = 0; i < strlen(input); i++) {
        if (isalpha(input[i])) {
            hasAlpha = true;
        } else if (isdigit(input[i])) {
            hasDigit = true;
        } else {
            return false;
        }
    }

    return hasAlpha && hasDigit;
}

bool isDoughnutExists(char input[32]) {
    auto data = searchDoughnut(input);
    return data != nullptr;
}

int main() {
    readUsers();
    readDoughnuts();

    mainPage();
    return 0;
}
