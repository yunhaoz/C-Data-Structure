#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "input_reader.h"
#include "derived_events.h"
#include "streetgrid.h"
#include "heap.h"

using namespace std;

struct PQEventComparator {
    bool operator()(Event* a, Event* b) {
        return (a->timeStamp() > b->timeStamp()) ||
               (a->timeStamp() == b->timeStamp() &&
                a->getSubPriority() > b->getSubPriority());
    }
};

/**
 * Functor to be used by the heap (priority queue) to determine
 * if event a is BEFORE event b in time.
 *
 */
struct HeapEventComparator {
    bool operator()(Event* a, Event* b) {
        // TO BE COMPLETED
        return a->timeStamp() < b->timeStamp();
        /*cout << a->timeStamp << endl;*/
    }
};

using namespace std;
int main(int argc, char* argv[])
{
    // check command line args
    if(argc < 3) {
        std::cout << "Provide an input file and output file" << std::endl;
        return 1;
    }
    std::ifstream ifile(argv[1]);
    std::vector<unsigned int> rowCapacities;
    std::vector<unsigned int> colCapacities;
    std::vector<Vehicle> vehicleStarts;
    try {
        readConfig(ifile, rowCapacities, colCapacities);
    }
    catch(std::exception& e) {
        cout << e.what() << endl;
        return 1;
    }

    StreetGrid streets(rowCapacities, colCapacities);

    EventList elist;
    try {
        elist = readEvents(ifile,
                           rowCapacities.size(),
                           colCapacities.size(),
                           streets);
    }
    catch(std::exception& e) {
        cout << e.what() << endl;
        return 1;

    }

    Heap<Event*, HeapEventComparator> pq(2);
    pq.push(new MonitorEvent(SCALE_FACTOR, streets));

    // TO BE COMPLETED
    //
    // Implement the main event simulation loop here.
    // Be sure to add the events read by readEvents and implement the
    //   approach in listing 1 of the writeup.
    // Be sure to free any memory resources when appropriate to avoid leaks
    //   and be sure to output the order list of vehicle completions by
    //   calling StreetGrid::outputCompletedVehicles() when the simulation
    //   finished.

    ofstream ofile(argv[2]);
    for (size_t i = 0; i < elist.size(); i++)
    {
        pq.push(elist.at(i));
    }
    while (pq.empty() != true)
    {
        Event* temp = pq.top();
        pq.pop();
        try 
        {
            EventList templist = temp->process();//return the eventlist
            for (size_t i = 0; i < templist.size(); i++) 
            {
                pq.push(templist.at(i));
            }
        }
        catch (exception & e) {
            cout << e.what();
        }
    }
    streets.outputCompletedVehicles(ofile);
    ofile.close();

    return 0;
}

