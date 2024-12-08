#ifndef COMPLETE_BINARY_TREE_H
#define COMPLETE_BINARY_TREE_H

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>

using namespace std;

// Реализация очереди на основе связного списка
template<typename T>
class Queue2 {
private:
    struct Node {
        T data;  // Данные узла
        Node* next;  // Указатель на следующий узел
        Node(const T& value) : data(value), next(nullptr) {}  // Конструктор узла
    };

    Node* front_node;  // Указатель на первый узел очереди
    Node* back_node;  // Указатель на последний узел очереди
    size_t size;  // Размер очереди

public:
    Queue2() : front_node(nullptr), back_node(nullptr), size(0) {}  // Конструктор очереди

    ~Queue2() {
        while (!is_empty()) {  // Пока очередь не пуста
            pop();  // Удаляем первый элемент
        }
    }

    // Добавление элемента в конец очереди
    void push(const T& value) {
        Node* new_node = new Node(value);  // Создаем новый узел
        if (!back_node) {  // Если очередь пуста
            front_node = back_node = new_node;  // Новый узел становится первым и последним
        } else {
            back_node->next = new_node;  // Привязываем новый узел к концу очереди
            back_node = new_node;  // Обновляем указатель на последний узел
        }
        size++;  // Увеличиваем размер очереди
    }

    // Удаление элемента из начала очереди
    void pop() {
        if (is_empty()) throw out_of_range("Queue is empty");  // Если очередь пуста, бросаем исключение
        Node* temp = front_node;  // Сохраняем указатель на первый узел
        front_node = front_node->next;  // Перемещаем указатель на следующий узел
        if (!front_node) back_node = nullptr;  // Если очередь пуста, обнуляем указатель на последний узел
        delete temp;  // Удаляем старый первый узел
        size--;  // Уменьшаем размер очереди
    }

    // Получение элемента из начала очереди
    T& front() {
        if (is_empty()) throw out_of_range("Queue is empty");  // Если очередь пуста, бросаем исключение
        return front_node->data;  // Возвращаем данные первого узла
    }

    const T& front() const {
        if (is_empty()) throw out_of_range("Queue is empty");  // Если очередь пуста, бросаем исключение
        return front_node->data;  // Возвращаем данные первого узла
    }

    // Проверка, пуста ли очередь
    bool is_empty() const {
        return size == 0;  // Возвращаем true, если размер очереди равен 0
    }

    // Получение размера очереди
    size_t get_size() const {
        return size;  // Возвращаем размер очереди
    }
};

// Реализация полного двоичного дерева
template<typename T>
class CompleteBinaryTree {
private:
    struct Node {
        T data;  // Данные узла
        Node* left;  // Указатель на левого потомка
        Node* right;  // Указатель на правого потомка
        Node(const T& value) : data(value), left(nullptr), right(nullptr) {}  // Конструктор узла
    };

    Node* root;  // Указатель на корень дерева
    size_t size;  // Размер дерева

    // Вспомогательная функция для поиска первого пустого места для добавления нового узла
    void add_node(Node* new_node) {
        Queue2<Node*> queue2;  // Создаем очередь для обхода дерева
        queue2.push(root);  // Добавляем корень в очередь

        while (!queue2.is_empty()) {  // Пока очередь не пуста
            Node* current = queue2.front();  // Получаем текущий узел из очереди
            queue2.pop();  // Удаляем текущий узел из очереди

            // Если есть место для левого ребенка, добавляем новый узел
            if (!current->left) {
                current->left = new_node;  // Новый узел становится левым потомком
                return;  // Выходим из функции
            } else {
                queue2.push(current->left);  // Добавляем левого потомка в очередь
            }

            // Если есть место для правого ребенка, добавляем новый узел
            if (!current->right) {
                current->right = new_node;  // Новый узел становится правым потомком
                return;  // Выходим из функции
            } else {
                queue2.push(current->right);  // Добавляем правого потомка в очередь
            }
        }
    }

