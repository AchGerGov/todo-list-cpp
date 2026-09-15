#pragma once

#include "TodoList.h"
#include <istream>
#include <ostream>
#include <string>

class ConsoleUI {
public:
    ConsoleUI(TodoList& list, std::istream& in, std::ostream& out);

    void run();
    // Возвращает false для выхода
    bool processCommand(const std::string& line);

private:
    void printHelp() const;
    void printTasks(const std::vector<Task>& tasks) const;
    void printTask(const Task& t) const;

    TodoList& list_;
    std::istream& in_;
    std::ostream& out_;
    bool running_ = true;
};
