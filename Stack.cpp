#include "Stack.h"

int main() {
    Stack<int> pila;
    pila.push(4);
    pila.push(5);
    pila.push(3);
    pila.push(7);
    pila.push(8);
    pila.push(4);
    pila.push(89);
    pila.pop();
    pila.toDot("Stack.dot");
    return 0;
}
