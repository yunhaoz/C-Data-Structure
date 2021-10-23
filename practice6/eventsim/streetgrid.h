#ifndef STREETGRID_H
#define STREETGRID_H
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "event.h"
#include "vehicle.h"
#include "heap.h"

class StreetGrid
{
public:
    /**
     * [TO BE COMPLETED]
     * Constructs the Street grid conditions
     *
     * @param[in] rowCapacities
     *    vector of capacities for row 0 to n-1
     * @param[in] colCapacities
     *    vector of capacities for column 0 to m-1
     */
    StreetGrid(const std::vector<unsigned int> rowCapacities,
               const std::vector<unsigned int> colCapacities);

    ~StreetGrid();
    /**
     * [COMPLETE]
     * Returns the number of rows, n
     */
    size_t numRows() const;

    /**
     * [COMPLETE]
     * Returns the number of columns, m
     */
    size_t numCols() const;

    /**
     * Sets the number of expected vehicles
     */
    void setExpected(size_t expectedVehicles);

    /**
     * [TO BE COMPLETED]
     * Adds a new vehicle to the street grid using
     * assuming its start TimeStamp represents the current time.
     *
     * @param[in] v
     *    Vehicle data
     *
     * @return EventList of new events spawned by this addition
     *
     * @throws std::logic_error if a vehicle with that ID has already
     *           been added
     */
    EventList addVehicle(const Vehicle& v);

    /**
     * [TO BE COMPLETED]
     * Blocks or unblocks a particular street segment
     *
     * @pre Both row and column segments from a given intersection/vertex
     *      shall not be blocked at the same time. Neither should a row
     *      row segment in the bottom nor a column sigment in the rightmost
     *      column
     *
     * @param[in] row, col
     *    Row and column of starting vertex of the street segment
     * @param[in] rowDir
     *    True indicates the row segment should be set,
     *    false indicates the column segment should be set
     * @param[in] val
     *    True indicates blocked, false indicates unblocked
     */
    void setBlockage(size_t row, size_t col, bool rowDir, bool val);

    /**
     * [TO BE COMPLETED]
     * Processes a vehicle arrival and moves it to the next street segment
     *
     * @param[in] vehicleID
     *  ID of the vehicle to process
     * @param[in] ts
     *  Now (current time)
     * @return EventList of new events spawned by this processing
     *
     * @throws std::logic_error if no vehicle with that ID exists
     */
    EventList processArrival(const std::string& vehicleID, const TimeStamp& ts);

    /**
     * [COMPLETE]
     * Return true if all added vehicles have arrived at the terminal location
     */
    bool allVehiclesArrived() const;

    /**
     * [COMPLETE]
     * Outputs all completed vehicle info in the order of their completion
     */
    void outputCompletedVehicles(std::ostream& ostr) const;

    /**
     * [COMPLETE]
     * Debug function to output current state
     */
    void monitorState();

private:
    /**
     * [TO BE COMPLETED]
     * Returns true if the vehicle at intersection row,col should choose
     *  to travel down a row or travel down a column
     *
     * If the vehicle can choose either, it should choose the option
     *  with the lower time/delay. If both directions have an equal time
     *  then vehicles should choose the opposite direction as the last vehicle
     *  in that intersection that encountered equal times in both directions,
     *  starting with the choice of row for the first vehicle in that situation.
     *
     * @returns true if the vehicle should travel in the current row and
     *          false if the vehicle should travel down the current column
     */
    bool shouldChooseRow(size_t row, size_t col);

    /**
     * [TO BE COMPLETED]
     * Returns the number of vehicles on the row street segment originating
     * from the intersection given by row,col
     */
    size_t getRowOccupancy(size_t row, size_t col) const;

    /**
     * [TO BE COMPLETED]
     * Returns the number of vehicles on the column street segment originating
     * from the intersection given by row,col
     */
    size_t getColOccupancy(size_t row, size_t col) const;

    /**
     * [TO BE COMPLETED]
     * Returns true if the street segment is blocked (false otherwise) for
     * the row street segment (if rowDir=true) or column street segment
     * (if rowDir=false) that originates from the intersection given
     * by row,col
     */
    bool isBlocked(size_t row, size_t col, bool rowDir) const;

    /**** YOU MAY ADD PRIVATE HELPER FUNCTIONS HERE *****/




    /**** REQUIRED DATA MEMBER -- DO NOT CHANGE ****/
    size_t n;  // number of row streets
    size_t m;  // number of col streets
    // n = Number of row streets
    std::vector<unsigned int> rowCapacities_;
    // m = Number of column streets
    std::vector<unsigned int> colCapacities_;
    // Number of expected vehicles
    size_t expectedVehicles_;
    // Map to track vehicles en-route (key = vehicle ID)
    std::map<std::string, Vehicle> vehicles_;
    // List to store the completed vehicles in the order
    // they are completed
    std::vector<Vehicle> completed_;


    /**** YOU MAY ADD MORE DATA MEMBERS HERE *****/
    /*Heap<Event> process;*/
    //2-D array storing:
    //data members for blockage
    bool** rowBlockage;
    bool** colBlockage;
    //data members for occupancys
    size_t** rowOccupancy;
    size_t** colOccupancy;
    //the case for the choosing the direction same 
    //amount of the time
    bool** lastDirection;


};


#endif
