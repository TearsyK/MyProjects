#include "Item.h"
#include <iostream>
#include "TimeUtils.h"

// Initialize the static counter
std::atomic<int> Item::globalIdCounter{ 1 };

/****************************
 * Base Item Implementation
 ****************************/
Item::Item()
    : id(globalIdCounter++),
    creationTime(std::time(nullptr))
{
}

/****************************
 * Priority -> String
 ****************************/
std::string priorityToString(Priority p) {
    switch (p) {
    case Priority::LOW:    return "LOW";
    case Priority::MEDIUM: return "MEDIUM";
    case Priority::HIGH:   return "HIGH";
    default:               return "UNKNOWN";
    }
}

/****************************
 * Task Implementation
 ****************************/
Task::Task()
    : description(""),
    completed(false),
    priority(Priority::MEDIUM),
    category(""),
    assignedUser(""),
    dueDate(0)                    // 0 means �no due date�
{
    // id and creationTime come from Item::Item()
}

Task::Task(const std::string& desc,
    bool isDone,
    Priority prio,
    const std::string& cat,
    const std::string& user,
    time_t due)
    : description(desc),
    completed(isDone),
    priority(prio),
    category(cat),
    assignedUser(user),
    dueDate(due)
{
    // id and creationTime are set in Item constructor
}

void Task::markCompleted() {
    completed = true;
}

std::string Task::getTypeName() const {
    return "Task";
}
std::string Task::getNameField() const {
    return description;
}
void Task::display() const {
    std::cout << "[ID " << id << "] (Task)\n";
    std::cout << "  Description : " << description << "\n";
    std::cout << "  Status      : " << (completed ? "Completed" : "Pending") << "\n";
    std::cout << "  Priority    : " << priorityToString(priority) << "\n";
    if (!category.empty()) {
        std::cout << "  Category    : " << category << "\n";
    }
    if (!assignedUser.empty()) {
        std::cout << "  AssignedTo  : " << assignedUser << "\n";
    }
    if (dueDate != 0) {
        extern std::string formatTime(time_t);
        std::cout << "  DueDate     : " << formatTime(dueDate) << "\n";
    }
    std::cout << "  CreatedAt   : " << formatTime(creationTime) << "\n\n";
}

/****************************
 * Event Implementation
 ****************************/
Event::Event()
    : title(""),
    startTime(0),               // 0 = unset
    endTime(0),
    location("")
{
    // id and creationTime come from Item::Item()
}

Event::Event(const std::string& t,
    time_t start,
    time_t end,
    const std::string& loc)
    : title(t),
    startTime(start),
    endTime(end),
    location(loc)
{
    // id, creationTime from base
}

std::string Event::getTypeName() const {
    return "Event";
}
std::string Event::getNameField() const {
    return title;
}
void Event::display() const {
    extern std::string formatTime(time_t);

    std::cout << "[ID " << id << "] (Event)\n";
    std::cout << "  Title       : " << title << "\n";
    std::cout << "  Start       : " << formatTime(startTime) << "\n";
    std::cout << "  End         : " << formatTime(endTime) << "\n";
    if (!location.empty()) {
        std::cout << "  Location    : " << location << "\n";
    }
    std::cout << "  CreatedAt   : " << formatTime(creationTime) << "\n\n";
}
