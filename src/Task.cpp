#include "Task.h"
#include <sstream>
#include <algorithm>
#include <cctype>

std::string priorityToString(Priority p) {
    switch (p) {
        case Priority::Low:    return "Low";
        case Priority::Medium: return "Medium";
        case Priority::High:   return "High";
    }
    return "Medium";
}
std::optional<Priority> priorityFromString(const std::string& s) {
    std::string lower;
    lower.reserve(s.size());
    for (char c : s) {
        lower += static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }
    if (lower == "low")    return Priority::Low;
    if (lower == "medium") return Priority::Medium;
    if (lower == "high")   return Priority::High;
    return std::nullopt;
}

Task::Task(int id, const std::string& title)
    : id_(id), title_(title) {}

void Task::addTag(const std::string& tag) {
    if (tag.empty()) return;
    if (std::find(tags_.begin(), tags_.end(), tag) == tags_.end()) {
        tags_.push_back(tag);
    }
}

bool Task::removeTag(const std::string& tag) {
    auto it = std::find(tags_.begin(), tags_.end(), tag);
    if (it == tags_.end()) return false;
    tags_.erase(it);
    return true;
}

bool Task::hasTag(const std::string& tag) const {
    return std::find(tags_.begin(), tags_.end(), tag) != tags_.end();
}

// Экранирование: \ -> \\ , TAB -> \t , LF -> \n
static std::string escapeField(const std::string& s) {
    std::string out;
    out.reserve(s.size());
    for (char c : s) {
        switch (c) {
            case '\\': out += "\\\\"; break;
            case '\t': out += "\\t";  break;
            case '\n': out += "\\n";  break;
            default:   out += c;      break;
        }
    }
    return out;
}

static std::string unescapeField(const std::string& s) {
    std::string out;
    out.reserve(s.size());
    for (std::size_t i = 0; i < s.size(); ++i) {
        if (s[i] == '\\' && i + 1 < s.size()) {
            char n = s[i + 1];
            if (n == '\\') { out += '\\'; ++i; continue; }
            if (n == 't')  { out += '\t'; ++i; continue; }
            if (n == 'n')  { out += '\n'; ++i; continue; }
        }
        out += s[i];
    }
    return out;
}

std::string Task::toString() const {
    std::ostringstream oss;
    oss << id_ << '\t'
        << escapeField(title_) << '\t'
        << escapeField(description_) << '\t'
        << (completed_ ? '1' : '0') << '\t'
        << priorityToString(priority_) << '\t';
    for (std::size_t i = 0; i < tags_.size(); ++i) {
        if (i) oss << ',';
        oss << escapeField(tags_[i]);
    }
    oss << '\t' << escapeField(deadline_);
    return oss.str();
}

static std::vector<std::string> split(const std::string& s, char delim) {
    std::vector<std::string> parts;
    std::string cur;
    for (char c : s) {
        if (c == delim) { parts.push_back(cur); cur.clear(); }
        else cur += c;
    }
    parts.push_back(cur);
    return parts;
}

std::optional<Task> Task::fromString(const std::string& line) {
    auto parts = split(line, '\t');
    if (parts.size() != 7) return std::nullopt;

    Task t;
    try {
        t.id_ = std::stoi(parts[0]);
    } catch (...) {
        return std::nullopt;
    }
    t.title_       = unescapeField(parts[1]);
    t.description_ = unescapeField(parts[2]);
    t.completed_   = (parts[3] == "1");
    auto p = priorityFromString(parts[4]);
    t.priority_ = p.value_or(Priority::Medium);

    if (!parts[5].empty()) {
        for (auto& tag : split(parts[5], ',')) {
            if (!tag.empty()) t.tags_.push_back(unescapeField(tag));
        }
    }
    t.deadline_ = unescapeField(parts[6]);
    return t;
}
