#ifndef VEHICLE_H
#define VEHICLE_H
#include <string>
// Event defines TimeStamp
#include "event.h"

struct Vehicle
{
    Vehicle() : start(0), end(0), rowIndex(0), colIndex(0), rowDir(false)
    {}
    Vehicle(TimeStamp s, TimeStamp e, std::string vid, size_t ridx, size_t cidx)
        : start(s), end(e), id(vid),  rowIndex(ridx), colIndex(cidx), rowDir(false) {}
    TimeStamp start;
    TimeStamp end;
    std::string id;
    size_t rowIndex;
    size_t colIndex;
    bool rowDir;
};

#endif
