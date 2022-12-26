#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>

struct MemberData {
    char name[128]{};
    int age = 0;
    int weight = 0;
    char gender[10]{};

    char memberCode[16]{};

    MemberData *next = nullptr;
};

MemberData *createMemberData(const char *name, int age, int weight, const char *gender, const char *memberCode) {
    auto *memberData = (MemberData *) malloc(sizeof(MemberData));
    strcpy(memberData->name, name);
    memberData->age = age;
    memberData->weight = weight;
    strcpy(memberData->gender, gender);
    strcpy(memberData->memberCode, memberCode);
    memberData->next = nullptr;

    return memberData;
}

const int MAX_MEMBER = 9990;
MemberData *table[MAX_MEMBER]{};

int hash(const char *memberCode) {
    int sum = 0;
    for (int i = 0; i < strlen(memberCode); i++) {
        sum += (int) memberCode[i];
    }
    return sum % MAX_MEMBER;
}

MemberData *insertMemberChainingMecha(MemberData *curr, MemberData *node) {
    if (curr == nullptr) {
        return node;
    }
    curr->next = insertMemberChainingMecha(curr->next, node);
}

void insertMember(MemberData *node) {
    int index = hash(node->memberCode);
    table[index] = insertMemberChainingMecha(table[index], node);
}

MemberData *deleteMemberChainingMecha(MemberData *curr, const char *memberCode) {
    if (curr == nullptr) {
        return nullptr;
    }
    if (strcmp(curr->memberCode, memberCode) == 0) {
        free(curr);
        return curr->next;
    }

    curr->next = deleteMemberChainingMecha(curr->next, memberCode);
}

void deleteMember(const char *memberCode) {
    int index = hash(memberCode);
    table[index] = deleteMemberChainingMecha(table[index], memberCode);
}

MemberData *searchMemberChainingMecha(MemberData *curr, const char *memberCode) {
    if (curr == nullptr) {
        return nullptr;
    }
    if (strcmp(curr->memberCode, memberCode) == 0) {
        return curr;
    }

    return searchMemberChainingMecha(curr->next, memberCode);
}

bool doesMemberExists(const char *memberCode) {
    int index = hash(memberCode);
    MemberData *memberData = searchMemberChainingMecha(table[index], memberCode);
    return memberData != nullptr;
}

bool hasData() {
    for (auto &i: table) {
        if (i != nullptr) {
            return true;
        }
    }
    return false;
}

void insertMembership();

bool printMembershipTable();

void viewMembership();

void deleteMembership();

