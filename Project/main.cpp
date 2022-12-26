#include <cstdio>
#include <cstdlib>
#include <cstring>

#ifdef _WIN32
#include <windows.h>
#else

#include <termios.h>
#include <unistd.h>

#endif

#include <cctype>
#include "database/Database.h"
#include "manager/FileManager.h"

void hideUserInput(bool hide) {
    if (hide) {
        // hide using asterisk
#ifdef _WIN32
        HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
        DWORD mode = 0;
        GetConsoleMode(hStdin, &mode);
        SetConsoleMode(hStdin, mode & (~ENABLE_ECHO_INPUT));
#else
        system("stty -echo");
#endif
    } else {
        // show using asterisk
#ifdef _WIN32
        HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
        DWORD mode = 0;
        GetConsoleMode(hStdin, &mode);
        SetConsoleMode(hStdin, mode | ENABLE_ECHO_INPUT);
#else
        system("stty echo");
#endif
    }

}


/*
 * ==================
 * #                #
 * #  BASIC STUFF   #
 * #                #
 * ==================
 */
void printLDLogo() {
    printf("\n");
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

void landingPage();

void loginMenu();

void registerMenu();

void homePage();

void searchFilmMenu();

void uploadFilmMenu();

void returnFilmMenu();

void favouritedFilmMenu();

void filmDetailPage(Database::Film film);


bool isValidAlnum(char *str);

void borrowFilm(Database::Film film);

/*
 * ==================
 * #                #
 * #  STUFF IMPL    #
 * #                #
 * ==================
 */

void landingPage() {
    int choice = 0;
    do {
        clearScreen();
        printf("filMZ\n");
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
            break;
        default:
            break;
    }
}

void loginMenu() {
    clearScreen();

    char username[128]{};
    char password[128]{};

    printf("Username: ");
    getchar();
    scanf("%[^\n]", username);
    getchar();

    printf("Password: ");
    hideUserInput(true);
    scanf("%[^\n]", password);
    getchar();
    hideUserInput(false);

    auto user = Database::getUser(username);
    if (user == nullptr || strcmp(user->password, password) != 0) {
        printf("\nLogin failed!\n");
        printf("Press enter to continue...");
        getchar();

        landingPage();
        return;
    }

    Database::setCurrentUser(user);

    printf("\nLogin success!\n");
    printf("Press enter to continue...");
    getchar();

    homePage();
}

void registerMenu() {
    clearScreen();

    char username[128]{};
    char password[128]{};

    getchar();
    do {
        printf("Username [8 .. 30 characters]: ");
        scanf("%[^\n]", username);
        getchar();

        if (strlen(username) < 8) {
            printf("Username must be at least 8 characters long!\n");
            continue;
        }

        if (strlen(username) > 30) {
            printf("Username must be at most 30 characters long!\n");
            continue;
        }

        if (!isValidAlnum(username)) {
            printf("Username must only contain alphanumeric characters!\n");
            continue;
        }

        if (Database::getUser(username) != nullptr) {
            printf("Username already taken!\n");
            continue;
        }

        break;
    } while (true);

    getchar();
    do {
        printf("Password [8 .. 30 characters]: ");
        scanf("%[^\n]", password);
        getchar();

        if (strlen(password) < 8) {
            printf("Password must be at least 8 characters long!\n");
            continue;
        }

        if (strlen(password) > 30) {
            printf("Password must be at most 30 characters long!\n");
            continue;
        }
        break;
    } while (true);

    auto newUser = Database::createUserData(username, password, 300);
    Database::addUser(newUser);
    FileManager::saveUsersFile();

    Database::setCurrentUser(newUser);

    printf("\nRegister success!\n");
    printf("Press enter to continue...");
    getchar();
    getchar();

    homePage();
}

void homePage() {
    FileManager::readBorrowedFilm();
    clearScreen();

    printf("filMZ\n");
    printf("Hi %s!\n", Database::getCurrentUser()->username);
    printf("Your money: %d\n", Database::getCurrentUser()->money);
    printf("Last checked time: %s\n\n", Utils::getFormatteDate());

    printf("Menu:\n");
    printf("1. Search Film\n");
    printf("2. Upload Film\n");
    printf("3. Return Film\n");
    printf("4. Favourited Film\n");
    printf("0. Logout\n");

    int choice = -1;
    do {
        printf(">> ");
        scanf("%d", &choice);
    } while (choice < 0 || choice > 4);

    switch (choice) {
        case 1:
            searchFilmMenu();
            break;
        case 2:
            uploadFilmMenu();
            break;
        case 3:
            returnFilmMenu();
            break;
        case 4:
            favouritedFilmMenu();
            break;

        case 0:
            Database::setCurrentUser(nullptr);
            landingPage();
            break;
    }
}

void searchFilmMenu() {
    clearScreen();

    char keyword[128]{};
    printf(">> search >> ");
    getchar();
    scanf("%[^\n]", keyword);
    getchar();

    auto filmList = Database::getFilmFromKeyword(keyword);
    auto current = filmList.head;

    if (current == nullptr) {
        printf("\nPress enter to continue...");
        getchar();

        homePage();
        return;
    }

    printf("\nChoose film: \n");
    printf("0. Return\n");

    Database::Film filmArray[8]{};
    int i = 0;
    while (current != nullptr) {
        if (i == 4) {
            break;
        }

        filmArray[i] = current->film;
        printf("%d. %s\n", i + 1, current->film.name);
        current = current->next;
        i++;
    }

    int choice = -1;
    do {
        printf(">> ");
        scanf("%d", &choice);
    } while (choice < 0 || choice > i);

    if (choice == 0) {
        homePage();
        return;
    }

    auto film = filmArray[choice - 1];
    filmDetailPage(film);
}

void uploadFilmMenu() {
    clearScreen();

    char name[128]{};
    char description[1024]{};
    char genre[128]{};
    int price = 0;
    int duration = 0;
    Database::GenreList genreList{};

    printf(">> Upload Film Menu\n\n");

    getchar();
    do {
        printf(">> name >> ");
        scanf("%[^\n]", name);
        getchar();

        if (!isValidAlnum(name)) {
            printf("Username must only contain alphanumeric characters!\n");
            continue;
        }

        break;
    } while (true);

    do {
        printf(">> description >> ");
        scanf("%[^\n]", description);
        getchar();

        if (strlen(description) < 1) {
            printf("Description must be at least 1 characters long!\n");
            continue;
        }
        break;
    } while (true);

    do {
        printf(">> price (per minute) >> ");
        scanf("%d", &price);
    } while (price < 0);

    do {
        printf(">> duration (in minutes) >> ");
        scanf("%d", &duration);
    } while (duration < 0);

    auto film = Database::createFilm(name, description, price, duration, Database::getCurrentUser()->username);

    // split genre by comma
    getchar();
    do {
        printf(">> genres, seperated by comma\n");
        Database::printValidGenres();
        printf("\n>> ");
        scanf("%[^\n]", genre);
        getchar();

        bool valid = true;

        char *token = strtok(genre, ", ");
        while (token != nullptr) {
            if (!Database::isValidGenre(token)) {
                film->genres.clear();
                valid = false;
                break;
            }

            film->genres.add(token);
            token = strtok(nullptr, ", ");
        }

        if (!valid) {
            printf("Genre must only contain valid genre!\n");
            continue;
        }

        break;
    } while (true);

    Database::addFilm(film);
    FileManager::saveFilmsFile();

    printf("\nUpload success!\n");
    printf("Press enter to continue...");
    getchar();

    homePage();
}

void returnFilmMenu() {

}

void favouritedFilmMenu() {
    clearScreen();

    auto favourite = Database::getCurrentUser()->getFavouriteFilmList();
    auto current = favourite.head;
    if (current == nullptr) {
        printf("You have no favourite film!\n");
        printf("\nPress enter to continue...");
        getchar();
        getchar();

        homePage();
        return;
    }

    printf("\nChoose film: \n");
    printf("0. Return\n");

    Database::Film filmArray[128]{};
    int i = 0;

    while (current != nullptr) {
        filmArray[i] = current->film;
        printf("%d. %s\n", i + 1, current->film.name);
        current = current->next;
        i++;
    }

    int choice = -1;
    do {
        printf(">> ");
        scanf("%d", &choice);
    } while (choice < 0 || choice > i);

    if (choice == 0) {
        homePage();
        return;
    }

    auto film = filmArray[choice - 1];
    filmDetailPage(film);
}

void filmDetailPage(Database::Film film) {
    clearScreen();

    printf(">> Film Detail\n\n");
    printf("Name: %s\n", film.name);
    printf("Description: %s\n", film.description);
    printf("Genres: ");
    film.genres.print();
    printf("\n");
    printf("Price per minute: %d\n", film.price);
    printf("Duration: %d minute(s)\n", film.duration);
    printf("Uploader: %s\n\n", film.uploader);

    printf("Menu:\n");
    printf("1. Borrow Film\n");

    auto getFilm = Database::getFilm(film.name);
    bool isFavourited = Database::getCurrentUser()->isFavourited(getFilm);
    if (isFavourited) {
        printf("2. Unfavourite Film\n");
    } else {
        printf("2. Favourite Film\n");
    }
    printf("0. Return\n");

    int choice = -1;
    do {
        printf(">> ");
        scanf("%d", &choice);
    } while (choice < 0 || choice > 2);

    switch (choice) {
        case 1:
            borrowFilm(film);
            break;
        case 2:
            if (isFavourited) {
                Database::getCurrentUser()->removeFavourite(getFilm);
                FileManager::saveUsersFile();
                printf("\nFilm unfavourited!\n");
            } else {
                Database::getCurrentUser()->insertFavourite(getFilm);
                FileManager::saveUsersFile();
                printf("\nFilm favourited!\n");
            }


            printf("Press enter to continue...");
            getchar();
            getchar();
            filmDetailPage(film);
            break;
        case 0:
            homePage();
            break;
    }
}

void borrowFilm(Database::Film film) {
    clearScreen();

    if (strcasecmp(film.uploader, Database::getCurrentUser()->username) == 0) {
        printf("You cannot borrow your own film!\n");

        printf("Press enter to continue...");
        getchar();
        getchar();
        filmDetailPage(film);
        return;
    }

    printf(">> Film Detail\n\n");
    printf("Name: %s\n", film.name);
    printf("Description: %s\n", film.description);
    printf("Genres: ");
    film.genres.print();
    printf("\n");
    printf("Duration: %d minute(s)\n", film.duration);
    printf("Uploader: %s\n\n", film.uploader);

    printf("Your Money: %d\n", Database::getCurrentUser()->money);
    printf("Price per minute: %d\n", film.price);

    if (Database::getCurrentUser()->money < film.price) {
        printf("You don't have enough money!\n");

        printf("Press enter to continue...");
        getchar();
        getchar();
        filmDetailPage(film);
        return;
    }

    int borrowDuration = 0;
    do {
        printf("Borrow duration (>= %d minutes) >> ", film.duration);
        scanf("%d", &borrowDuration);

        if (borrowDuration < film.duration) {
            printf("You must borrow it for at least %d minutes!\n", film.duration);
            continue;
        }

        bool price = film.price * borrowDuration;
        if (price > Database::getCurrentUser()->money) {
            printf("You don't have enough money!\n");
            continue;
        }

        break;
    } while (true);

    FileManager::saveBorrowedFilm(film, Database::getCurrentUser()->username, borrowDuration);
}

bool isValidAlnum(char *str) {
    int length = strlen(str);
    for (int i = 0; i < length; ++i) {
        char c = str[i];
        if (!isalnum(c) && c != ' ') {
            return false;
        }
    }

    return true;
}

int main() {
    FileManager::readFilmsFile();
    FileManager::readUsersFile();

    Database::addValidGenre("romance");
    Database::addValidGenre("drama");
    Database::addValidGenre("action");
    Database::addValidGenre("mecha");
    Database::addValidGenre("horror");
    Database::addValidGenre("fantasy");
    Database::addValidGenre("comedy");
    Database::addValidGenre("adventure");

    landingPage();

//    auto user = Database::getUser("asdf");
//    user->insertFavourite(Database::getFilm("evangelion"));
//
//    FileManager::saveUsersFile();
    return 0;
}
