#include "input_reader.h"
#include <sstream>

void readConfig(std::istream& istr,
                std::vector<unsigned int>& rowCapacities,
                std::vector<unsigned int>& colCapacities)
{
    std::string myline;
    if( ! getline(istr, myline) ) {
        throw std::logic_error("Expected line for rowbound capacities");
    }
    size_t vale = 0;
    std::stringstream sse(myline);
    sse >> vale;
    rowCapacities.resize(vale);
    for(size_t i = 0; i < rowCapacities.size(); i++) {
        sse >>rowCapacities[i];
    }
    if(!sse) {
        throw std::logic_error("Unable to read rowbound capacities");
    }

    // Read colbound capacities
    if( ! getline(istr, myline) ) {
        throw std::logic_error("Expected line for colbound capacities");
    }
    size_t vals = 0;
    std::stringstream sss(myline);
    sss >> vals;
    colCapacities.resize(vals);
    for(size_t i = 0; i < colCapacities.size(); i++) {
        sss >> colCapacities[i];
    }
    if(!sss) {
        throw std::logic_error("Unable to read colbound capacities");
    }

}
EventList readEvents(std::istream& istr,
                     size_t n, size_t m,
                     StreetGrid& streets)
{
    EventList elist;
    std::string myline;
    unsigned int expectedVehicles = 0;

    // Read the Events
    while( getline(istr, myline) ) {
        std::string mytype;
        std::stringstream ssv(myline);
        TimeStamp start;
        std::string id;
        unsigned int ridx, cidx;
        ssv >> mytype;
        if(mytype == "A") {
            ssv >> id >> start >> ridx >> cidx;
            if(ssv.fail()) {
                throw std::logic_error("Invalid vehicle");
            }
            if(ridx >= n) {
                throw std::logic_error("Invalid row index");
            }
            if(cidx >= m) {
                throw std::logic_error("Invalid col index");
            }
            Vehicle v(start, 0, id, ridx, cidx);
            elist.push_back(new AddEvent(v.start, streets, v));
            expectedVehicles++;
        }
        else if(mytype == "B") {
            std::string row_or_col;
            bool rowDir, val;
            ssv >> start >> ridx >> cidx >> row_or_col >> val;
            if(ssv.fail()) {
                throw std::logic_error("Invalid vehicle");
            }
            if(ridx >= n) {
                throw std::logic_error("Invalid row index");
            }
            if(cidx >= m) {
                throw std::logic_error("Invalid col index");
            }
            if(row_or_col != "R" && row_or_col != "C" && row_or_col != "r" && row_or_col != "c") {
                throw std::logic_error("Invalid rowDir");
            }
            if(row_or_col == "R" || row_or_col == "r") {
                rowDir = true;
            }
            else {
                rowDir = false;
            }
            elist.push_back(new BlockageEvent(streets, start, ridx, cidx, rowDir, val));
        }
        else {
            throw std::logic_error("Invalid event type");
        }
    }
    streets.setExpected(expectedVehicles);
    return elist;
}
