#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>

struct AircraftData {
    char flightNumber[32];
    char model[64];
    char manufacturer[16];
    int capacity;

    AircraftData *next;
};

struct FlightSchedule {
    char date[16];
    char time[16];
    char flightNumber[32];
    char departure[64];
    char destination[64];

    FlightSchedule *next;
};

void mainMenu();

void manageAircraft();

void viewAircraft();

void deleteAircraft();

bool displayAircraftTable();

void addAircraft();

bool isFlightNumberExist(char flightNumber[32]);

bool isFlightNumberValid(char number[32]);

bool isModelValid(char model[64]);

bool isManufacturerValid(char manufacturer[16]);

void updateAircraft();

void pushHead(AircraftData *aircraftData);

AircraftData *createFromInput(char preSetFlightNumber[32] = nullptr);

AircraftData *head = nullptr;
AircraftData *tail = nullptr;

AircraftData *
createAircraftData(const char flightNumber[32], const char model[64], const char manufacturer[16], int capacity) {
    auto *aircraftData = (AircraftData *) malloc(sizeof(AircraftData));
    strcpy(aircraftData->flightNumber, flightNumber);
    strcpy(aircraftData->model, model);
    strcpy(aircraftData->manufacturer, manufacturer);
    aircraftData->capacity = capacity;
    aircraftData->next = nullptr;
    return aircraftData;
}

void manageSchedule();

void viewSchedule();

bool displayScheduleTable();

void deleteSchedule();

void addSchedule();

void pushSchedulePriority(FlightSchedule *scheduleData);

FlightSchedule *createScheduleFromInput(char preSetDate[16] = nullptr, char preSetTime[16] = nullptr,
                                        char preSetFlightNumber[32] = nullptr);

bool isDateValid(char date[16]);

bool isTimeValid(char time[16]);

bool isNumber(char hour[3]);

bool isDepartureValid(char departure[16]);

bool isDestinationValid(char destination[16], char departure[16]);

bool isDateBefore(const char date[16], const char date1[16]);

bool isTimeBefore(char time[16], char time1[16]);

void updateSchedule();

FlightSchedule *headSchedule = nullptr;
FlightSchedule *tailSchedule = nullptr;

FlightSchedule *createFlightSchedule(const char date[16], const char time[16], const char flightNumber[32],
                                     const char departure[64], const char destination[64]) {
    auto *flightSchedule = (FlightSchedule *) malloc(sizeof(FlightSchedule));
    strcpy(flightSchedule->date, date);
    strcpy(flightSchedule->time, time);
    strcpy(flightSchedule->flightNumber, flightNumber);
    strcpy(flightSchedule->departure, departure);
    strcpy(flightSchedule->destination, destination);
    flightSchedule->next = nullptr;
    return flightSchedule;
}

