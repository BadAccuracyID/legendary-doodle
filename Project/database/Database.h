#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"
//
// Created by badaccuracyid on 12/11/22.
//

#ifndef GNINIARTERPSD_DATABASE_H
#define GNINIARTERPSD_DATABASE_H

#include "../utils/Utils.h"
#include <cstdlib>
#include <cstdio>

/*
 * FILMS ARE IN TRIE DATA STRUCTURE
 * GENRES ARE STORED IN LINKED LIST
 * FAVOURITES ARE STORED IN HASH TABLE
 */

class Database {
private:
    static const int MAX_FAVOURITES = 200;
public:
    /*
    * ======== *
    *          *
    *  GENRE   *
    *          *
    * ======== *
    */
    struct Genre {
        char name[32]{};
        Genre *next = nullptr;
    };

    struct GenreList {
        Genre *head = nullptr;
        Genre *tail = nullptr;

        void add(const char *name) {
            auto *genre = createGenre(name);
            if (head == nullptr) {
                head = genre;
                tail = genre;
            } else {
                tail->next = genre;
                tail = genre;
            }
        }

        void clear() {
            auto *genre = head;
            while (genre != nullptr) {
                auto *next = genre->next;
                free(genre);
                genre = next;
            }
            head = nullptr;
            tail = nullptr;
        }

        void print() const {
            auto *genre = head;
            while (genre != nullptr) {
                printf("%s", genre->name);
                genre = genre->next;
                if (genre != nullptr) {
                    printf(", ");
                }
            }
        }
    };

    static Genre *createGenre(const char *name);

    /*
    * ======== *
    *          *
    *   FILM   *
    *          *
    * ======== *
    */
    struct Film {
        char name[128]{};
        char description[1024]{};
        int price = 0;
        int duration = 0;
        char uploader[128]{};

        GenreList genres{};

        Film *left = nullptr;
        Film *right = nullptr;
    };

    struct FilmL {
        Film film;
        FilmL *next = nullptr;

        explicit FilmL(Film film) {
            this->film = film;
        }
    };

    struct FilmList {
        Database::FilmL *head;
        Database::FilmL *tail;

        void add(Film *film) {
            auto *filmL = new FilmL(*film);
            if (head == nullptr) {
                head = filmL;
                tail = filmL;
            } else {
                tail->next = filmL;
                tail = filmL;
            }
        }
    };

    static Film *getFilmRoot();

    static Film *createFilm(const char *name, const char *description, int price, int duration, const char *uploader);

    static void addFilm(Film *film);

    static Film *getFilm(const char *name);

    static void removeFilm(Film *film);

    static FilmList getFilmFromKeyword(const char *keyword);


    /*
    * ======== *
    *          *
    *FAVOURITES*
    *          *
    * ======== *
    */
    struct Favourite {
        Film *film = nullptr;
        Favourite *next = nullptr;
    };

    static Favourite *createFavourite(Film *film);


    /*
    * ======== *
    *          *
    *  BORROW  *
    *          *
    * ======== *
    */
    struct BorrowedFilm {
        Film *film;
        long remainingTime;
        BorrowedFilm *next;
    };

    struct BorrowedFilmList {
        BorrowedFilm *head;
        BorrowedFilm *tail;

        void add(BorrowedFilm *borrowedFilm) {
            if (head == nullptr) {
                head = borrowedFilm;
                tail = borrowedFilm;
            } else {
                tail->next = borrowedFilm;
                tail = borrowedFilm;
            }
        }

        void remove(BorrowedFilm *borrowedFilm) {
            if (head == borrowedFilm) {
                head = borrowedFilm->next;
                if (head == nullptr) {
                    tail = nullptr;
                }
            } else {
                auto *current = head;
                while (current->next != borrowedFilm) {
                    current = current->next;
                }
                current->next = borrowedFilm->next;
                if (current->next == nullptr) {
                    tail = current;
                }
            }
        }
    };

    static BorrowedFilm *createBorrowedFilm(Film *film, long remainingTime);

    static void addBorrowedFilm(BorrowedFilm *borrowedFilm);

