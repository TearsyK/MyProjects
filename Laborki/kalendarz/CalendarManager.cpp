#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include "Item.h"
#include "TimeUtils.h"

// Interfejs budowniczego dla Task
class TaskBuilder {
private:
    std::string description;
    bool isDone = false;
    Priority priority = Priority::LOW;
    std::string category;
    std::string user;
    time_t dueDate = 0;
public:
    TaskBuilder& setDescription(const std::string& desc) { description = desc; return *this; }
    TaskBuilder& setCompleted(bool done) { isDone = done; return *this; }
    TaskBuilder& setPriority(Priority prio) { priority = prio; return *this; }
    TaskBuilder& setCategory(const std::string& cat) { category = cat; return *this; }
    TaskBuilder& setUser(const std::string& usr) { user = usr; return *this; }
    TaskBuilder& setDueDate(time_t due) { dueDate = due; return *this; }

    std::shared_ptr<Task> build() {
        return std::make_shared<Task>(description, isDone, priority, category, user, dueDate);
    }
};

// Strategia sortowania
class ISortStrategy {
public:
    virtual void sort(std::vector<std::shared_ptr<Item>>& items) = 0;
    virtual ~ISortStrategy() = default;
};

class SortByNameAsc : public ISortStrategy {
public:
    void sort(std::vector<std::shared_ptr<Item>>& items) override {
        std::sort(items.begin(), items.end(), [](auto& a, auto& b) {
            auto c = a->getNameField();
            for (auto& d : c)
            {
                d = tolower(d);
            }
            auto e = b->getNameField();
            for (auto& f : e)
            {
                f = tolower(f);
            }
            return  c < e;
            });
    }
};



class SortByNameDesc : public ISortStrategy {
public:
    void sort(std::vector<std::shared_ptr<Item>>& items) override {
        std::sort(items.begin(), items.end(), [](auto& a, auto& b) {
            auto c = a->getNameField();
            for (auto& d : c)
            {
                d = tolower(d);
            }
            auto e = b->getNameField();
            for (auto& f : e)
            {
                f = tolower(f);
            }
            return  c > e;
            });
    }
};

class SortByCreationTimeAsc : public ISortStrategy {
public:
    void sort(std::vector<std::shared_ptr<Item>>& items) override {
        std::sort(items.begin(), items.end(), [](auto& a, auto& b) {
            return a->creationTime < b->creationTime;
            });
    }
};

class SortByCreationTimeDesc : public ISortStrategy {
public:
    void sort(std::vector<std::shared_ptr<Item>>& items) override {
        std::sort(items.begin(), items.end(), [](auto& a, auto& b) {
            return a->creationTime > b->creationTime;
            });
    }
};

class Context
{
private:
    std::shared_ptr<ISortStrategy> strategy_;

public:
    Context(std::shared_ptr<ISortStrategy> strategy) : strategy_(std::move(strategy)) {}
    void setStrategy(std::shared_ptr<ISortStrategy> strategy) {
        strategy_ = std::move(strategy);
    }
    void performTask(std::vector<std::shared_ptr<Item>>& items) const {
        strategy_->sort(items);
    }
};

// Iterator dla Item
class ItemIterator {
private:
    const std::vector<std::shared_ptr<Item>>& items;
    size_t index = 0;
public:
    ItemIterator(const std::vector<std::shared_ptr<Item>>& items) : items(items) {}

    bool hasNext() const { return index < items.size(); }
    std::shared_ptr<Item> next() { return items.at(index++); }
};

// Interfejs fabryki
class ICalendarFactory {
public:
    virtual std::shared_ptr<Item> create_item(const std::string& desc, bool isDone, Priority prio,
        const std::string& cat, const std::string& user,
        time_t dueDate, const std::string& type,
        time_t endT, const std::string& loc) = 0;
    virtual ~ICalendarFactory() = default;
};

