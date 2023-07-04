#include <condition_variable>
#include <iostream>
#include <mutex>
#include <stdlib.h>
#include <thread>
#include <vector>

using namespace std;

// Shared queue using template
template <typename T> class SharedQueue {
private:
  // data is stored using  linked list with Node haing string value and pointer
  // to the next node
  struct Node {
    T data;
    Node *next;
    Node(const T &value) : data(value), next(nullptr) {}
  };

  Node *head;
  Node *tail;

  // mutex for sync
  std::mutex mutex;
  std::condition_variable cv;

public:
  SharedQueue() : head(nullptr), tail(nullptr) {}

  ~SharedQueue() {
    while (head) {
      Node *node = head;
      head = head->next;
      delete node;
    }
  }

  // inserting the values in queue
  void enqueue(const T &item) {
    Node *newNode = new Node(item);

    {
      std::lock_guard<std::mutex> lock(mutex);

      if (!head) {
        head = newNode;
        tail = newNode;
      } else {
        tail->next = newNode;
        tail = newNode;
      }
    }

    cv.notify_one(); // Notify a waiting consumer
  }

  // getting the value from queue ating the node
  T dequeue() {
    std::unique_lock<std::mutex> lock(mutex);

    // Wait until the queue is not empty
    cv.wait(lock, [this] { return head != nullptr; });

    Node *node = head;
    head = head->next;
    T item = node->data;
    delete node;

    if (!head) {
      tail = nullptr;
    }

    return item;
  }

  bool empty() const {
    std::lock_guard<std::mutex> lock(mutex);
    return head == nullptr;
  }
};

int main() {
  SharedQueue<string> sharedQueue;

  // Producer thread
  std::thread producer([&]() {
    vector<string> vec = {"Sample1", "Sample2", "Sample3",
                          "Sample4", "Sample5", "stop"};
    for (auto d : vec) {
      sharedQueue.enqueue(d);
      std::cout << "Produced: " << d << std::endl;
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
  });

  // Consumer thread 1 2 3 4 5
  std::thread consumer1([&]() {
    while (true) {
      string item = sharedQueue.dequeue();
      std::cout << "Consumed 1: " << item << std::endl;

      if (item == "stop") {
        break; // Exit loop when the last item is consumed
      }
    }
  });

  std::thread consumer2([&]() {
    while (true) {
      string item = sharedQueue.dequeue();
      std::cout << "Consumed 2: " << item << std::endl;

      if (item == "stop") {
        break; // Exit loop when the last item is consumed
      }
    }
  });

  std::thread consumer3([&]() {
    while (true) {
      string item = sharedQueue.dequeue();
      std::cout << "Consumed 3: " << item << std::endl;

      if (item == "stop") {
        break; // Exit loop when the last item is consumed
      }
    }
  });

  std::thread consumer4([&]() {
    while (true) {
      string item = sharedQueue.dequeue();
      std::cout << "Consumed 4: " << item << std::endl;

      if (item == "stop") {
        break; // Exit loop when the last item is consumed
      }
    }
  });

  std::thread consumer5([&]() {
    while (true) {
      string item = sharedQueue.dequeue();
      std::cout << "Consumed 5: " << item << std::endl;

      if (item == "stop") {
        break; // Exit loop when the last item is consumed
      }
    }
  });

  consumer1.join();
  consumer2.join();
  consumer3.join();
  consumer4.join();
  consumer5.join();

  producer.join();

  return 0;
}