    // Вспомогательная функция для проверки, является ли дерево полным
    bool is_complete_queue() const {
        if (!root) return false;  // Пустое дерево считается полным

        Queue2<Node*> queue2;  // Создаем очередь для обхода дерева
        queue2.push(root);  // Добавляем корень в очередь
        bool must_be_leaf = false;  // Этот флаг указывает, что все последующие узлы должны быть листьями

        while (!queue2.is_empty()) {  // Пока очередь не пуста
            Node* current = queue2.front();  // Получаем текущий узел из очереди
            queue2.pop();  // Удаляем текущий узел из очереди

            // Если текущий узел - "пустой" узел (пустая строка), устанавливаем флаг must_be_leaf и проверяем далее
            if (current->data.empty()) {
                must_be_leaf = true;  // Устанавливаем флаг
                continue;  // Пропускаем этот узел, так как это пустое место
            }

            // Проверяем левый потомок
            if (current->left) {
                if (must_be_leaf) return false;  // Если левый ребенок найден после установки флага, дерево не полное
                queue2.push(current->left);  // Добавляем левого потомка в очередь
            } else {
                must_be_leaf = true;  // Если нет левого ребенка, все последующие узлы должны быть листьями
            }

            // Проверяем правый потомок
            if (current->right) {
                if (must_be_leaf) return false;  // Если правый ребенок найден после установки флага, дерево не полное
                queue2.push(current->right);  // Добавляем правого потомка в очередь
            } else {
                must_be_leaf = true;  // Если нет правого ребенка, все последующие узлы должны быть листьями
            }
        }

        return true;  // Если мы прошли через все узлы и не нарушили условие, дерево полное
    }

    // Вспомогательная функция для поиска элемента
    Node* find_node(Node* node, const T& value) const {
        if (!node) return nullptr;  // Если узел пустой, возвращаем nullptr

        if (node->data == value) return node;  // Если данные узла совпадают с искомым значением, возвращаем узел

        Node* left_result = find_node(node->left, value);  // Рекурсивно ищем в левом поддереве
        if (left_result) return left_result;  // Если нашли в левом поддереве, возвращаем результат

        return find_node(node->right, value);  // Рекурсивно ищем в правом поддереве
    }

    // Вспомогательная функция для удаления дерева
    void clear(Node* node) {
        if (!node) return;  // Если узел пустой, выходим из функции
        clear(node->left);  // Рекурсивно удаляем левое поддерево
        clear(node->right);  // Рекурсивно удаляем правое поддерево
        delete node;  // Удаляем текущий узел
    }

    // Вспомогательная функция для загрузки данных в дерево
    void load_from_queue(Queue2<T>& data_queue) {
        while (!data_queue.is_empty()) {  // Пока очередь не пуста
            add(data_queue.front());  // Добавляем элемент из очереди в дерево
            data_queue.pop();  // Удаляем элемент из очереди
        }
    }

public:
    CompleteBinaryTree() : root(nullptr), size(0) {}  // Конструктор дерева

    ~CompleteBinaryTree() {
        clear(root);  // Удаляем все узлы дерева
    }

    // Добавление элемента
    void add(const T& value) {
        Node* new_node = new Node(value);  // Создаем новый узел
        if (!root) {  // Если дерево пустое
            root = new_node;  // Новый узел становится корнем
        } else {
            add_node(new_node);  // Иначе добавляем узел в дерево
        }

        size++;  // Увеличиваем размер дерева
    }

