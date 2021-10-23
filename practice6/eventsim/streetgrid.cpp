#include <stdexcept>
#include <algorithm>
#include <iomanip>
#include "streetgrid.h"
#include "derived_events.h"
#include <algorithm>

StreetGrid::StreetGrid(
    const std::vector<unsigned int> rowCapacities,
    const std::vector<unsigned int> colCapacities)
    : rowCapacities_(rowCapacities), colCapacities_(colCapacities)
{
    n = rowCapacities.size();
    m = colCapacities.size();
    expectedVehicles_ = 0;

    // TO BE COMPLETED
    //initialize all the data
    //bool** rowBlockage;
    rowBlockage = new bool* [n];
    /*for (int i = 0; i < m; i++)*/
    for (size_t i = 0; i < n; i++)
    {
        rowBlockage[i] = new bool[m - 1];//because the last col don't exist street to be blocked
        for (size_t j = 0; j < m - 1; j++)
        {
            rowBlockage[i][j] = false;
        }
    }

    //colBlockage
    colBlockage = new bool* [n - 1];
    for (size_t i = 0; i < n - 1; i++)//because the last row don't have street to be blocked
    {
        rowBlockage[i] = new bool[m];
        for (size_t j = 0; j < m; j++)
        {
            rowBlockage[i][j] = false;
        }
    }

    //rowOccupancy
    rowOccupancy = new size_t * [n];
    for (size_t i = 0; i < n; i++)
    {
        rowOccupancy[i] = new size_t[m - 1];//because the last col don't exist street to be blocked
        for (size_t j = 0; j < m - 1; j++)
        {
            rowOccupancy[i][j] = 0;
        }
    }

    colOccupancy = new size_t * [n - 1];
    for (size_t i = 0; i < n - 1; i++)//because the last row don't have street to be blocked
    {
        colOccupancy[i] = new size_t[m];
        for (size_t j = 0; j < m; j++)
        {
            colOccupancy[i][j] = 0;
        }
    }

    lastDirection = new bool* [n];
    for (size_t i = 0; i < n; i++)
    {
        lastDirection[i] = new bool[m];
        for (size_t j = 0; j < m; j++)
        {
            lastDirection[i][j] = true;
        }
    }

    // Call monitor state to output starting state
    monitorState();
}

StreetGrid::~StreetGrid()
{
    /*cout << "here" << endl;*/
    for (size_t i = 0; i < n; i++)//some have n some have n-1
    {
        delete[] lastDirection[i];
        delete[] rowBlockage[i];
        delete[] rowOccupancy[i];
    }
    for (size_t i = 0; i < n-1; i++)//some have n some have n-1
    {
        delete[] colBlockage[i];
        delete[] colOccupancy[i];
    }
    delete[] lastDirection;
    delete[] rowBlockage;
    delete[] rowOccupancy;
    delete[] colBlockage;
    delete[] colOccupancy;
}

size_t StreetGrid::numRows() const
{
    return n;
}
size_t StreetGrid::numCols() const
{
    return m;
}
void StreetGrid::setExpected(size_t expectedVehicles)
{
    expectedVehicles_ = expectedVehicles;
}

void StreetGrid::outputCompletedVehicles(std::ostream& ostr) const
{
    for(auto v : completed_) {
        ostr << v.end << " " << v.id << std::endl;
    }
}

bool StreetGrid::allVehiclesArrived() const
{
    return completed_.size() == expectedVehicles_;
}

void StreetGrid::monitorState()
{
    std::cout << "State: " << std::endl;
    std::cout << std::setw(4) << " ";

    // Print header row with column capacities
    for(size_t cidx = 0; cidx < m; ++cidx) {
        std::cout <<  std::setw(4) << colCapacities_[cidx] << " " <<  std::setw(4) << " ";
    }
    std::cout << std::endl;
    std::cout << "    ";
    for(size_t cidx = 0; cidx < m; ++cidx) {
        std::cout <<  std::setw(4) << "====="  << "=====";
    }
    std::cout << std::endl;

    // Start printing grid data
    for(size_t ridx = 0; ridx < n; ++ridx) {
        // Print row capacity
        std::cout << std::setw(2) << rowCapacities_[ridx] << " |";
        // Print row occupancies
        for(size_t cidx = 0; cidx < m; ++cidx) {
            std::cout << std::setw(4) << "+" << " ";
            if(cidx < m-1) {
                if(isBlocked(ridx,cidx,true)) {
                    std::cout << std::setw(3) << getRowOccupancy(ridx,cidx);
                    std::cout << "B";
                }
                else {
                    std::cout << std::setw(4) << getRowOccupancy(ridx,cidx);
                }
            }
        }
        std::cout << std::endl;
        std::cout <<  "   |";
        // Print column occupancies
        if(ridx < n-1) {
            for(size_t cidx = 0; cidx < m; ++cidx) {
                if(isBlocked(ridx,cidx,false)) {
                    std::cout << std::setw(3) << getColOccupancy(ridx,cidx);
                    std::cout << "B";
                }
                else {
                    std::cout << std::setw(4) << getColOccupancy(ridx,cidx);
                }
                std::cout << std::setw(4) << " " << " " ;
            }
        }
        std::cout << std::endl;

    }
}

size_t StreetGrid::getRowOccupancy(size_t row, size_t col) const
{
    // TO BE COMPLETED
    return rowOccupancy[row][col];

}

size_t StreetGrid::getColOccupancy(size_t row, size_t col) const
{
    // TO BE COMPLETED
    return colOccupancy[row][col];
}

