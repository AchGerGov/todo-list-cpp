#include "TodoList.h"
#include "ConsoleUI.h"
#include <iostream>
#include <string>

int main(int argc, char** argv) {
    TodoList list;

    // Необязательно: загружаем файл, переданный аргументом
    std::string storage;
    if (argc > 1) {
        storage = argv[1];
        if (list.loadFromFile(storage)) {
            std::cout << "Загружено задач: " << list.size() << "\n";
        }
    }

    ConsoleUI ui(list, std::cin, std::cout);
    ui.run();

    if (!storage.empty()) {
        if (list.saveToFile(storage))
            std::cout << "Сохранено в " << storage << "\n";
    }
    return 0;
}
