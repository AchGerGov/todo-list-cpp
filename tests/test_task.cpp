#include <gtest/gtest.h>
#include "Task.h"

TEST(TaskTest, Constructor) {
    Task t(1, "Test");
    EXPECT_EQ(t.id(), 1);
    EXPECT_EQ(t.title(), "Test");
    EXPECT_FALSE(t.completed());
    EXPECT_EQ(t.priority(), Priority::Medium);
}

TEST(TaskTest, Tags) {
    Task t(1, "T");
    t.addTag("work");
    t.addTag("work");   // дубликат не добавляется
    t.addTag("home");
    EXPECT_EQ(t.tags().size(), 2u);
    EXPECT_TRUE(t.hasTag("work"));
    EXPECT_TRUE(t.removeTag("work"));
    EXPECT_FALSE(t.hasTag("work"));
    EXPECT_FALSE(t.removeTag("nope"));
}

TEST(TaskTest, Deadline) {
    Task t(1, "T");
    EXPECT_FALSE(t.hasDeadline());
    t.setDeadline("2026-01-01");
    EXPECT_TRUE(t.hasDeadline());
    EXPECT_EQ(t.deadline(), "2026-01-01");
}

TEST(TaskTest, SerializationRoundTrip) {
    Task t(42, "Title with | pipe");
    t.setDescription("line1\nline2\twith tab");
    t.setPriority(Priority::High);
    t.setCompleted(true);
    t.setDeadline("2030-12-31");
    t.addTag("alpha");
    t.addTag("beta");

    auto parsed = Task::fromString(t.toString());
    ASSERT_TRUE(parsed.has_value());
    EXPECT_EQ(parsed->id(), 42);
    EXPECT_EQ(parsed->title(), "Title with | pipe");
    EXPECT_EQ(parsed->description(), "line1\nline2\twith tab");
    EXPECT_TRUE(parsed->completed());
    EXPECT_EQ(parsed->priority(), Priority::High);
    EXPECT_EQ(parsed->deadline(), "2030-12-31");
    EXPECT_TRUE(parsed->hasTag("alpha"));
    EXPECT_TRUE(parsed->hasTag("beta"));
}

TEST(TaskTest, PriorityString) {
    EXPECT_EQ(priorityToString(Priority::Low), "Low");
    EXPECT_EQ(priorityToString(Priority::Medium), "Medium");
    EXPECT_EQ(priorityToString(Priority::High), "High");
    EXPECT_EQ(priorityFromString("High").value(), Priority::High);
    EXPECT_FALSE(priorityFromString("nope").has_value());
}
