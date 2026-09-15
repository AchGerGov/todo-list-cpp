#include "ConsoleUI.h"
#include <sstream>
#include <iostream>
#include <iomanip>

ConsoleUI::ConsoleUI(TodoList& list, std::istream& in, std::ostream& out)
    : list_(list), in_(in), out_(out) {}

void ConsoleUI::printTask(const Task& t) const {
    out_ << "  [" << (t.completed() ? 'x' : ' ') << "] #" << t.id()
         << " " << t.title()
         << "  <" << priorityToString(t.priority()) << ">";
    if (t.hasDeadline()) out_ << "  @due:" << t.deadline();
    if (!t.tags().empty()) {
        out_ << "  tags:";
        for (const auto& tag : t.tags()) out_ << " #" << tag;
    }
    out_ << '\n';
    if (!t.description().empty())
        out_ << "      " << t.description() << '\n';
}

void ConsoleUI::printTasks(const std::vector<Task>& tasks) const {
    if (tasks.empty()) { out_ << "  (пусто)\n"; return; }
    for (const auto& t : tasks) printTask(t);
}

void ConsoleUI::printHelp() const {
    out_ <<
        "Команды:\n"
        "  add <title> [| <desc>]              добавить задачу\n"
        "  list                                показать все\n"
        "  done <id>                           отметить выполненной\n"
        "  undone <id>                         снять отметку\n"
        "  rm <id>                             удалить\n"
        "  prio <id> <low|medium|high>         приоритет\n"
        "  due <id> <YYYY-MM-DD>               дедлайн\n"
        "  tag <id> <tag>                      добавить тег\n"
        "  untag <id> <tag>                    удалить тег\n"
        "  search <query>                      поиск по title/desc/tags\n"
        "  filter tag <tag>                    фильтр по тегу\n"
        "  filter prio <low|medium|high>       фильтр по приоритету\n"
        "  filter done|todo                    фильтр по статусу\n"
        "  save <path> / load <path>           сохранить / загрузить\n"
        "  help / exit\n";
}

bool ConsoleUI::processCommand(const std::string& line) {
    std::istringstream iss(line);
    std::string cmd;
    iss >> cmd;
    if (cmd.empty()) return true;

    if (cmd == "exit" || cmd == "quit") { running_ = false; return false; }
    if (cmd == "help") { printHelp(); return true; }
    if (cmd == "list") { printTasks(list_.tasks()); return true; }

    if (cmd == "add") {
        std::string rest;
        std::getline(iss, rest);
        while (!rest.empty() && rest.front() == ' ') rest.erase(rest.begin());
        auto pos = rest.find('|');
        std::string title = (pos == std::string::npos) ? rest : rest.substr(0, pos);
        std::string desc  = (pos == std::string::npos) ? "" : rest.substr(pos + 1);
        while (!title.empty() && title.back() == ' ') title.pop_back();
        while (!desc.empty() && desc.front() == ' ') desc.erase(desc.begin());
        if (title.empty()) { out_ << "  Ошибка: пустой заголовок\n"; return true; }
        int id = list_.addTask(title, desc);
        out_ << "  Добавлено #" << id << '\n';
        return true;
    }

    if (cmd == "rm" || cmd == "done" || cmd == "undone" ||
        cmd == "prio" || cmd == "due" || cmd == "tag" || cmd == "untag") {
        int id = 0;
        if (!(iss >> id)) { out_ << "  Ошибка: нужен id\n"; return true; }

        if (cmd == "rm") {
            out_ << (list_.removeTask(id) ? "  Удалено\n" : "  Не найдено\n");
        } else if (cmd == "done" || cmd == "undone") {
            bool ok = list_.markCompleted(id, cmd == "done");
            out_ << (ok ? "  Обновлено\n" : "  Не найдено\n");
        } else if (cmd == "prio") {
            std::string p; iss >> p;
            auto pr = priorityFromString(p);
            if (!pr) { out_ << "  Ошибка: приоритет low|medium|high\n"; return true; }
            out_ << (list_.setPriority(id, *pr) ? "  Обновлено\n" : "  Не найдено\n");
        } else if (cmd == "due") {
            std::string d; iss >> d;
            out_ << (list_.setDeadline(id, d) ? "  Обновлено\n" : "  Не найдено\n");
        } else if (cmd == "tag") {
            std::string t; iss >> t;
            out_ << (list_.addTag(id, t) ? "  Тег добавлен\n" : "  Не найдено\n");
        } else if (cmd == "untag") {
            std::string t; iss >> t;
            out_ << (list_.removeTag(id, t) ? "  Тег удалён\n" : "  Не найдено/нет тега\n");
        }
        return true;
    }

    if (cmd == "search") {
        std::string q;
        std::getline(iss, q);
        while (!q.empty() && q.front() == ' ') q.erase(q.begin());
        printTasks(list_.search(q));
        return true;
    }

    if (cmd == "filter") {
        std::string what; iss >> what;
        if (what == "tag") {
            std::string tag; iss >> tag;
            printTasks(list_.filterByTag(tag));
        } else if (what == "prio") {
            std::string p; iss >> p;
            auto pr = priorityFromString(p);
            if (!pr) { out_ << "  Ошибка\n"; return true; }
            printTasks(list_.filterByPriority(*pr));
        } else if (what == "done") {
            printTasks(list_.filterCompleted(true));
        } else if (what == "todo") {
            printTasks(list_.filterCompleted(false));
        } else {
            out_ << "  Неизвестный фильтр\n";
        }
        return true;
    }

    if (cmd == "save" || cmd == "load") {
        std::string path; iss >> path;
        if (path.empty()) { out_ << "  Нужен путь\n"; return true; }
        bool ok = (cmd == "save") ? list_.saveToFile(path) : list_.loadFromFile(path);
        out_ << (ok ? "  OK\n" : "  Ошибка\n");
        return true;
    }

    out_ << "  Неизвестная команда. help — справка.\n";
    return true;
}

void ConsoleUI::run() {
    out_ << "todo-list-cpp v2.0  (help — справка, exit — выход)\n";
    std::string line;
    while (running_) {
        out_ << "> " << std::flush;
        if (!std::getline(in_, line)) break;
        processCommand(line);
    }
}
