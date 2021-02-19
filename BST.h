//
// Created by magte on 12.02.2021.
//

#ifndef BST_BST_H
#define BST_BST_H

#include <optional>
#include <stack>

template<typename K, typename D>
class BST {
    struct node {
        K key;
        D data;
        node *left, *right;
    } *root; // Самая верхняя ячейка в дереве
    int count; // Количество

    void _clear(node *sub_root); // Удалить поддерево

    void print_node(node *current);

public:
    BST();

    ~BST();

    std::optional<D> find(const K &key); // Находит ячейку по ключу и возвращает содержимое

    bool insert(const K &key,
                const D &data); // Вставляет новую ячейку, если ключ ранее не использовался, иначе заменяет содержимое

    bool remove(const K &key); // Удаляет ячейку по ключу

    int size(); // return count; Возвращает количество ячеек

    void clear(); // Удаляет все дерево

    std::optional<K> findNext(const K &key);

    std::optional<K> findPrev(const K &key);

    void print();

};

template<typename K, typename D>
BST<K, D>::BST() {
    root = nullptr;
    count = 0;
}

// не рекурс.
template<typename K, typename D>
BST<K, D>::~BST() {
    // Кладем в стек корень
    // цикл начинается:
    //    Берем с вершины стека ячейку
    //    Убираем из стека
    //    Если есть дети, кладем их в стек
    //    Удаляем вершину
    std::stack<node *> stack;
    stack.push(root);
    while (!stack.empty()) {
        node *current = stack.top();
        stack.pop();
        if (current->left)
            stack.push(current->left);
        if (current->right)
            stack.push(current->right);
        delete current;
    }
    count = 0;
}

template<typename K, typename D>
void BST<K, D>::clear() {
    _clear(root);
}

template<typename K, typename D>
void BST<K, D>::_clear(BST::node *sub_root) {
    if (sub_root == nullptr) return;
    _clear(sub_root->left);
    _clear(sub_root->right);
    delete sub_root;
}

template<typename K, typename D>
int BST<K, D>::size() {
    return count;
}

template<typename K, typename D>
std::optional<D> BST<K, D>::find(const K &key) {
    node *current = root;
    while (current != nullptr && current->key != key) {
        current = current->key < key ? current->right : current->left;
    }
    if (current == nullptr) {
        return std::nullopt;
    } else {
        return current->data;
    }
}

template<typename K, typename D>
bool BST<K, D>::insert(const K &key, const D &data) {
    node *parent_current = nullptr; // Чтобы добавить новую ячейку, мы должны сказать родителю ссылаться на нее
    node *current = root;
    while (current != nullptr && current->key != key) {
        parent_current = current;
        current = current->key < key ? current->right : current->left;
    }

    if (parent_current == nullptr) {
        root = new node{key, data, nullptr, nullptr}; // Если в дереве ничего не было, то добавляемая ячейка - корень
        count++;
        return true;
    }

    if (current == nullptr) {
        if (parent_current->key < key) {
            parent_current->right = new node{key, data, nullptr, nullptr};
        } else {
            parent_current->left = new node{key, data, nullptr, nullptr};
        }
        count++;
        return true;
    }

    current->data = data;
    return false;
}

template<typename K, typename D>
bool BST<K, D>::remove(const K &key) {
    node *parent_current = nullptr;
    node *current = root;
    while (current != nullptr && current->key != key) {
        parent_current = current;
        current = current->key < key ? current->right : current->left;
    }
    if (current == nullptr)
        return false;

    if (current->left == nullptr && current->right == nullptr) {
        if (parent_current != nullptr) {
            if (parent_current->key < key) {
                parent_current->right = nullptr;
            } else {
                parent_current->left = nullptr;
            }
            delete current;
        } else {
            delete current;
            root = nullptr;
        }
    } else if (current->left == nullptr) {
        if (parent_current != nullptr) {
            if (parent_current->key < key) {
                parent_current->right = current->right;
            } else {
                parent_current->left = current->right;
            }
            delete current;
        } else {
            root = current->right;
            delete current;
        }
    } else if (current->right == nullptr) {
        if (parent_current != nullptr) {
            if (parent_current->key < key) {
                parent_current->right = current->left;
            } else {
                parent_current->left = current->left;
            }
            delete current;
        } else {
            root = current->left;
            delete current;
        }
    } else {
        //get local max
        node *child = current->left;
        node *parent_child = current;
        while (child->right != nullptr) {
            parent_child = child;
            child = child->right;
        }
        parent_child->right = child->left;
        current->key = child->key;
        current->data = current->data;
        delete child;
    }
    count--;
    return true;
}

template<typename K, typename D>
void BST<K, D>::print() {
    print_node(root);
}

template<typename K, typename D>
void BST<K, D>::print_node(BST::node *current) {
    if (current == nullptr) return;
    std::cout << current->key << " ";
    print_node(current->left);
    print_node(current->right);
}

template<typename K, typename D>
std::optional<K> BST<K, D>::findNext(const K &key) {
    node *current = root;
    node *save = nullptr;

    while (current->key != key) {
        if (key > current->key) {
            current = current->right;
        } else {
            save = current;
            current = current->left;
        }
    }

    if (current->right != nullptr) {
        // находим минимум
        // - finNext/Prev - пзволит сделать наше дерево контейнером и позволит иттерировать, алгоритм:
        // findNext - находим ключ key как при поиске find, запоминая последний совершенный переход на лево,
        // если у ключа key есть правое поддерево, то следующий по значению - это минимум правого поддерева,
        // иначе это заомненный переход налево.
        node *min = current->right;
        while (min->left != nullptr)
            min = min->left;
        return min->key;
    }

    if (save == nullptr)
        return std::nullopt;

    return save->key;
}

template<typename K, typename D>
std::optional<K> BST<K, D>::findPrev(const K &key) {
    // максимум у левого поддерева
    // Если поддерева нет, то последний поворот НАПРАВО
    node *current = root;
    node *save = nullptr;

    while (current->key != key) {
        if (key > current->key) {
            save = current;
            current = current->right;
        } else {
            current = current->left;
        }
    }

    if (current->left != nullptr) {
        // Находим максимум
        node *max = current->left;
        while (max->right != nullptr)
            max = max->right;
        return max->key;
    }

    if (save == nullptr)
        return std::nullopt;

    return save->key;
}


#endif //BST_BST_H
