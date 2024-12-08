#include "HashTable.h"

using namespace std;

// Конструктор хеш-таблицы
HashTable::HashTable() {
    // Инициализируем каждый элемент массива таблицы указателем NULL
    for (int i = 0; i < TABLE_SIZE; ++i) {
        table[i] = nullptr;
    }
}

// Деструктор хеш-таблицы
HashTable::~HashTable() {
    // Освобождаем память, выделенную для каждого узла в таблице
    for (int i = 0; i < TABLE_SIZE; ++i) { 
        NodeH* current = table[i]; // Указатель на текущий узел
        while (current) {
            NodeH* temp = current; // Временный указатель на удаляемый узел
            current = current->next; // Переходим на следующий узел
            delete temp; // Удаляем текущий узел
        }
    }
}

// Хеш-функция для получения индекса в таблице по ключу
size_t HashTable::hash(const string &key) const {
    size_t hashValue = 0; // Начальное значение хеша
    for (char ch : key) {
        // Применяем простой алгоритм хеширования
        hashValue = (hashValue * 31 + ch) % TABLE_SIZE;
    }
    return hashValue; // Возвращаем рассчитанный индекс
}

// Функция для добавления или обновления элемента в хеш-таблице
void HashTable::Hinsert(const string &key, const string &value) {
    size_t index = hash(key); // Вычисляем индекс для данного ключа
    NodeH* newNode = new NodeH(key, value); // Создаем новый узел

    // Если в таблице нет узла по этому индексу, добавляем новый узел
    if (!table[index]) {
        table[index] = newNode;
    } else {
        NodeH* current = table[index]; // Начинаем с первого узла в цепочке
        while (current->next) {
            // Проверяем, существует ли уже ключ
            if (current->key == key) {
                current->value = value; // Обновляем значение
                delete newNode; // Избегаем утечки памяти
                return; // Завершаем выполнение функции
            }
            current = current->next; // Переходим к следующему узлу
        }
        // Проверяем последний узел (если у нас несколько узлов в цепочке)
        if (current->key == key) {
            current->value = value; // Обновляем значение
            delete newNode;         // Избегаем утечки памяти
        } else {
            current->next = newNode;  // Добавляем новый узел в конец цепочки
        }
    }
}

// Функция для получения значения по ключу
bool HashTable::Hget(const string &key, string &value) {
    size_t index = hash(key); // Вычисляем индекс для данного ключа
    NodeH* current = table[index]; // Начинаем с первого узла в цепочке
    while (current) {
        // Проверяем, совпадает ли ключ
        if (current->key == key) {
            value = current->value; // Возвращаем значение
            return true; // Успешно найдено
        }
        current = current->next; // Переходим к следующему узлу
    }
    return false; // Ключ не найден
}

// Функция для удаления элемента по ключу
bool HashTable::Hdel(const string &key) {
    size_t index = hash(key); // Вычисляем индекс для данного ключа
    NodeH* current = table[index]; // Начинаем с первого узла
    NodeH* previous = nullptr; // Предыдущий узел для отслеживания ссылок

    while (current) {
        // Проверяем, совпадает ли ключ
        if (current->key == key) {
            if (previous) {
                previous->next = current->next; // Удаляем узел, если он не первый
            } else {
                table[index] = current->next; // Удаляем первый узел (голову)
            }
            delete current; // Освобождаем память
            return true; // Успешно удалено
        }
        previous = current; // Обновляем предыдущий узел
        current = current->next; // Переходим к следующему узлу
    }
    return false; // Ключ не найден
}

// Функция для отображения всех элементов таблицы
void HashTable::Hprint() const {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        cout << i << ": "; // Печатаем индекс
        NodeH* current = table[i]; // Указатель на текущий узел
        while (current) {
            cout << "{" << current->key << ": " << current->value << "} "; // Печатаем ключ-значение
            current = current->next; // Переходим к следующему узлу
        }
        cout << endl; // Переходим на новую строку
    }
}

// Функция для считывания хеш-таблицы из файла
void HashTable::HreadFromFile(const string &filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Не удалось открыть файл для чтения.");
    }

    string key, value;
    while (file >> key >> value) {
        Hinsert(key, value); // Вставляем каждую пару (ключ, значение) в хеш-таблицу
    }

    file.close(); // Закрываем файл
}

// Функция для записи хеш-таблицы в файл
void HashTable::HwriteToFile(const string &filename) const {
    ofstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Не удалось открыть файл для записи.");
    }

    for (int i = 0; i < TABLE_SIZE; ++i) {
        NodeH* current = table[i];
        while (current) {
            file << current->key << " " << current->value << endl; // Записываем пары ключ-значение
            current = current->next; // Переходим к следующему узлу
        }
    }

    file.close(); // Закрываем файл
}