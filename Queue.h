#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>
#include <string>
#include <fstream>


template <class T> struct NodeQueue;
template <class T> class Queue;
template <class T> std::ostream& operator<< (std::ostream& os, Queue<T>& queue);

template <class T>
struct NodeQueue {
    T value;
    NodeQueue<T>* next;
    NodeQueue() {
        value = 0;
        next = nullptr;
    }
    NodeQueue(T v, NodeQueue<T>* nxt) : value(v), next(nxt) {}
};


template <class T>
class Queue {
private:
    int size;
    NodeQueue<T> *first;
    NodeQueue<T> *last;

public:
    Queue() {
        size = 0;
        first = nullptr;
        last = nullptr;
    }

    ~Queue() {
        clear();
    }

    void enqueue(T n) {
        if (first) {
            last->next = new NodeQueue<T>(n, nullptr);
            last = last->next;
        }
        else {
            first = new NodeQueue<T>(n, nullptr);
            last = first;
        }
        size++;
    }

    T dequeue() {
        if (!first) {
            std::cout << "ERROR: Queue empty" << std::endl;
            exit(1);
        }
        NodeQueue<T>* to_delete = first;
        first = first->next;
        if (!first)
            last = first;
        T value = to_delete->value;
        delete to_delete;
        size--;
        return value;
    }

    void clear() {
        NodeQueue<T>* to_delete;
        while (first) {
            to_delete = first;
            first =  first->next;
            delete to_delete;
        }
        size = 0;
        last = nullptr;
    }

    int getSize() {
        return size;
    }

    void toDot(std::string f) {
        std::fstream file;
        file.open(f, std::ios_base::out);
        if (file.is_open()) {
            file << "digraph Queue{" << std::endl;
            file << "\tbgcolor = transparent;" << std::endl;
            // nodes
            file << "\tfirst [shape=box; style=filled; fillcolor=red; color=red; fontcolor=white]" << std::endl;
            file << "\tlast [shape=box; style=filled; fillcolor=red; color=red; fontcolor=white]" << std::endl;
            NodeQueue<T> *ptr = first;
            for (int i = 1; i <= size; i++) {
                file << "\tnode" << i << " [label=\"" << ptr->value <<  "\"; style=filled; fillcolor=blue; color=blue; fontcolor=white]" << std::endl;
                ptr = ptr->next;
            }
            // vertex
            if (size > 0) {
                file << "\t{first; node1; rank=source}" << std::endl;
                file << "\tfirst -> node1;" << std::endl;
                for(int i = 1; i < size; i++)
                    file << "\tnode" << i << " -> node" << (i+1) << ";" << std::endl;
                file << "\t{last; node" << size << "; rank=same}" << std::endl;
                file << "\tlast -> node" << size << ";" << std::endl;
            }
            file << "}" << std::endl;
        }
    }

    friend std::ostream& operator<<<T> (std::ostream& os, Queue<T>& queue);
};

template <class T> std::ostream& operator<< (std::ostream& os, Queue<T>& queue) {
    NodeQueue<T>* aux = queue.first;
    os << "[";
    for (int i=1; i < queue.size; i++, aux = aux->next)
        os << aux->value << ", ";
    if (queue.size)
        os << aux->value;
    os << "]";
    return os;
}

#endif // QUEUE_H
