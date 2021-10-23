#include "derived_events.h"
#include "streetgrid.h"

MonitorEvent::MonitorEvent(const TimeStamp& ts,
                           StreetGrid& conditions)
    : Event(ts), conditions_(conditions)
{
}

EventList MonitorEvent::process()
{
    //  std::cout << "MonitorEvent::process()" << std::endl;
    EventList newEvents;
    conditions_.monitorState();
    if(!conditions_.allVehiclesArrived()) {

        newEvents.push_back(new MonitorEvent(
                                this->timeStamp() + SCALE_FACTOR, conditions_));
    }
    return newEvents;
}

AddEvent::AddEvent(const TimeStamp& ts,
                   StreetGrid& conditions,
                   const Vehicle& vehicle)
    : Event(ts), conditions_(conditions), vehicle_(vehicle)
{
}

EventList AddEvent::process()
{
    return conditions_.addVehicle(vehicle_);
}

BlockageEvent::BlockageEvent(StreetGrid& conditions,
                             const TimeStamp& ts,
                             size_t row, size_t col, bool rowDir, bool val)
    : Event(ts), conditions_(conditions), row_(row), col_(col),
      rowDir_(rowDir), val_(val)
{

}
EventList BlockageEvent::process()
{
    conditions_.setBlockage(row_, col_, rowDir_, val_);
    return EventList();
}

ArrivalEvent::ArrivalEvent(const TimeStamp& ts,
                           StreetGrid& conditions,
                           const std::string& vehicleID)
    : Event(ts), conditions_(conditions), vehicleID_(vehicleID)
{

}

EventList ArrivalEvent::process()
{
    return conditions_.processArrival(vehicleID_, this->timeStamp());
}

