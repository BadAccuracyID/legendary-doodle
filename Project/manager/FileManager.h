//
// Created by badaccuracyid on 12/11/22.
//

#ifndef GNINIARTERPSD_FILEMANAGER_H
#define GNINIARTERPSD_FILEMANAGER_H

#include "../database/Database.h"
#include <cstdlib>
#include <cstdio>

class FileManager {
public:
    static void readFilmsFile();

    static void saveFilmsFile();

    static void readUsersFile();

    static void saveUsersFile();

    static void saveBorrowedFilm(Database::Film film, char *borrowerName, int duration);

    static void readBorrowedFilm();
};


#endif //GNINIARTERPSD_FILEMANAGER_H
