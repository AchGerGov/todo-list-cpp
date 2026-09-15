#include "TodoList.h"
#include <fstream>
#include <algorithm>
#include <cctype>

int TodoList::addTask(const std::string& title, const std::string& description) {
    Task t(nextId_++, title);
    t.setDescription(description);
    tasks_.push_back(std::move(t));
    return tasks_.back().id();
}

bool TodoList::removeTask(int id) {
    auto it = std::find_if(tasks_.begin(), tasks_.end(),
                           [id](const Task& t) { return t.id() == id; });
    if (it == tasks_.end()) return false;
    tasks_.erase(it);
    return true;
}

Task* TodoList::findTask(int id) {
    for (auto& t : tasks_) if (t.id() == id) return &t;
    return nullptr;
}

const Task* TodoList::findTask(int id) const {
    for (const auto& t : tasks_) if (t.id() == id) return &t;
    return nullptr;
}

bool TodoList::markCompleted(int id, bool completed) {
    if (auto* t = findTask(id)) { t->setCompleted(completed); return true; }
    return false;
}

bool TodoList::setPriority(int id, Priority p) {
    if (auto* t = findTask(id)) { t->setPriority(p); return true; }
    return false;
}

bool TodoList::setDeadline(int id, const std::string& deadline) {
    if (auto* t = findTask(id)) { t->setDeadline(deadline); return true; }
    return false;
}

bool TodoList::addTag(int id, const std::string& tag) {
    if (auto* t = findTask(id)) { t->addTag(tag); return true; }
    return false;
}

bool TodoList::removeTag(int id, const std::string& tag) {
    if (auto* t = findTask(id)) return t->removeTag(tag);
    return false;
}

static std::string toLower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return s;
}

std::vector<Task> TodoList::search(const std::string& query) const {
    std::vector<Task> result;
    std::string q = toLower(query);
    for (const auto& t : tasks_) {
        bool match = toLower(t.title()).find(q) != std::string::npos ||
                     toLower(t.description()).find(q) != std::string::npos;
        if (!match) {
            for (const auto& tag : t.tags()) {
                if (toLower(tag).find(q) != std::string::npos) { match = true; break; }
            }
        }
        if (match) result.push_back(t);
    }
    return result;
}

std::vector<Task> TodoList::filterByTag(const std::string& tag) const {
    std::vector<Task> result;
    for (const auto& t : tasks_) if (t.hasTag(tag)) result.push_back(t);
    return result;
}

std::vector<Task> TodoList::filterByPriority(Priority p) const {
    std::vector<Task> result;
    for (const auto& t : tasks_) if (t.priority() == p) result.push_back(t);
    return result;
}

std::vector<Task> TodoList::filterCompleted(bool completed) const {
    std::vector<Task> result;
    for (const auto& t : tasks_) if (t.completed() == completed) result.push_back(t);
    return result;
}

bool TodoList::saveToFile(const std::string& path) const {
    std::ofstream out(path, std::ios::trunc);
    if (!out) return false;
    out << "# todo-list-cpp v2\n";
    out << tasks_.size() << '\n';
    for (const auto& t : tasks_) out << t.toString() << '\n';
    return static_cast<bool>(out);
}

bool TodoList::loadFromFile(const std::string& path) {
    std::ifstream in(path);
    if (!in) return false;

    std::string header;
    if (!std::getline(in, header)) return false;
    if (header.rfind("# todo-list-cpp", 0) != 0) return false;

    std::string countLine;
    if (!std::getline(in, countLine)) return false;
    std::size_t count = 0;
    try { count = static_cast<std::size_t>(std::stoul(countLine)); }
    catch (...) { return false; }

    std::vector<Task> loaded;
    int maxId = 0;
    for (std::size_t i = 0; i < count; ++i) {
        std::string line;
        if (!std::getline(in, line)) return false;
        auto t = Task::fromString(line);
        if (!t) return false;
        maxId = std::max(maxId, t->id());
        loaded.push_back(std::move(*t));
    }

    tasks_ = std::move(loaded);
    nextId_ = maxId + 1;
    return true;
}
