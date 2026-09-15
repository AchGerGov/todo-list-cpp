#pragma once

#include <string>
#include <vector>
#include <optional>
#include <cstddef>

enum class Priority {
    Low = 0,
    Medium = 1,
    High = 2
};

std::string priorityToString(Priority p);
std::optional<Priority> priorityFromString(const std::string& s);

class Task {
public:
    Task() = default;
    Task(int id, const std::string& title);

    int id() const noexcept { return id_; }
    void setId(int id) { id_ = id; }

    const std::string& title() const noexcept { return title_; }
    void setTitle(const std::string& t) { title_ = t; }

    const std::string& description() const noexcept { return description_; }
    void setDescription(const std::string& d) { description_ = d; }

    bool completed() const noexcept { return completed_; }
    void setCompleted(bool c) { completed_ = c; }

    Priority priority() const noexcept { return priority_; }
    void setPriority(Priority p) { priority_ = p; }

    const std::vector<std::string>& tags() const noexcept { return tags_; }
    void addTag(const std::string& tag);
    bool removeTag(const std::string& tag);
    bool hasTag(const std::string& tag) const;

    // Дедлайн в формате YYYY-MM-DD (пустая строка = не задан)
    const std::string& deadline() const noexcept { return deadline_; }
    void setDeadline(const std::string& d) { deadline_ = d; }
    bool hasDeadline() const noexcept { return !deadline_.empty(); }

    std::string toString() const;
    static std::optional<Task> fromString(const std::string& line);

private:
    int id_ = 0;
    std::string title_;
    std::string description_;
    bool completed_ = false;
    Priority priority_ = Priority::Medium;
    std::vector<std::string> tags_;
    std::string deadline_;
};
