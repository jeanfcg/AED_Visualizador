#ifndef STACK_H
#define STACK_H

#include <iostream>
#include <string>
#include <fstream>

template <class T>
struct NodeStack {
    T value;
    NodeStack<T>* next;
    NodeStack() {
        value = 0;
        next = nullptr;
    }
    NodeStack(T v, NodeStack<T>* nxt) : value(v), next(nxt) {}
};


template <class T>
class Stack {
private:
    int size;
    NodeStack<T> *top;

public:
    Stack()
    {
        size = 0;
        top = nullptr;
    }

    ~Stack()
    {
        clear();
    }

    void push(T n) {
        if (top) {
            NodeStack<T>* temp = new NodeStack<T>(n, top);
            top = temp;
        }
        else {
            top = new NodeStack<T>(n, nullptr);
        }
        size++;
    }

    T pop() {
        NodeStack<T>* temp = top->next;
        T value = top->value;
        delete top;
        top = temp;
        --size;
        return value;
    }

    void clear() {
        NodeStack<T> *temp;
        while (top) {
            temp = top;
            top = top->next;
            delete temp;
        }
        size = 0;
    }

    int getSize() {
        return size;
    }

    void toDot(std::string f) {
        std::fstream file;
        file.open(f, std::ios_base::out);
        if (file.is_open()) {
            file << "digraph Stack{" << std::endl;
            file << "\tbgcolor = transparent;" << std::endl;
            // nodes
            file << "\ttop [shape=box; style=filled; fillcolor=red; color=red; fontcolor=white]" << std::endl;
            NodeStack<T> *ptr = top;
            for (int i = 1; i <= size; i++) {
                file << "\tnode" << i << " [label=\"" << ptr->value <<  "\"; style=filled; fillcolor=blue; color=blue; fontcolor=white]" << std::endl;
                ptr = ptr->next;
            }
            // vertex
            if (size > 0){
                file << "\t{top; node1; rank=source}" << std::endl;
                file << "\ttop -> node1;" << std::endl;
                for(int i = 1; i < size; i++)
                    file << "\tnode" << i << " -> node" << (i+1) << ";" << std::endl;
            }
            file << "}" << std::endl;
        }
    }
};

#endif // STACK_H