class ConcreteCalendarFactory : public ICalendarFactory {
public:
    std::shared_ptr<Item> create_item(const std::string& desc, bool isDone, Priority prio,
        const std::string& cat, const std::string& user,
        time_t dueDate, const std::string& type,
        time_t endT, const std::string& loc) override {
        if (type == "task") {
            return TaskBuilder()
                .setDescription(desc)
                .setCompleted(isDone)
                .setPriority(prio)
                .setCategory(cat)
                .setUser(user)
                .setDueDate(dueDate)
                .build();
        }
        else if (type == "event") {
            return std::make_shared<Event>(desc, isDone, endT, loc);
        }
        else {
            return nullptr;
        }
    }
};

class Observer {
public:
    virtual void notifyItemChanged(Item* item, const std::string& action) = 0;
    virtual ~Observer() = default;
};

class ConsoleObserver : public Observer {
public:
    void notifyItemChanged(Item* item, const std::string& action) override {
        std::cout << "[Observer Notification] " << item->getTypeName()
            << " [ID " << item->id << "]: " << action << "\n";
    }
};

class Calendar {
private:
    Calendar(ICalendarFactory* f) : factory(f) {}
    static Calendar* instance;
    std::vector<std::shared_ptr<Item>> items;
    std::vector<Observer*> observers;
    //std::unique_ptr<ISortStrategy> sortStrategy = std::make_unique<SortByNameAsc>();
    Context sortContext{ std::make_unique<SortByNameAsc>() };
   
    ICalendarFactory* factory;

    void notifyItemChanged(Item* item, const std::string& action) {
        for (Observer* obs : observers) {
            if (obs) obs->notifyItemChanged(item, action);
        }
    }

public:
    Calendar(const Calendar&) = delete;
    Calendar& operator=(const Calendar&) = delete;

    static Calendar* getInstance(ICalendarFactory* f = nullptr) {
        if (!instance) {
            if (!f) throw std::runtime_error("Factory must be provided for the first time.");
            instance = new Calendar(f);
        }
        return instance;
    }

    void attach(Observer* obs) { observers.push_back(obs); }
    void detach(Observer* obs) {
        observers.erase(std::remove(observers.begin(), observers.end(), obs), observers.end());
    }

    void setSortStrategy(int option) {
        switch (option) {
        case 1:
            sortContext.setStrategy(std::make_shared<SortByNameAsc>());
            break;
        case 2:
            sortContext.setStrategy(std::make_shared<SortByNameDesc>());
            break;
        case 3:
            sortContext.setStrategy(std::make_shared<SortByCreationTimeAsc>());
            break;
        case 4:
            sortContext.setStrategy(std::make_shared<SortByCreationTimeDesc>());
            break;
        default:
            std::cout << "Invalid sort option. Using Name Asc by default.\n";
            sortContext.setStrategy(std::make_shared<SortByNameAsc>());
        }
    }



    void addTask(const std::string& desc, bool isDone, Priority prio,
        const std::string& cat, const std::string& user, time_t dueDate) {
        auto task = factory->create_item(desc, isDone, prio, cat, user, dueDate, "task", 0, "");
        items.push_back(task);
        notifyItemChanged(task.get(), "added");
    }

    void addEvent(const std::string& title, time_t startT, time_t endT, const std::string& loc) {
        if (startT == 0) startT = std::time(nullptr);
        if (endT < startT) endT = startT + 3600;

        auto ev = factory->create_item(title, false, Priority::MEDIUM, "", "", startT, "event", endT, loc);
        items.push_back(ev);
        notifyItemChanged(ev.get(), "added");
    }

    void removeItem(int id) {
        for (auto it = items.begin(); it != items.end(); ++it) {
            if ((*it)->id == id) {
                notifyItemChanged(it->get(), "removed");
                items.erase(it);
                return;
            }
        }
        std::cout << "No item found with ID " << id << ".\n";
    }