    static void removeBorrowedFilm(BorrowedFilm *borrowedFilm);

    /*
    * ======== *
    *          *
    * USER DATA*
    *          *
    * ======== *
    */
    struct UserData {
        char username[32]{};
        char password[32]{};
        int money = 300;

        Favourite *favouriteTable[MAX_FAVOURITES]{};
        BorrowedFilmList borrowedFilms{};
        UserData *next = nullptr;

    private:
        Favourite *insertFavouriteMecha(Favourite *curr, Favourite *node) {
            if (curr == nullptr) {
                return node;
            }

            curr->next = insertFavouriteMecha(curr->next, node);
        }

        Favourite *removeFavouriteMecha(Favourite *curr, Film *node) {
            if (curr == nullptr) {
                return nullptr;
            }

            if (strcasecmp(curr->film->name, node->name) == 0) {
                free(curr);
                return nullptr;
            }

            curr->next = removeFavouriteMecha(curr->next, node);
        }

        Favourite *searchFavouriteMecha(Favourite *curr, const char *name) {
            if (curr == nullptr) {
                return nullptr;
            }

            if (strcmp(curr->film->name, name) == 0) {
                return curr;
            }

            return searchFavouriteMecha(curr->next, name);
        }

    public:
        void insertFavourite(Favourite *favourite) {
            unsigned int index = Utils::djb2_hash(favourite->film->name) % MAX_FAVOURITES;
            favouriteTable[index] = insertFavouriteMecha(favouriteTable[index], favourite);
        }

        void insertFavourite(Film *film) {
            auto *favourite = createFavourite(film);
            insertFavourite(favourite);
        }

        void removeFavourite(Film *film) {
            unsigned int index = Utils::djb2_hash(film->name) % MAX_FAVOURITES;
            favouriteTable[index] = removeFavouriteMecha(favouriteTable[index], film);
        }

        bool isFavourited(const char *name) {
            unsigned int index = Utils::djb2_hash(name) % MAX_FAVOURITES;
            Favourite *favourite = searchFavouriteMecha(favouriteTable[index], name);
            return favourite != nullptr;
        }

        bool isFavourited(Film *film) {
            return isFavourited(film->name);
        }

        Favourite *getFavouriteList() {
            Favourite *head = nullptr;
            Favourite *tail = nullptr;

            for (auto curr: favouriteTable) {
                while (curr != nullptr) {
                    if (head == nullptr) {
                        head = curr;
                        tail = curr;
                    } else {
                        tail->next = curr;
                        tail = curr;
                    }
                    curr = curr->next;
                }
            }

            return head;
        }

        FilmList getFavouriteFilmList() {
            auto *fav = getFavouriteList();
            FilmList filmList{};

            while (fav != nullptr) {
                filmList.add(fav->film);
                fav = fav->next;
            }

            return filmList;
        }

        int getFavoriteCount() {
            int count = 0;
            auto fav = getFavouriteList();
            while (fav != nullptr) {
                count++;
                fav = fav->next;
            }

            return count;
        }
    };

    static UserData *createUserData(const char *username, const char *password, int money);

    static UserData *getCurrentUser();

    static void setCurrentUser(UserData *user);

    static FilmList getFavouriteFilmList();

    struct UserList {
        UserData *head;
        UserData *tail;

        void add(UserData *userData) {
            if (head == nullptr) {
                head = userData;
                tail = userData;
            } else {
                tail->next = userData;
                tail = userData;
            }
        }

        UserData *search(const char *username) const {
            auto *curr = head;
            while (curr != nullptr) {
                if (strcmp(curr->username, username) == 0) {
                    return curr;
                }
                curr = curr->next;
            }
            return nullptr;
        }
    };

    static UserList *getUserList();

    static void addUser(UserData *data);

    static UserData *getUser(const char *username);

    static void addValidGenre(const char *name);

    static bool isValidGenre(const char *name);

    static void printValidGenres();
};


#endif //GNINIARTERPSD_DATABASE_H

#pragma clang diagnostic pop