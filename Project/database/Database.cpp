//
// Created by badaccuracyid on 12/11/22.
//

#include <cstring>
#include <cstdlib>
#include "Database.h"

/*
 * FILMS ARE IN TRIE DATA STRUCTURE
 * GENRES ARE STORED IN LINKED LIST
 * FAVOURITES ARE STORED IN HASH TABLE
 */

/*
 * ======== *
 *          *
 *  GENRE   *
 *          *
 * ======== *
 */
Database::GenreList validGenres{};

Database::Genre *Database::createGenre(const char *name) {
    auto *genre = (Genre *) malloc(sizeof(Genre));
    strcpy(genre->name, name);

    return genre;
}

void Database::addValidGenre(const char *name) {
    validGenres.add(name);
}

void Database::printValidGenres() {
    validGenres.print();
}

bool Database::isValidGenre(const char *name) {
    auto *genre = validGenres.head;
    while (genre != nullptr) {
        if (strcasecmp(genre->name, name) == 0) {
            return true;
        }
        genre = genre->next;
    }
    return false;
}


/*
 * ======== *
 *          *
 *   FILM   *
 *          *
 * ======== *
 */
Database::Film *filmRoot;

Database::Film *
Database::createFilm(const char *name, const char *description, int price, int duration, const char *uploader) {
    auto *film = (Film *) malloc(sizeof(Film));
    strcpy(film->name, name);
    strcpy(film->description, description);
    strcpy(film->uploader, uploader);
    film->price = price;
    film->duration = duration;

    return film;
}

Database::Film *Database::getFilmRoot() {
    return filmRoot;
}

/*
 * ======== *
 *          *
 *FAVOURITES*
 *          *
 * ======== *
*/
Database::Favourite *Database::createFavourite(Database::Film *film) {
    auto *favourite = (Favourite *) malloc(sizeof(Favourite));
    favourite->film = film;

    return favourite;
}

/*
 * ======== *
 *          *
 *USER DATA *
 *          *
 * ======== *
*/
Database::UserList userList;
Database::UserData *currentUser;

Database::UserData *Database::createUserData(const char *username, const char *password, int money) {
    auto *userData = (UserData *) malloc(sizeof(UserData));
    strcpy(userData->username, username);
    strcpy(userData->password, password);
    userData->money = money;

    return userData;
}

void Database::addFilm(Database::Film *film) {
    if (filmRoot == nullptr) {
        filmRoot = film;
        return;
    }

    auto *current = filmRoot;
    while (true) {
        if (strcmp(film->name, current->name) < 0) {
            if (current->left == nullptr) {
                current->left = film;
                break;
            } else {
                current = current->left;
            }
        } else {
            if (current->right == nullptr) {
                current->right = film;
                break;
            } else {
                current = current->right;
            }
        }
    }

}

Database::Film *Database::getFilm(const char *name) {
    auto *current = filmRoot;
    while (current != nullptr) {
        if (strcmp(name, current->name) == 0) {
            return current;
        } else if (strcmp(name, current->name) < 0) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    return nullptr;
}

Database::FilmList Database::getFilmFromKeyword(const char *keyword) {
    auto *current = filmRoot;
    FilmList filmList{};

    while (current != nullptr) {
        if (strstr(current->name, keyword) != nullptr) {
            filmList.add(current);

            if (strcmp(keyword, current->name) > 0) {
                current = current->left;
            } else {
                current = current->right;
            }
        } else if (strcmp(keyword, current->name) < 0) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    return filmList;
}

void Database::removeFilm(Database::Film *film) {
    auto *current = filmRoot;
    auto *parent = filmRoot;
    while (current != nullptr) {
        if (strcmp(film->name, current->name) == 0) {
            if (current->left == nullptr && current->right == nullptr) {
                if (parent->left == current) {
                    parent->left = nullptr;
                } else {
                    parent->right = nullptr;
                }
            } else if (current->left == nullptr) {
                if (parent->left == current) {
                    parent->left = current->right;
                } else {
                    parent->right = current->right;
                }
            } else if (current->right == nullptr) {
                if (parent->left == current) {
                    parent->left = current->left;
                } else {
                    parent->right = current->left;
                }
            } else {
                auto *successor = current->right;
                auto *successorParent = current->right;
                while (successor->left != nullptr) {
                    successorParent = successor;
                    successor = successor->left;
                }

                if (successorParent->left == successor) {
                    successorParent->left = successor->right;
                } else {
                    successorParent->right = successor->right;
                }

                successor->left = current->left;
                successor->right = current->right;

                if (parent->left == current) {
                    parent->left = successor;
                } else {
                    parent->right = successor;
                }
            }

            free(current);
            break;
        } else if (strcmp(film->name, current->name) < 0) {
            parent = current;
            current = current->left;
        } else {
            parent = current;
            current = current->right;
        }
    }
}

Database::UserList *Database::getUserList() {
    return &userList;
}

void Database::addUser(Database::UserData *data) {
    getUserList()->add(data);
}

Database::UserData *Database::getUser(const char *username) {
    return getUserList()->search(username);
}

Database::UserData *Database::getCurrentUser() {
    return currentUser;
}

void Database::setCurrentUser(Database::UserData *user) {
    currentUser = user;
}

Database::FilmList Database::getFavouriteFilmList() {
    auto *fav = currentUser->getFavouriteList();
    FilmList filmList{};

    while (fav != nullptr) {
        filmList.add(fav->film);
        fav = fav->next;
    }

    return filmList;
}

Database::BorrowedFilm *Database::createBorrowedFilm(Database::Film *film, long remainingTime) {
    auto *borrowedFilm = (BorrowedFilm *) malloc(sizeof(BorrowedFilm));
    borrowedFilm->film = film;
    borrowedFilm->remainingTime = remainingTime;

    return borrowedFilm;
}

void Database::addBorrowedFilm(Database::BorrowedFilm *borrowedFilm) {
    currentUser->borrowedFilms.add(borrowedFilm);
}

void Database::removeBorrowedFilm(Database::BorrowedFilm *borrowedFilm) {
    currentUser->borrowedFilms.remove(borrowedFilm);
}