    void markTaskCompleted(int id) {
        for (auto& it : items) {
            if (it->id == id) {
                if (Task* t = dynamic_cast<Task*>(it.get())) {
                    t->markCompleted();
                    notifyItemChanged(t, "marked completed");
                }
                else {
                    std::cout << "Item [ID " << id << "] is not a Task.\n";
                }
                return;
            }
        }
        std::cout << "No item found with ID " << id << ".\n";
    }

    void displayItems() {
        sortContext.performTask(items);

        if (items.empty()) {
            std::cout << "No items to display.\n";
            return;
        }

        std::cout << "\n======= ALL ITEMS (" << items.size() << ") =======\n\n";
        ItemIterator it(items);
        while (it.hasNext()) {
            it.next()->display();
        }
    }
};

Calendar* Calendar::instance = nullptr;

int main() {
    ConcreteCalendarFactory factory;
    Calendar* calendar = Calendar::getInstance(&factory);
    ConsoleObserver observer;
    
    calendar->attach(&observer);

    while (true) {
        std::cout << "=======================================\n";
        std::cout << " Simple Task & Event Calendar\n";
        std::cout << " 1. Add Task\n";
        std::cout << " 2. Add Event\n";
        std::cout << " 3. Remove Item by ID\n";
        std::cout << " 4. Mark Task Completed\n";
        std::cout << " 5. Show Items\n";
        std::cout << " 6. Change Sort Strategy\n";
        std::cout << " 7. Exit\n";
        std::cout << "Enter choice: ";

        int choice;
        std::cin >> choice;
        std::cin.ignore();
        if (choice == 7) return 0;

        switch (choice) {
        case 1: {
            std::string desc, cat, assigned, dueDateStr;
            int pChoice;
            Priority prio = Priority::LOW;
            std::cout << "Enter task description: "; std::getline(std::cin, desc);
            std::cout << "Priority? (0=LOW, 1=MEDIUM, 2=HIGH): "; std::cin >> pChoice; std::cin.ignore();
            if (pChoice == 1) prio = Priority::MEDIUM;
            else if (pChoice == 2) prio = Priority::HIGH;
            std::cout << "Category (leave empty if none): "; std::getline(std::cin, cat);
            std::cout << "Assigned User (leave empty if none): "; std::getline(std::cin, assigned);
            std::cout << "Enter due date/time (YYYY-MM-DD HH:MM), or empty if none: "; std::getline(std::cin, dueDateStr);
            time_t dd = parseDateTime(dueDateStr);
            calendar->addTask(desc, false, prio, cat, assigned, dd);
            break;
        }
        case 2: {
            std::string title, startStr, endStr, loc;
            std::cout << "Enter event title: "; std::getline(std::cin, title);
            std::cout << "Enter event start date/time (YYYY-MM-DD HH:MM): "; std::getline(std::cin, startStr);
            std::cout << "Enter event end date/time (YYYY-MM-DD HH:MM): "; std::getline(std::cin, endStr);
            std::cout << "Location (leave empty if none): "; std::getline(std::cin, loc);
            time_t startT = parseDateTime(startStr);
            time_t endT = parseDateTime(endStr);
            calendar->addEvent(title, startT, endT, loc);
            break;
        }
        case 3: {
            int id;
            std::cout << "Enter item ID to remove: "; std::cin >> id; std::cin.ignore();
            calendar->removeItem(id);
            break;
        }
        case 4: {
            int id;
            std::cout << "Enter item ID to mark completed (Task only): "; std::cin >> id; std::cin.ignore();
            calendar->markTaskCompleted(id);
            break;
        }
        case 5:
            calendar->displayItems();
            break;
        case 6: {
            int opt;
            std::cout << "Select sorting strategy:\n"
                << "1. Name Asc\n2. Name Desc\n3. Creation Time Asc\n4. Creation Time Desc\nEnter option: ";
            std::cin >> opt;
            std::cin.ignore();
            calendar->setSortStrategy(opt);
            break;
        

        }
        default:
            std::cout << "Invalid choice!\n";
            break;
        }
    }
}
