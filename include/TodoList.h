#pragma once

#include "Task.h"
#include <vector>
#include <optional>
#include <string>
#include <cstddef>

class TodoList {
public:
    int addTask(const std::string& title, const std::string& description = "");
    bool removeTask(int id);

    Task* findTask(int id);
    const Task* findTask(int id) const;

    bool markCompleted(int id, bool completed = true);
    bool setPriority(int id, Priority p);
    bool setDeadline(int id, const std::string& deadline);
    bool addTag(int id, const std::string& tag);
    bool removeTag(int id, const std::string& tag);

    // Поиск и фильтры (фича «Поиск»)
    std::vector<Task> search(const std::string& query) const;
    std::vector<Task> filterByTag(const std::string& tag) const;
    std::vector<Task> filterByPriority(Priority p) const;
    std::vector<Task> filterCompleted(bool completed) const;

    const std::vector<Task>& tasks() const noexcept { return tasks_; }
    std::size_t size() const noexcept { return tasks_.size(); }

    // Сохранение/загрузка (фича «Сохранение»)
    bool saveToFile(const std::string& path) const;
    bool loadFromFile(const std::string& path);

    void clear() { tasks_.clear(); nextId_ = 1; }

private:
    std::vector<Task> tasks_;
    int nextId_ = 1;
};