void printLogo() {
    printf("  _       __      ___           _            \n"
           " | |      \\ \\    / (_)         | |                \n"
           " | |     __\\ \\  / / _ _ __ __ _| |_ ___  ___      \n"
           " | |    / _ \\ \\/ / | | '__/ _` | __/ _ \\/ __|    \n"
           " | |___|  __/\\  /  | | | | (_| | ||  __/\\__ \\    \n"
           " |______\\___| \\/   |_|_|  \\__,_|\\__\\___||___/  \n"
           "                                                    \n");
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void mainMenu() {
    int option = 0;
    do {
        clearScreen();
        printLogo();
        printf("Choose menu: \n\n"
               "1. Manage Aircraft\n"
               "2. Manage Schedule\n"
               "3. Exit\n"
               "Input option >> ");
        scanf("%d", &option);
    } while (option < 1 || option > 3);

    switch (option) {
        case 1:
            manageAircraft();
            break;
        case 2:
            manageSchedule();
            break;
        case 3:
            exit(0);
        default:
            break;
    }
}

void manageAircraft() {
    int option = 0;
    do {
        clearScreen();
        printLogo();
        printf("Manage Aircraft menu: \n\n"
               "1. View Aircraft\n"
               "2. Add Aircraft\n"
               "3. Update Aircraft\n"
               "4. Delete Aircraft\n"
               "5. Exit\n"
               "Input option >> ");
        scanf("%d", &option);
    } while (option < 1 || option > 5);

    switch (option) {
        case 1:
            viewAircraft();
            break;
        case 2:
            addAircraft();
            break;
        case 3:
            updateAircraft();
            break;
        case 4:
            deleteAircraft();
            break;
        case 5:
            mainMenu();
            break;
        default:
            break;
    }

}

bool displayAircraftTable() {
    if (head == nullptr) {
        printf("No aircraft data available\n");
        return false;
    } else {
        printf("-----------------------------------------------------------------\n");
        printf("| %-15s | %-12s | %-15s | %-10s |\n", "Flight Number", "Model", "Manufacturer", "Capacity");
        printf("-----------------------------------------------------------------\n");

        auto *temp = head;
        while (temp != nullptr) {
            printf("| %-15s | %-12s | %-15s | %-10d |\n", temp->flightNumber, temp->model, temp->manufacturer,
                   temp->capacity);
            temp = temp->next;
        }
        printf("-----------------------------------------------------------------\n");
        return true;
    }
}

void viewAircraft() {
    clearScreen();
    displayAircraftTable();

    printf("Press enter to continue...");
    getchar();
    getchar();
    manageAircraft();
}

void addAircraft() {
    clearScreen();
    AircraftData *aircraftData = createFromInput();

    pushHead(aircraftData);

    printf("\nAircraft added successfully\n");
    printf("Press enter to continue...");
    getchar();
    manageAircraft();
}

void updateAircraft() {
    clearScreen();
    if (!displayAircraftTable()) {
        printf("Press enter to continue...");
        getchar();
        manageAircraft();
        return;
    }

    // using flightNumber
    char flightNumber[32];
    do {
        printf("Input flight number to update >> ");
        scanf("%s", flightNumber);
        getchar();
    } while (!isFlightNumberExist(flightNumber));

    // find the aircraft data
    bool found = false;
    auto *temp = head;
    while (temp != nullptr) {
        if (strcmp(temp->flightNumber, flightNumber) == 0) {
            found = true;
            break;
        }
        temp = temp->next;
    }

    // re-verify if found
    if (!found) {
        printf("Aircraft data not found\n");
        printf("Press enter to continue...");
        getchar();

        manageAircraft();
        return;
    }

    bool confirmed = false;
    do {
        clearScreen();
        AircraftData *aircraftData = createFromInput(flightNumber);

        clearScreen();
        printf("Aircraft Details:\n");
        printf("Flight Number: %s\n", aircraftData->flightNumber);
        printf("Model: %s\n", aircraftData->model);
        printf("Manufacturer: %s\n", aircraftData->manufacturer);
        printf("Capacity: %d\n\n", aircraftData->capacity);

        printf("Are you sure to update this aircraft data? (y/n) >> ");
        char confirmation;
        scanf("%c", &confirmation);
        if (confirmation == 'y') {
            strcpy(temp->flightNumber, aircraftData->flightNumber);
            strcpy(temp->model, aircraftData->model);
            strcpy(temp->manufacturer, aircraftData->manufacturer);
            temp->capacity = aircraftData->capacity;
            confirmed = true;
        }
    } while (!confirmed);

    printf("\nAircraft updated successfully\n");
    printf("Press enter to continue...");
    getchar();
    getchar();
    manageAircraft();
}

void deleteAircraft() {
    clearScreen();
    if (!displayAircraftTable()) {
        printf("Press enter to continue...");
        getchar();
        manageAircraft();
        return;
    }

    // using flightNumber
    char flightNumber[32];
    do {
        printf("Input flight number to delete >> ");
        scanf("%s", flightNumber);
        getchar();
    } while (!isFlightNumberExist(flightNumber));

    // find the aircraft data
    bool found = false;
    auto *temp = head;
    while (temp != nullptr) {
        if (strcmp(temp->flightNumber, flightNumber) == 0) {
            found = true;
            break;
        }
        temp = temp->next;
    }

    // re-verify if found
    if (!found) {
        printf("Aircraft data not found\n");
        printf("Press enter to continue...");
        getchar();

        manageAircraft();
        return;
    }

    // delete the aircraft data
    if (temp == head) {
        head = head->next;
        free(temp);
    } else if (temp == tail) {
        auto *prev = head;
        while (prev->next != tail) {
            prev = prev->next;
        }
        prev->next = nullptr;
        tail = prev;
        free(temp);
    } else {
        auto *prev = head;
        while (prev->next != temp) {
            prev = prev->next;
        }
        prev->next = temp->next;
        free(temp);
    }
}

bool isManufacturerValid(char manufacturer[16]) {
    if (strlen(manufacturer) < 1) {
        return false;
    }

    return isupper(manufacturer[0]);
}

bool isModelValid(char model[64]) {
    if (strncmp(model, "Airbus", 6) == 0 || strncmp(model, "Boeing", 6) == 0) {
        int spaceCount = 0;
        for (int i = 0; i < strlen(model); ++i) {
            if (isspace(model[i])) {
                spaceCount++;
            }
        }
        return spaceCount == 1;
    }
    return false;
}

bool isFlightNumberValid(char number[32]) {
    // all capital
    for (int i = 0; i < strlen(number); ++i) {
        if (isalpha(number[i])) {
            if (!isupper(number[i])) {
                return false;
            }
        }
    }

    // alphanumeric
    for (int i = 0; i < strlen(number); ++i) {
        if (!isalnum(number[i])) {
            return false;
        }
    }

    // chk for spaces
    for (int i = 0; i < strlen(number); ++i) {
        if (isspace(number[i])) {
            return false;
        }
    }

    return true;
}

bool isFlightNumberExist(char flightNumber[32]) {
    auto *temp = head;
    while (temp != nullptr) {
        if (strcmp(temp->flightNumber, flightNumber) == 0) {
            return true;
        }
        temp = temp->next;
    }
    return false;
}

AircraftData *createFromInput(char *preSetFlightNumber) {
    char flightNumber[32]{};
    char model[64]{};
    char manufacturer[16]{};
    int capacity = 0;

    if (preSetFlightNumber == nullptr) {
        printf("Add Aircraft menu: \n\n");
        do {
            printf("Input flight number (unique, capital alphanum) >> ");
            scanf("%s", flightNumber);
            getchar();

            if (!isFlightNumberValid(flightNumber)) {
                printf("Invalid flight number\n");
            } else if (isFlightNumberExist(flightNumber)) {
                printf("Flight number already exist\n");
            } else {
                break;
            }
        } while (true);
    } else {
        printf("Update Aircraft menu: \n\n");
        getchar();
        strcpy(flightNumber, preSetFlightNumber);
    }

    do {
        printf(R"(Input model (starts with "Airbus" or "Boeing" and have two words) >> )");
        scanf("%[^\n]", model);
        getchar();
    } while (!isModelValid(model));

    do {
        printf("Input manufacturer (starts with a capital letter) >> ");
        scanf("%s", manufacturer);
        getchar();
    } while (!isManufacturerValid(manufacturer));

    do {
        printf("Input capacity (greater than 500) >> ");
        scanf("%d", &capacity);
        getchar();
    } while (capacity <= 500);

    AircraftData *aircraftData = createAircraftData(flightNumber, model, manufacturer, capacity);
    return aircraftData;
}

void pushHead(AircraftData *aircraftData) {
    if (head == nullptr) {
        head = aircraftData;
        tail = aircraftData;
    } else {
        aircraftData->next = head;
        head = aircraftData;
    }
}

void manageSchedule() {
    int option = 0;
    do {
        clearScreen();
        printLogo();
        printf("Manage Schedule menu: \n\n"
               "1. View Schedule\n"
               "2. Add Schedule\n"
               "3. Update Schedule\n"
               "4. Done Schedule\n"
               "5. Exit\n"
               "Input option >> ");
        scanf("%d", &option);
    } while (option < 1 || option > 5);

    switch (option) {
        case 1:
            viewSchedule();
            break;
        case 2:
            addSchedule();
            break;
        case 3:
            updateSchedule();
            break;
        case 4:
            deleteSchedule();
            break;
        case 5:
            mainMenu();
            break;
        default:
            break;
    }
}

void updateSchedule() {
    clearScreen();
    if (!displayScheduleTable()) {
        printf("Press enter to continue...");
        getchar();
        getchar();
        manageSchedule();
        return;
    }

    // using date, time, and flightNumber
    char date[16];
    char time[16];
    char flightNumber[32];

    do {
        printf("Input date (dd/mm/yyyy) >> ");
        scanf("%s", date);
    } while (!isDateValid(date));

    do {
        printf("Input time (hh:mm) >> ");
        scanf("%s", time);
    } while (!isTimeValid(time));

    do {
        printf("Input flight number >> ");
        scanf("%s", flightNumber);
    } while (!isFlightNumberExist(flightNumber));

    // find the schedule data
    bool found = false;
    auto *temp = headSchedule;
    while (temp != nullptr) {
        if (strcmp(temp->date, date) == 0 && strcmp(temp->time, time) == 0 &&
            strcmp(temp->flightNumber, flightNumber) == 0) {
            found = true;
            break;
        }
        temp = temp->next;
    }

    // re-verify if found
    if (!found) {
        printf("Schedule data not found\n");
        printf("Press enter to continue...");
        getchar();

        manageSchedule();
        return;
    }

    bool confirmed = false;
    do {
        clearScreen();
        FlightSchedule *flightSchedule = createScheduleFromInput(date, time, flightNumber);

        clearScreen();
        printf("Flight Schedule Details:\n");
        printf("Date: %s\n", flightSchedule->date);
        printf("Time: %s\n", flightSchedule->time);
        printf("Flight Number: %s\n", flightSchedule->flightNumber);
        printf("Departure: %s\n", flightSchedule->departure);
        printf("Destination: %s\n", flightSchedule->destination);

        printf("Are you sure to update this flight schedule data? (y/n) >> ");
        char confirmation;
        scanf("%c", &confirmation);
        if (confirmation == 'y') {
            strcpy(temp->departure, flightSchedule->departure);
            strcpy(temp->destination, flightSchedule->destination);
            confirmed = true;
        }
    } while (!confirmed);

    printf("\nSchedule data updated\n");
    printf("Press enter to continue...");
    getchar();
    getchar();
    manageSchedule();
}

bool displayScheduleTable() {
    if (headSchedule == nullptr) {
        printf("No flight schedule data available\n");
        return false;
    } else {
        printf("------------------------------------------------------------------------------\n");
        printf("| %-15s | %-12s | %-15s | %-10s | %-10s |\n", "Date", "Time", "Flight Number", "From", "To");
        printf("------------------------------------------------------------------------------\n");

        auto *temp = headSchedule;
        while (temp != nullptr) {
            printf("| %-15s | %-12s | %-15s | %-10s | %-10s |\n", temp->date, temp->time, temp->flightNumber,
                   temp->departure, temp->destination);
            temp = temp->next;
        }
        printf("------------------------------------------------------------------------------\n");
        return true;
    }
}

void viewSchedule() {
    clearScreen();
    displayScheduleTable();

    printf("Press enter to continue...");
    getchar();
    getchar();
    manageSchedule();
}

void addSchedule() {
    clearScreen();

    if (head == nullptr) {
        printf("No aircraft data available\n");
        printf("Press enter to continue...");
        getchar();
        getchar();
        manageSchedule();
        return;
    }

    FlightSchedule *scheduleData = createScheduleFromInput();
    pushSchedulePriority(scheduleData);

    printf("Schedule added successfully\n");
    printf("Press enter to continue...");
    getchar();
    manageSchedule();
}

FlightSchedule *createScheduleFromInput(char preSetDate[16], char preSetTime[16], char preSetFlightNumber[32]) {
    char flightNumber[32]{};
    char date[16]{};
    char time[16]{};
    char departure[16]{};
    char destination[16]{};

    if (preSetDate == nullptr) {
        printf("Add Schedule menu: \n\n");
        do {
            printf("Input date (dd/mm/yyyy) >> ");
            scanf("%s", date);
            getchar();
        } while (!isDateValid(date));
    } else {
        printf("Update Schedule menu: \n\n");
        getchar();
        strcpy(date, preSetDate);
    }

    if (preSetTime == nullptr) {
        do {
            printf("Input time (hh:mm) >> ");
            scanf("%s", time);
            getchar();
        } while (!isTimeValid(time));
    } else {
        strcpy(time, preSetTime);
    }

    if (preSetFlightNumber == nullptr) {
        displayAircraftTable();
        do {
            printf("Input flight number (unique, capital alphanum) >> ");
            scanf("%s", flightNumber);
            getchar();

            if (!isFlightNumberValid(flightNumber)) {
                printf("Invalid flight number\n");
            } else if (isFlightNumberExist(flightNumber)) {
                printf("Flight number already exist\n");
            } else {
                break;
            }
        } while (true);
    } else {
        strcpy(flightNumber, preSetFlightNumber);
    }

    do {
        printf("Input departure (starts with capital letter, >= 5 letters) >> ");
        scanf("%[^\n]", departure);
        getchar();
        getchar();
    } while (!isDepartureValid(departure));

    do {
        printf("Input destination (starts with capital letter, >= 8 letters) >> ");
        scanf("%[^\n]", destination);
        getchar();
        getchar();
    } while (!isDestinationValid(destination, departure));

    FlightSchedule *scheduleData = createFlightSchedule(flightNumber, date, time, departure, destination);
    return scheduleData;
}

bool isDepartureValid(char departure[16]) {
    if (strlen(departure) < 5) {
        return false;
    }

    if (!isupper(departure[0])) {
        return false;
    }

    return true;
}

bool isDestinationValid(char destination[16], char departure[16]) {
    if (strlen(destination) < 8) {
        return false;
    }

    if (!isupper(destination[0])) {
        return false;
    }

    if (strcmp(departure, destination) == 0) {
        return false;
    }

    return true;
}

bool isTimeValid(char time[16]) {
    if (strlen(time) != 5) {
        return false;
    }

    if (time[2] != ':') {
        return false;
    }

    char hour[3]{};
    char minute[3]{};
    strncpy(hour, time, 2);
    strncpy(minute, time + 3, 2);

    if (!isNumber(hour) || !isNumber(minute)) {
        return false;
    }

    int hourInt = atoi(hour);
    int minuteInt = atoi(minute);

    if (hourInt < 0 || hourInt > 23) {
        return false;
    }

    if (minuteInt < 0 || minuteInt > 59) {
        return false;
    }

    return true;
}

bool isNumber(char hour[3]) {
    for (int i = 0; i < strlen(hour); ++i) {
        if (!isdigit(hour[i])) {
            return false;
        }
    }
    return true;
}

bool isDateValid(char date[16]) {
    // dd/mm/yyyy
    if (strlen(date) != 10) {
        return false;
    }

    // dd
    if (!isdigit(date[0]) || !isdigit(date[1])) {
        return false;
    }

    // mm
    if (!isdigit(date[3]) || !isdigit(date[4])) {
        return false;
    }

    // yyyy
    if (!isdigit(date[6]) || !isdigit(date[7]) || !isdigit(date[8]) || !isdigit(date[9])) {
        return false;
    }

    // /
    if (date[2] != '/' || date[5] != '/') {
        return false;
    }

    // month
    char month[3]{};
    month[0] = date[3];
    month[1] = date[4];
    int monthInt = atoi(month);
    if (monthInt < 1 || monthInt > 12) {
        return false;
    }

    // day
    char day[3]{};
    day[0] = date[0];
    day[1] = date[1];
    int dayInt = atoi(day);
    if (dayInt < 1 || dayInt > 31) {
        return false;
    }

    // year
    char year[5]{};
    year[0] = date[6];
    year[1] = date[7];
    year[2] = date[8];
    year[3] = date[9];
    int yearInt = atoi(year);
    if (yearInt < 1000 || yearInt > 9999) {
        return false;
    }

    return true;
}

void deleteSchedule() {
    clearScreen();
    if (!displayScheduleTable()) {
        printf("Press enter to continue...");
        getchar();
        getchar();
        manageSchedule();
        return;
    }

    char doneDate[16];
    char doneTime[16];

    do {
        printf("Input date (dd/mm/yyyy) >> ");
        scanf("%s", doneDate);
        getchar();
    } while (!isDateValid(doneDate));

    do {
        printf("Input time (hh:mm) >> ");
        scanf("%s", doneTime);
        getchar();
    } while (!isTimeValid(doneTime));

    // remove all before set date and time
    FlightSchedule *temp = headSchedule;
    FlightSchedule *prev = nullptr;
    while (temp != nullptr) {
        if (isDateBefore(temp->date, doneDate) || isDateBefore(doneDate, temp->date)) {
            if (prev == nullptr) {
                headSchedule = temp->next;
                free(temp);
                headSchedule;
            } else {
                prev->next = temp->next;
                free(temp);
                temp = prev->next;
            }
        } else if (isTimeBefore(temp->time, doneTime) || isTimeBefore(doneTime, temp->time)) {
            if (prev == nullptr) {
                headSchedule = temp->next;
                free(temp);
                temp = headSchedule;
            } else {
                prev->next = temp->next;
                free(temp);
                temp = prev->next;
            }
        } else {
            prev = temp;
            temp = temp->next;
        }
    }
}

bool isTimeBefore(char time[16], char time1[16]) {
    char hour[3]{};
    char minute[3]{};
    strncpy(hour, time, 2);
    strncpy(minute, time + 3, 2);

    char hour1[3]{};
    char minute1[3]{};
    strncpy(hour1, time1, 2);
    strncpy(minute1, time1 + 3, 2);

    int hourInt = atoi(hour);
    int minuteInt = atoi(minute);
    int hourInt1 = atoi(hour1);
    int minuteInt1 = atoi(minute1);

    if (hourInt < hourInt1) {
        return true;
    } else if (hourInt == hourInt1) {
        if (minuteInt < minuteInt1) {
            return true;
        }
    }

    return false;
}

bool isDateBefore(const char date[16], const char date1[16]) {
    // dd/mm/yyyy
    // dd
    char day[3]{};
    day[0] = date[0];
    day[1] = date[1];
    int dayInt = atoi(day);

    char day1[3]{};
    day1[0] = date1[0];
    day1[1] = date1[1];
    int day1Int = atoi(day1);

    if (dayInt < day1Int) {
        return true;
    }

    // mm
    char month[3]{};
    month[0] = date[3];
    month[1] = date[4];
    int monthInt = atoi(month);

    char month1[3]{};
    month1[0] = date1[3];
    month1[1] = date1[4];
    int month1Int = atoi(month1);

    if (monthInt < month1Int) {
        return true;
    }

    // yyyy
    char year[5]{};
    year[0] = date[6];
    year[1] = date[7];
    year[2] = date[8];
    year[3] = date[9];
    int yearInt = atoi(year);

    char year1[5]{};
    year1[0] = date1[6];
    year1[1] = date1[7];
    year1[2] = date1[8];
    year1[3] = date1[9];
    int year1Int = atoi(year1);

    if (yearInt < year1Int) {
        return true;
    }

    return false;
}

void pushSchedulePriority(FlightSchedule *scheduleData) {
    if (headSchedule == nullptr) {
        headSchedule = scheduleData;
        tailSchedule = scheduleData;
    } else {
        FlightSchedule *temp = headSchedule;
        FlightSchedule *prev = nullptr;

        // sort date then time
        while (temp != nullptr) {
            if (strcmp(scheduleData->date, temp->date) < 0) {
                break;
            } else if (strcmp(scheduleData->date, temp->date) == 0) {
                if (strcmp(scheduleData->time, temp->time) < 0) {
                    break;
                }
            }
            prev = temp;
            temp = temp->next;
        }

        if (prev == nullptr) {
            scheduleData->next = headSchedule;
            headSchedule = scheduleData;
        } else {
            scheduleData->next = temp;
            prev->next = scheduleData;
        }
    }
}

int main() {
    mainMenu();
    return 0;
}
