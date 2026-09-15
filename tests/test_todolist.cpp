#include <gtest/gtest.h>
#include "TodoList.h"
#include <cstdio>
#include <fstream>

TEST(TodoListTest, AddAndFind) {
    TodoList l;
    int a = l.addTask("A");
    int b = l.addTask("B");
    EXPECT_NE(a, b);
    EXPECT_EQ(l.size(), 2u);
    ASSERT_NE(l.findTask(a), nullptr);
    EXPECT_EQ(l.findTask(a)->title(), "A");
    EXPECT_EQ(l.findTask(999), nullptr);
}

TEST(TodoListTest, RemoveAndComplete) {
    TodoList l;
    int id = l.addTask("X");
    EXPECT_TRUE(l.markCompleted(id));
    EXPECT_TRUE(l.findTask(id)->completed());
    EXPECT_TRUE(l.markCompleted(id, false));
    EXPECT_FALSE(l.findTask(id)->completed());
    EXPECT_TRUE(l.removeTask(id));
    EXPECT_FALSE(l.removeTask(id));
}

TEST(TodoListTest, FiltersAndSearch) {
    TodoList l;
    int a = l.addTask("Buy milk", "for breakfast");
    int b = l.addTask("Write report");
    int c = l.addTask("Call mom");

    l.addTag(a, "home");
    l.addTag(b, "work");
    l.setPriority(c, Priority::High);
    l.markCompleted(b);

    EXPECT_EQ(l.search("milk").size(), 1u);
    EXPECT_EQ(l.search("breakfast").size(), 1u);
    EXPECT_EQ(l.search("work").size(), 1u);      // нашли по тегу
    EXPECT_EQ(l.filterByTag("home").size(), 1u);
    EXPECT_EQ(l.filterByPriority(Priority::High).size(), 1u);
    EXPECT_EQ(l.filterCompleted(true).size(), 1u);
    EXPECT_EQ(l.filterCompleted(false).size(), 2u);
}

TEST(TodoListTest, SaveAndLoad) {
    const std::string path = "test_todolist_tmp.txt";

    TodoList a;
    int x = a.addTask("Task X", "desc");
    a.addTag(x, "tag1");
    a.setPriority(x, Priority::High);
    a.setDeadline(x, "2026-06-01");
    a.markCompleted(x);
    a.addTask("Task Y");
    ASSERT_TRUE(a.saveToFile(path));

    TodoList b;
    ASSERT_TRUE(b.loadFromFile(path));
    EXPECT_EQ(b.size(), 2u);
    auto* t = b.findTask(x);
    ASSERT_NE(t, nullptr);
    EXPECT_EQ(t->title(), "Task X");
    EXPECT_EQ(t->description(), "desc");
    EXPECT_TRUE(t->completed());
    EXPECT_EQ(t->priority(), Priority::High);
    EXPECT_EQ(t->deadline(), "2026-06-01");
    EXPECT_TRUE(t->hasTag("tag1"));

    // Загрузка из несуществующего файла
    TodoList c;
    EXPECT_FALSE(c.loadFromFile("definitely_missing_file_xyz.txt"));

    std::remove(path.c_str());
}
