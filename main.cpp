#include <vector>
#include <cstddef>
#include <stdexcept>
#include <thread>
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

    ~FineGrainedQueue() {
        Node* current = head;
        while(current) {
            Node* next = current->next;
            delete current;
            current = next;
        }
    }

    void insertIntoMiddle(int value, int pos) {
        static std::lock_guard<std::mutex> lock(queue_mutex);

        Node* newNode = new Node();
        newNode->value = value;
        newNode->next = nullptr;
        newNode->mutex = new std::mutex;

        if(pos == 0) {
            newNode->next = head;
            head = newNode;
            return;
        }
        
        Node* current = head;
        int count = 1;

        while(current && count < pos) {
            current = current->next;
            count++;
        }

        if(!current) {
            if(!current) {
                newNode->next = nullptr;
                head = newNode;
                return;
            }

            while(current->next)
                current = current->next;

            current->next = newNode;
            newNode->next = nullptr;
            return;
        }
        newNode->next = current->next;
        current->next = newNode;
    }
};

int main()
{
    FineGrainedQueue queue;
    const int num_threads = 5;
    std::vector<std::thread> threads;

    for (size_t i = 0; i < num_threads; ++i)
        threads.emplace_back([&queue, i]() {
            queue.insertIntoMiddle((i + 1) * 100, 5); 
        });

    for (auto& t : threads)
        if (t.joinable())
            t.join();

    std::cout << "List: ";
    Node* current = queue.head;
    while(current) {
        std::cout << current->value << " ";
        current = current->next;
    }
    std::cout << std::endl;
    
    return 0;
}
