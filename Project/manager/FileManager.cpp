//
// Created by badaccuracyid on 12/11/22.
//

#include <cstdio>
#include <cstring>
#include <chrono>
#include "FileManager.h"
#include "../database/Database.h"

void FileManager::readFilmsFile() {
    FILE *file = fopen("films/films.txt", "r");

    char bufferTitle[100];
    char bufferDescription[100];
    int bufferPrice = 0;
    int bufferDuration = 0;
    char bufferGenre[128];
    char bufferUploader[100];
    while (fscanf(file, "%[^#]#%[^#]#%d#%d#%[^#]#%s\n", bufferTitle, bufferDescription, &bufferPrice, &bufferDuration,
                  bufferGenre, bufferUploader) != EOF) {
        auto film = Database::createFilm(bufferTitle, bufferDescription, bufferPrice, bufferDuration, bufferUploader);
        // split genres
        char *token = strtok(bufferGenre, ",");
        while (token != nullptr) {
            film->genres.add(token);
            token = strtok(nullptr, ",");
        }

        Database::addFilm(film);
    }
    fclose(file);
}

void saveFilmsFileMecha(Database::Film *root, FILE *file) {
    if (root == nullptr) {
        return;
    }

    saveFilmsFileMecha(root->left, file);
    fprintf(file, "%s#%s#%d#%d#", root->name, root->description, root->price, root->duration);
    Database::Genre *genre = root->genres.head;
    while (genre != nullptr) {
        fprintf(file, "%s", genre->name);
        genre = genre->next;
        if (genre != nullptr) {
            fprintf(file, ",");
        }
    }
    fprintf(file, "#%s\n", root->uploader);
    saveFilmsFileMecha(root->right, file);
}

void FileManager::saveFilmsFile() {
    FILE *file = fopen("films/films.txt", "w");
    if (file == nullptr) {
        return;
    }

    saveFilmsFileMecha(Database::getFilmRoot(), file);
    fclose(file);
}

void FileManager::readUsersFile() {
    FILE *file = fopen("users/users.txt", "r");

    char bufferUsername[100];
    char bufferPassword[100];
    int bufferBalance = 0;
    char bufferFavoriteFilms[100];
    while (fscanf(file, "%[^#]#%[^#]#%d#%[^\n]\n", bufferUsername, bufferPassword, &bufferBalance,
                  bufferFavoriteFilms) !=
           EOF) {
        auto user = Database::createUserData(bufferUsername, bufferPassword, bufferBalance);
        // split favorite films
        char *token = strtok(bufferFavoriteFilms, ",");
        while (token != nullptr) {
            if (strcmp(token, "-") == 0) {
                token = strtok(nullptr, ",");
                continue;
            }

            auto film = Database::getFilm(token);
            if (film != nullptr) {
                user->insertFavourite(film);
            }
            token = strtok(nullptr, ",");
        }

        Database::addUser(user);
    }
    fclose(file);
}

void FileManager::saveUsersFile() {
    FILE *file = fopen("users/users.txt", "w");

    Database::UserList *userList = Database::getUserList();
    Database::UserData *temp = userList->head;
    while (temp != nullptr) {
        fprintf(file, "%s#%s#%d#", temp->username, temp->password, temp->money);

        int size = temp->getFavoriteCount();
        if (size < 1) {
            fprintf(file, "%s\n", "-");
            temp = temp->next;
            continue;
        }

        int tableSize = sizeof(temp->favouriteTable) / sizeof(temp->favouriteTable[0]);
        for (int i = 0; i < tableSize; i++) {
            auto *curr = temp->favouriteTable[i];
            while (curr != nullptr) {
                fprintf(file, "%s", curr->film->name);
                size--;
                if (size > 0) {
                    fprintf(file, ",");
                }
                curr = curr->next;
            }
        }

        fprintf(file, "\n");
        temp = temp->next;
    }
    fclose(file);
}

void FileManager::readBorrowedFilm() {
    FILE *file = fopen("borrow/borrows.txt", "r");

    char bufferFilmName[100];
    char bufferBorrowerName[100];
    long bufferStartBorrowTimeStamp = 0;
    int bufferBorrowDuration = 0;
    while (fscanf(file, "%[^#]#%[^#]#%ld#%d\n", bufferFilmName, bufferBorrowerName, &bufferStartBorrowTimeStamp,
                  &bufferBorrowDuration) != EOF) {
        auto film = Database::getFilm(bufferFilmName);
        if (film == nullptr) {
            continue;
        }

        auto user = Database::getUser(bufferBorrowerName);
        if (user == nullptr) {
            continue;
        }

        // get current milis as long
        auto now = std::chrono::system_clock::now();
        auto durationNow = now.time_since_epoch();
        auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(durationNow).count();

        // actual duration = borrowed * 5 * 3600
        long actualDuration = bufferBorrowDuration * 5 * 3600;
        long remainingTime = bufferStartBorrowTimeStamp + actualDuration - millis;

        auto borrowedFilm = Database::createBorrowedFilm(film, remainingTime);
        if (user == Database::getCurrentUser()) {
            Database::addBorrowedFilm(borrowedFilm);
        }
    }
    fclose(file);
}

void FileManager::saveBorrowedFilm(Database::Film film, char *borrowerName, int duration) {
    FILE *file = fopen("borrow/borrows.txt", "w");
    // filmName#borrowerName#startBorrowTimeStamp#borrowDuration

    // get current milis as long
    auto now = std::chrono::system_clock::now();
    auto durationNow = now.time_since_epoch();
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(durationNow).count();

    fprintf(file, "%s#%s#%ld#%d\n", film.name, borrowerName, millis, duration);
    fclose(file);
}
