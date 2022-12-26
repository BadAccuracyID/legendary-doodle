#include <cstdio>
#include <cstdlib>

struct Data {
    int number;

    Data *next;
};

Data *head = nullptr;
Data *tail = nullptr;

Data *createData(int number) {
    Data *data = (Data *) malloc(sizeof(Data));
    data->number = number;
    data->next = nullptr;
    return data;
}

void pushHead(int number) {
    Data *data = createData(number);
    if (head == nullptr) {
        head = data;
        tail = data;
    } else {
        data->next = head;
        head = data;
    }
}

void pushTail(int number) {
    Data *data = createData(number);
    if (head == nullptr) {
        head = data;
        tail = data;
    } else {
        tail->next = data;
        tail = data;
    }
}

void pushPriority(int number) {
    Data *data = createData(number);
    if (head == nullptr) {
        head = data;
        tail = data;
    } else {
        if (number < head->number) {
            pushHead(number);
        } else if (number > tail->number) {
            pushTail(number);
        } else {
            Data *temp = head;
            while (temp->next != nullptr && temp->next->number < number) {
                temp = temp->next;
            }
            data->next = temp->next;
            temp->next = data;
        }
    }
}

void printData() {
    Data *temp = head;
    while (temp != nullptr) {
        printf("%d ", temp->number);
        temp = temp->next;
    }
    printf("\n");
}

void popHead() {
    if (head == nullptr) {
        return;
    }

    if (head == tail) {
        free(head);
        head = tail = nullptr;
    } else {
        Data *temp = head;
        head = head->next;
        free(temp);
    }
}

void popTail() {
    if (head == nullptr) {
        return;
    }

    if (head == tail) {
        free(head);
        head = tail = nullptr;
    } else {
        Data *temp = head;
        while (temp->next != tail) {
            temp = temp->next;
        }
        free(tail);
        tail = temp;
        tail->next = nullptr;
    }
}

int main() {

//    pushTail(10);
//    pushTail(20);
//    pushTail(30);

    pushPriority(10);
    pushPriority(30);
    pushPriority(70);
    pushPriority(20);
    pushPriority(5);
    pushPriority(50);

    printData();
    popHead();
    printData();
    popTail();
    printData();

    return 0;
}