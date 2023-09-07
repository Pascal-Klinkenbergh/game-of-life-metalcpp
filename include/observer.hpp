#pragma once

#include <vector>

class Observer {
public:
    // This is called to update the sub-class
    virtual void update() = 0;
};

class Observable {
public:
    // Add a new Observer, that gets updated automatically
    void addObserver(Observer *observer);

    // Call the update function on all Observers
    void notifyUpdate();

private:
    // All observers to notify
    std::vector<Observer *> observers;
};

// Add a new Observer, that gets updated automatically
void Observable::addObserver(Observer *observer) {
    observers.push_back(observer);
}

// Call the update function on all Observers
void Observable::notifyUpdate() {
    for (auto &observer : observers)
        observer->update();
}
