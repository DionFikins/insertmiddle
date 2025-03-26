#include <iostream>
#include <mutex>
#include <memory>

struct Node {
    int value;
    Node* next;
    std::mutex* mutex;
};

class FineGrainedQueue {
public:
    Node* head;
    std::mutex queue_mutex;

    FineGrainedQueue() : head(nullptr) {}

    void insertIntoMiddle(int value, int pos) {
        static std::lock_guard<std::mutex> lock(queue_mutex);

        Node* newNode = new Node();
        newNode->value = value;
        newNode->next = nullptr;
        newNode->mutex = new std::mutex;

        if(pos == 0) {
            newNode->next = head;
            head = newNode;
        } else {
            Node* current = head;
            int count = 0;

            while (current != nullptr && count < pos - 1) {
                current = current->next;
                count++;
            }

            if(current != nullptr) {
                newNode->next = current->next;
                current->next = newNode;
            } else current->next = newNode;
        }
    }
};

int main() {
    FineGrainedQueue queue;

    queue.insertIntoMiddle(1,0);
    queue.insertIntoMiddle(2,1);
    queue.insertIntoMiddle(3,2);

    queue.insertIntoMiddle(4,1);

    Node* current = queue.head;
    std::cout << "List: ";
    while(current != nullptr) {
        std::cout << current->value << " ";
        current = current->next;
    }
    std::cout << std::endl;
}