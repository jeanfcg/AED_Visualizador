#include "Queue.h"

int main() {
    Queue<int> queue;
    queue.enqueue(4);
    std::cout << queue << std::endl;
    queue.enqueue(5);
    std::cout << queue << std::endl;
    queue.enqueue(3);
    std::cout << queue << std::endl;
    queue.enqueue(7);
    std::cout << queue << std::endl;
    queue.enqueue(8);
    std::cout << queue << std::endl;
    queue.enqueue(4);
    std::cout << queue << std::endl;
    queue.enqueue(89);
    std::cout << queue << std::endl;
    std::cout << queue.dequeue() << std::endl;
    std::cout << queue.dequeue() << std::endl;
    std::cout << queue << std::endl;
    queue.toDot("Queue.dot");
    return 0;
}
