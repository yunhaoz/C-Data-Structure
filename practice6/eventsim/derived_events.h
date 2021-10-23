#ifndef DERIVED_EVENTS_H
#define DERIVED_EVENTS_H
#include <string>
#include "event.h"
#include "vehicle.h"
#include "streetgrid.h"


class MonitorEvent : public Event
{
public:
    MonitorEvent(const TimeStamp& ts,
                 StreetGrid& conditions);
    EventList process();
    unsigned int getSubPriority() const {
        return 0;
    }

private:
    StreetGrid& conditions_;
};


class AddEvent : public Event
{
public:
    AddEvent(const TimeStamp& ts,
             StreetGrid& conditions,
             const Vehicle& vehicle);
    EventList process();
    unsigned int getSubPriority() const {
        return 1;
    }

private:
    StreetGrid& conditions_;
    Vehicle vehicle_;
};




class BlockageEvent : public Event
{
public:
    BlockageEvent(StreetGrid& conditions,
                  const TimeStamp& ts,
                  size_t row, size_t col, bool rowDir, bool val);
    EventList process();
    unsigned int getSubPriority() const {
        return 2;
    }

private:
    StreetGrid& conditions_;
    size_t row_, col_;
    bool rowDir_, val_;
};

class ArrivalEvent : public Event
{
public:
    ArrivalEvent(const TimeStamp& ts,
                 StreetGrid& conditions,
                 const std::string& vehicleID);
    EventList process();
    unsigned int getSubPriority() const {
        return 3;
    }

private:
    StreetGrid& conditions_;
    std::string vehicleID_;
};


#endif