bool StreetGrid::isBlocked(size_t row, size_t col, bool rowDir) const
{
    // TO BE COMPLETED
    //if its direction is row
    if (rowDir == true)
    {
        return rowBlockage[row][col];
    }
    else
    {
        return colBlockage[row][col];
    }
}

bool StreetGrid::shouldChooseRow(size_t row, size_t col)
{
    // TO BE COMPLETED
    //check the blockage
    if (rowBlockage[row][col])
    {
        return false;
    }
    if (colBlockage[row][col])
    {
        return true;
    }
    //check whether is the edge
    if (row == n - 1)
    {
        return true;
    }
    if (col == m - 1)
    {
        return false;
    }
    //compare the time
    int rowTime = std::max(1.0, (double)(1.0 + getRowOccupancy(row, col)) / rowCapacities_[row]) * SCALE_FACTOR;
    int colTime = std::max(1.0, (double)(1.0 + getColOccupancy(row, col)) / colCapacities_[col]) * SCALE_FACTOR;
    if (rowTime > colTime)
    {
        return false;
    }
    else if (rowTime < colTime)
    {
        return true;
    }
    else//equal to each other
    {
        if (lastDirection[row][col])
        {
            lastDirection[row][col] = false;
            return true;
        }
        else
        {
            lastDirection[row][col] = true;
            return false;
        }
    }
}

void StreetGrid::setBlockage(size_t row, size_t col, bool rowDir, bool val)
{
    // TO BE COMPLETED
    //got to the row direction
    if (rowDir)
    {
        //whether it is out of the edge
        if (row < n && col < m - 1)
        {
            //can not block both
            if (!(val == true && colBlockage[row][col]==true))
            {
                rowBlockage[row][col] == val;
            }
        }
    }
    else
    {
        if (row < n - 1 && col < m)
        {
            if (!(val == true && rowBlockage[row][col]==true)) 
            {
                colBlockage[row][col] == val;
            }
        }
    }
}

EventList StreetGrid::processArrival(const std::string& vehicleID, const TimeStamp& ts)
{
    // TO BE COMPLETED
    if (vehicles_.find(vehicleID) == vehicles_.end())
    {
        throw std::logic_error("Vehicle doesn't exist");
    }
    Vehicle* temp = &vehicles_.at(vehicleID);//need to use the pointer
    EventList mylist;
    int mytime;
    //go to the row
    if (temp->rowDir)
    {
        rowOccupancy[temp->rowIndex][temp->colIndex]--;
        temp->colIndex++;
    }
    //go to the col
    else
    {
        colOccupancy[temp->rowIndex][temp->colIndex]--;
        temp->rowIndex++;
    }
    // determine whether it is reaching the terminal
    if (temp->rowIndex == n - 1 && temp->colIndex == m - 1)
    {
        completed_.push_back(*temp);
        temp->end == ts;
        return mylist;
    }
    //add the new event
    if (shouldChooseRow(temp->rowIndex, temp->colIndex))
    {
        //calculate the time
        mytime = std::max(1.0, (double)(1.0 + getRowOccupancy(temp->rowIndex, temp->colIndex)) / rowCapacities_[temp->rowIndex]) * SCALE_FACTOR;
        //set to the next direction
        vehicles_.at(vehicleID).rowDir == true;
        rowOccupancy[temp->rowIndex][temp->colIndex]++;
    }
    else
    {
        mytime = std::max(1.0, (double)(1.0 + getColOccupancy(temp->rowIndex, temp->colIndex)) / colCapacities_[temp->colIndex]) * SCALE_FACTOR;
        vehicles_.at(vehicleID).rowDir == false;
        colOccupancy[temp->rowIndex][temp->colIndex]++;
    }
    /*mytime*/
    ArrivalEvent* newEvent = new ArrivalEvent(mytime + ts, *this, vehicleID);
    mylist.push_back(newEvent);
    return mylist;
}

EventList StreetGrid::addVehicle(const Vehicle& v)
{
    // TO BE COMPLETED
    if (vehicles_.find(v.id) == vehicles_.end())
    {
        throw std::logic_error("Vehicle doesn't exist");
    }
    vehicles_.insert(std::make_pair(v.id, v));
    EventList mylist;
    int mytime;
    //if it is at the terminal
    if (v.rowIndex == n - 1 && v.colIndex == m - 1)
    {
        vehicles_.at(v.id).end = v.start;//end=start
        completed_.push_back(vehicles_.at(v.id));
    }
    //similar to process 
    if (shouldChooseRow(v.rowIndex, v.colIndex))
    {
        mytime = std::max(1.0, (double)(1.0 + getRowOccupancy(v.rowIndex, v.colIndex)) / rowCapacities_[v.rowIndex]) * SCALE_FACTOR;
        vehicles_.at(v.id).rowDir = true;
        rowOccupancy[v.rowIndex][v.colIndex]++;
    }
    else
    {
        mytime = std::max(1.0, (double)(1.0 + getColOccupancy(v.rowIndex, v.colIndex)) / colCapacities_[v.colIndex]) * SCALE_FACTOR;
        vehicles_.at(v.id).rowDir == false;
        colOccupancy[v.rowIndex][v.colIndex]++;
    }
    ArrivalEvent* newEvent = new ArrivalEvent(mytime + v.start, *this, v.id);//at this time ts =v.start
    mylist.push_back(newEvent);
    return mylist;
}


