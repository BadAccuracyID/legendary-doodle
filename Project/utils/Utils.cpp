//
// Created by badaccuracyid on 12/11/22.
//

#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include "Utils.h"

unsigned long Utils::djb2_hash(const char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }

    return hash;
}

static char *getDay(int i) {
    // Mon, Tue, Wed, Thu, Fri, Sat, Sun
    char *day = (char *) malloc(4);
    switch (i) {
        case 0:
            strcpy(day, "Mon");
            break;
        case 1:
            strcpy(day, "Tue");
            break;
        case 2:
            strcpy(day, "Wed");
            break;
        case 3:
            strcpy(day, "Thu");
            break;
        case 4:
            strcpy(day, "Fri");
            break;
        case 5:
            strcpy(day, "Sat");
            break;
        case 6:
            strcpy(day, "Sun");
            break;
        default:
            strcpy(day, "Mon");
            break;
    }

    return day;
}

static char *getMonth(int i) {
    // Jan, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec
    char *month = (char *) malloc(4);
    switch (i) {
        case 0:
            strcpy(month, "Jan");
            break;
        case 1:
            strcpy(month, "Feb");
            break;
        case 2:
            strcpy(month, "Mar");
            break;
        case 3:
            strcpy(month, "Apr");
            break;
        case 4:
            strcpy(month, "May");
            break;
        case 5:
            strcpy(month, "Jun");
            break;
        case 6:
            strcpy(month, "Jul");
            break;
        case 7:
            strcpy(month, "Aug");
            break;
        case 8:
            strcpy(month, "Sep");
            break;
        case 9:
            strcpy(month, "Oct");
            break;
        case 10:
            strcpy(month, "Nov");
            break;
        case 11:
            strcpy(month, "Dec");
            break;
        default:
            strcpy(month, "Jan");
            break;
    }

    return month;
}

char *Utils::getFormatteDate() {
    time_t now = time(nullptr);
    tm *ltm = localtime(&now);

    char *time = (char *) malloc(32);
    // Mon Oct 10 16:00:00 2011
    sprintf(time, "%s %s %d %d:%d:%d %d", getDay(ltm->tm_wday), getMonth(ltm->tm_mon), ltm->tm_mday, ltm->tm_hour,
            ltm->tm_min, ltm->tm_sec, 1900 + ltm->tm_year);

    return time;
}