    // Удаление элемента
    void remove(const T& value) {
        if (!root) {  // Если дерево пустое
            throw runtime_error("Tree is empty");  // Бросаем исключение
        }

        // Если дерево состоит из одного узла
        if (size == 1 && root->data == value) {
            delete root;  // Удаляем корень
            root = nullptr;  // Обнуляем указатель на корень
            size--;  // Уменьшаем размер дерева
            return;  // Выходим из функции
        }

        Queue2<Node*> queue;  // Создаем очередь для обхода дерева
        queue.push(root);  // Добавляем корень в очередь

        Node* node_to_delete = nullptr;  // Указатель на узел, который нужно удалить
        Node* last_node = nullptr;  // Указатель на последний узел в дереве
        Node* last_node_parent = nullptr;  // Указатель на родителя последнего узла

        // Поиск узла для удаления и последнего узла
        while (!queue.is_empty()) {  // Пока очередь не пуста
            Node* current = queue.front();  // Получаем текущий узел из очереди
            queue.pop();  // Удаляем текущий узел из очереди

            if (current->data == value) {  // Если данные узла совпадают с искомым значением
                node_to_delete = current;  // Найден узел для удаления
            }

            if (current->left) {  // Если есть левый потомок
                queue.push(current->left);  // Добавляем левого потомка в очередь
                last_node_parent = current;  // Обновляем указатель на родителя последнего узла
                last_node = current->left;  // Обновляем указатель на последний узел
            }

            if (current->right) {  // Если есть правый потомок
                queue.push(current->right);  // Добавляем правого потомка в очередь
                last_node_parent = current;  // Обновляем указатель на родителя последнего узла
                last_node = current->right;  // Обновляем указатель на последний узел
            }
        }

        // Если узел для удаления не найден
        if (!node_to_delete) {
            throw runtime_error("Element not found");  // Бросаем исключение
        }

        // Если найден последний узел и узел для удаления
        if (last_node) {
            node_to_delete->data = last_node->data;  // Замена данных
            // Удаляем последний узел
            if (last_node_parent->right == last_node) {  // Если последний узел - правый потомок
                delete last_node_parent->right;  // Удаляем правый потомок
                last_node_parent->right = nullptr;  // Обнуляем указатель на правый потомок
            } else {  // Если последний узел - левый потомок
                delete last_node_parent->left;  // Удаляем левый потомок
                last_node_parent->left = nullptr;  // Обнуляем указатель на левый потомок
            }
        }

        size--;  // Уменьшаем размер дерева
    }

    // Поиск элемента
    bool find(const T& value) const {
        return find_node(root, value) != nullptr;  // Возвращаем true, если узел найден
    }

    // Проверка, является ли дерево полным
    bool is_complete_tree() const {
        return is_complete_queue();  // Возвращаем результат проверки
    }

    // Печать дерева (обход в ширину)
    void print() const {
        if (!root) return;  // Если дерево пустое, выходим из функции

        Queue2<Node*> queue2;  // Создаем очередь для обхода дерева
        queue2.push(root);  // Добавляем корень в очередь

        while (!queue2.is_empty()) {  // Пока очередь не пуста
            Node* current = queue2.front();  // Получаем текущий узел из очереди
            queue2.pop();  // Удаляем текущий узел из очереди
            cout << current->data << " ";  // Выводим данные узла

            if (current->left) queue2.push(current->left);  // Добавляем левого потомка в очередь
            if (current->right) queue2.push(current->right);  // Добавляем правого потомка в очередь
        }
        cout << endl;  // Переходим на новую строку
    }

    // Получение размера дерева
    size_t get_size() const {
        return size;  // Возвращаем размер дерева
    }

    // Загрузка данных из файла
    void load_from_file(const string& filename) {
        ifstream file(filename);  // Открываем файл для чтения
        if (file.is_open()) {  // Если файл открыт
            Queue2<T> data_queue;  // Создаем очередь для хранения данных
            T value;
            while (file >> value) {  // Читаем данные из файла
                data_queue.push(value);  // Добавляем данные в очередь
            }
            file.close();  // Закрываем файл
            load_from_queue(data_queue);  // Загружаем элементы в дерево
        } else {
            throw runtime_error("Unable to open file for loading");  // Бросаем исключение, если файл не открыт
        }
    }

    // Сохранение дерева в файл (обход в ширину)
    void save_to_file(const string& filename) const {
        ofstream file(filename);  // Открываем файл для записи
        if (file.is_open()) {  // Если файл открыт
            Queue2<Node*> queue;  // Создаем очередь для обхода дерева
            queue.push(root);  // Добавляем корень в очередь

            while (!queue.is_empty()) {  // Пока очередь не пуста
                Node* current = queue.front();  // Получаем текущий узел из очереди
                queue.pop();  // Удаляем текущий узел из очереди
                file << current->data << " ";  // Записываем данные узла в файл

                if (current->left) queue.push(current->left);  // Добавляем левого потомка в очередь
                if (current->right) queue.push(current->right);  // Добавляем правого потомка в очередь
            }
            file.close();  // Закрываем файл
        } else {
            throw runtime_error("Unable to open file for saving");  // Бросаем исключение, если файл не открыт
        }
    }
};

#endif