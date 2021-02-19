#include <iostream>
#include "BST.h"

int main() {
    BST<int, std::string> tree;
    tree.insert(15, "dog");
    tree.insert(6, "krisa");
    tree.insert(17, "home");
    tree.insert(16, "home");
    tree.insert(10, "krisa");

    tree.print();

    std::cout << std::endl;


    int lastkey = 15;
    while (true) {
        if(auto prev_key = tree.findPrev(lastkey); prev_key != std::nullopt) {
            lastkey = *(prev_key);
        } else {
            break;
        }
    }

    while (true) {
        std::cout << lastkey << " ";
        if(auto next_key = tree.findNext(lastkey); next_key != std::nullopt) {
            lastkey = *(next_key);
        } else {
            break;
        }
    }

    return 0;
}
