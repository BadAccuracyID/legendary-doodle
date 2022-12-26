#include <cstdio>
#include <cstdlib>
#include <cstring>

struct PlayerData {
    char name[32];
    int kills, deaths;

    int getKDRatio() const {
        return kills / deaths;
    }

    PlayerData *next;
};

PlayerData *head = nullptr;
PlayerData *tail = nullptr;

PlayerData *createPlayerData(const char name[32], int kills, int deaths) {
    auto *playerData = (PlayerData *) malloc(sizeof(PlayerData));
    strcpy(playerData->name, name);
    playerData->kills = kills;
    playerData->deaths = deaths;
    playerData->next = nullptr;
    return playerData;
}

void pushHead(PlayerData *playerData) {
    if (head == nullptr) {
        head = playerData;
        tail = playerData;
    } else {
        playerData->next = head;
        head = playerData;
    }
}

void pushTail(PlayerData *playerData) {
    if (head == nullptr) {
        head = playerData;
        tail = playerData;
    } else {
        tail->next = playerData;
        tail = playerData;
    }
}

void pushPriority(PlayerData *playerData) {
    if (head == nullptr) {
        head = playerData;
        tail = playerData;
    } else {
        // sort name
        if (strcmp(playerData->name, head->name) < 0) {
            pushHead(playerData);
        } else if (strcmp(playerData->name, tail->name) > 0) {
            pushTail(playerData);
        } else {
            auto *temp = head;
            while (temp->next != nullptr && strcmp(temp->next->name, playerData->name) < 0) {
                temp = temp->next;
            }
            playerData->next = temp->next;
            temp->next = playerData;
        }
    }
}

void printList() {
    auto *temp = head;
    printf("==========================================\n");
    printf("| %-20s | %-6s | %-6s |\n", "Username", "Kills", "Deaths");
    printf("==========================================\n");
    while (temp != nullptr) {
        printf("| %-20s | %-6d | %-6d |\n", temp->name, temp->kills, temp->deaths);
        temp = temp->next;
    }
    printf("==========================================\n");
}

PlayerData *search(const char name[32]) {
    auto *temp = head;
    while (temp != nullptr) {
        if (strcmp(temp->name, name) == 0) {
            return temp;
        }
        temp = temp->next;
    }
    return nullptr;
}

int main() {
    pushPriority(createPlayerData("BadAccuracyID", 1, 1));
    pushPriority(createPlayerData("LyraMS", 100, 3));
    pushPriority(createPlayerData("FacedApollo", 15, 100));
    pushPriority(createPlayerData("Alexand3r", 150, 7));

    printList();
    return 0;
}