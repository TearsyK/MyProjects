#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <ctime>
#include <atomic>
#include <memory>

// Abstract base class: Item
class Item {
protected:
    static std::atomic<int> globalIdCounter;
public:
    int id;
    time_t creationTime;

    Item();
    virtual ~Item() = default;

    virtual std::string getTypeName() const = 0;
    virtual std::string getNameField() const = 0;
    virtual void display() const = 0;
};

// A minimal Priority enum for tasks
enum class Priority {
    LOW,
    MEDIUM,
    HIGH
};

// Convert Priority to string
std::string priorityToString(Priority p);

// Concrete class: Task
class Task : public Item {
public:
    std::string description;
    bool completed;
    Priority priority;
    std::string category;
    std::string assignedUser;
    time_t dueDate;

    Task();

    Task(const std::string& desc,
        bool isDone,
        Priority prio,
        const std::string& cat,
        const std::string& user,
        time_t due);

    void markCompleted();
    std::string getTypeName() const override;
    std::string getNameField() const override;
    void display() const override;
};

// Concrete class: Event
class Event : public Item {
public:
    std::string title;
    time_t startTime;
    time_t endTime;
    std::string location;

    Event();

    Event(const std::string& t,
        time_t start,
        time_t end,
        const std::string& loc);

    std::string getTypeName() const override;
    std::string getNameField() const override;
    void display() const override;
};

#endif // ITEM_H