void printLogo() {
    printf(
            "______  _____ _____              \n"
            "| ___ \\/  ___|  __ \\                  \n"
            "| |_/ /\\ `--.| |  \\/_   _ _ __ ___    \n"
            "| ___ \\ `--. \\ | __| | | | '_ ` _ \\  \n"
            "| |_/ //\\__/ / |_\\ \\ |_| | | | | | | \n"
            "\\____/ \\____/ \\____/\\__, |_| |_| |_|\n"
            "                     __/ |              \n"
            "                    |___/               \n"
            "");
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

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void homePage() {
    int choice = 0;
    do {
        clearScreen();
        printLogo();

        printf("1. Insert Membership\n");
        printf("2. View Membership\n");
        printf("3. Delete Membership\n");
        printf("4. Exit\n");

        printf(">> ");
        scanf("%d", &choice);
        getchar();
    } while (choice < 1 || choice > 4);

    switch (choice) {
        case 1:
            insertMembership();
            break;
        case 2:
            viewMembership();
            break;
        case 3:
            deleteMembership();
            break;
        case 4:
            clearScreen();
            printLDLogo();
            exit(0);
        default:
            break;
    }
}

void viewMembership() {
    clearScreen();
    printLogo();

    if (printMembershipTable()) {
        printf("Press enter to continue...");
        getchar();
        homePage();
    }
}

void insertMembership() {
    clearScreen();
    printLogo();

    char name[128]{};
    do {
        printf("Input new member name [>= 3 characters]: ");
        scanf("%[^\n]", name);
        getchar();
    } while (strlen(name) < 3);

    int age = 0;
    do {
        printf("Input new member age [>= 12 years old]: ");
        scanf("%d", &age);
        getchar();
    } while (age < 12);

    int weight = 0;
    do {
        printf("Input new member weight [>= 32 kilograms]: ");
        scanf("%d", &weight);
        getchar();
    } while (weight < 32);

    char gender[10]{};
    do {
        printf("Input new member gender [male or female]: ");
        scanf("%s", gender);
        getchar();
    } while (strcasecmp(gender, "male") != 0 && strcasecmp(gender, "female") != 0);

    char memberCode[16]{};
    // generate random number 1-200
    int randomNumber = rand() % 200 + 1;
    sprintf(memberCode, "%d", randomNumber);
    int lastIndex = 0;
    if (randomNumber < 10) {
        lastIndex = 1;
    } else if (randomNumber < 100) {
        lastIndex = 2;
    } else {
        lastIndex = 3;
    }

    // if name has space get first letter of each word, else get first 2 characters
    if (strchr(name, ' ') == nullptr) {
        memberCode[lastIndex + 1] = (char) toupper(name[0]);
        memberCode[lastIndex + 2] = (char) toupper(name[1]);
        lastIndex = lastIndex + 2;
    } else {
        int i = lastIndex;
        for (int j = 0; j < strlen(name); j++) {
            if (j == 0) {
                memberCode[i] = (char) toupper(name[j]);
                i++;
            } else if (name[j] == ' ') {
                memberCode[i] = (char) toupper(name[j + 1]);
                i++;
            }
        }

        lastIndex = i - 1;
    }

    // insert first letter from gender to uppercase
    memberCode[lastIndex + 1] = (char) toupper((gender[0] - 32));

    int stuff = (age * weight) / 4;
    memberCode[lastIndex + 2] = (char) (stuff / 100 + '0');
    memberCode[lastIndex + 3] = (char) ((stuff % 100) / 10 + '0');
    memberCode[lastIndex + 4] = (char) (stuff % 10 + '0');

    auto memberData = createMemberData(name, age, weight, gender, memberCode);
    insertMember(memberData);

    printf("New member has been inserted!\n");
    printf("Name: %s | code %s\n", memberData->name, memberData->memberCode);
    printf("Press enter to continue...");
    getchar();

    homePage();
}

void deleteMembership() {
    clearScreen();
    printLogo();

    if (!printMembershipTable()) {
        return;
    }

    char memberCode[16]{};
    do {
        printf("Input member code to delete ['cancel' to go back]: ");
        scanf("%s", memberCode);
        getchar();

        if (strcasecmp(memberCode, "cancel") == 0) {
            homePage();
            return;
        }
        if (doesMemberExists(memberCode)) {
            break;
        }
        printf("Member code not found!\n");
    } while (true);

    deleteMember(memberCode);

    printf("Successfully deleted member with member code %s\n", memberCode);


    printf("Press enter to continue...");
    getchar();
    homePage();
}


bool printMembershipTable() {
    // if table emtpy
    if (!hasData()) {
        printf("Membership table is empty!\n");
        printf("Press enter to continue...");
        getchar();
        homePage();

        return false;
    }

    printf("Membership Table\n");
    printf("=================================================================================\n");
    // no, name, age, weight, gender, code
    printf("| %-3s | %-32s | %-4s | %-7s | %-7s | %-10s|\n", "No.", "Name", "Age", "Weight", "Gender", "Code");
    printf("=================================================================================\n");

    int tableSize = sizeof(table) / sizeof(table[0]);
    int i = 0;
    for (int j = 0; j < tableSize; j++) {
        MemberData *curr = table[j];
        while (curr != nullptr) {
            printf("| %-3d | %-32s | %-4d | %-7d | %-7s | %-10s|\n", i + 1, curr->name, curr->age, curr->weight,
                   curr->gender,
                   curr->memberCode);
            curr = curr->next;
            i++;
        }
    }

    printf("=================================================================================\n");
    return true;
}


int main() {
    homePage();
    return 0;
}
