#include <stdexcept>
#include <algorithm>
#include <iomanip>
#include "streetgrid.h"
#include "derived_events.h"

StreetGrid::StreetGrid(
    const std::vector<unsigned int> rowCapacities,
    const std::vector<unsigned int> colCapacities)
    : rowCapacities_(rowCapacities), colCapacities_(colCapacities)
{
    n = rowCapacities.size();
    m = colCapacities.size();
    expectedVehicles_ = 0;

    // TO BE COMPLETED
    //initialize row_occupancy
    last_turn = new int*[n];
    for(unsigned int i=0;i<n;i++){
        last_turn[i] = new int[m];
        for(unsigned int j=0;j<m-1;j++){
            last_turn[i][j]=0;
        }
    }
    row_occupancy = new int*[n];
    for(unsigned int i=0;i<n;i++){
        row_occupancy[i] = new int[m-1];
        for(unsigned int j=0;j<m-1;j++){
            row_occupancy[i][j]=0;
        }
    }
    row_block = new int*[n];
    for(unsigned int i=0;i<n;i++){
        row_block[i] = new int[m-1];
        for(unsigned int j=0;j<m-1;j++){
            row_block[i][j]=0;
        }
    }
    //initialize column_occupancy
    col_occupancy = new int*[n-1];
    for(unsigned int i=0;i<n-1;i++){
        col_occupancy[i] = new int[m];
        for(unsigned int j=0;j<m;j++){
            col_occupancy[i][j]=0;
        }
    }
    col_block = new int*[n-1];
    for(unsigned int i=0;i<n-1;i++){
        col_block[i] = new int[m];
        for(unsigned int j=0;j<m;j++){
            col_block[i][j]=0;
        }
    }
    // Call monitor state to output starting state
    monitorState();
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
    return row_occupancy[row][col];
}

size_t StreetGrid::getColOccupancy(size_t row, size_t col) const
{
    // TO BE COMPLETED
    return col_occupancy[row][col];
}

bool StreetGrid::isBlocked(size_t row, size_t col, bool rowDir) const
{
    // TO BE COMPLETED
    if (rowDir){
        return row_block[row][col]==1;
    }
    else{
        return col_block[row][col]==1;
    }
}

bool StreetGrid::shouldChooseRow(size_t row, size_t col)
{
    // TO BE COMPLETED
    if(col>=m-1) return false;
    if(row>=n-1) return true;
    if(row_block[row][col]==1) return false;
    if(col_block[row][col]==1) return true;
    int row_car = getRowOccupancy(row,col);
    int col_car = getColOccupancy(row,col);
    int row_time = max(1,(1.0+row_car)/rowCapacities_[row])*SCALE_FACTOR;
    int col_time = max(1,(1.0+col_car)/colCapacities_[col])*SCALE_FACTOR;
    if(row_time<col_time) return true;
    else if(row_time>col_time) return false;
    else{
        if(last_turn[row][col]==0){
            last_turn[row][col]=1;
            return true;
        }else{
            last_turn[row][col]=0;
            return false;
        }
    }
}

void StreetGrid::setBlockage(size_t row, size_t col, bool rowDir, bool val)
{
    // TO BE COMPLETED
    if(rowDir){
        if(row<n&&col<m-1){//check for boundaries
            if(row_block[row][col]!=val ){
                if(!(val==1 && col_block[row][col]==1)){//cannot block both segments
                    row_block[row][col] = val;
                }
            }
        }
    }
    else{
        if(row<n-1&&col<m){//check for boundaries
            if(col_block[row][col]!=val){
                if(!(val==1 && row_block[row][col]==1)){//cannot block both segments
                    col_block[row][col]=val;
                }
            }
        }
    }

}

EventList StreetGrid::processArrival(const std::string& vehicleID, const TimeStamp& ts)
{
    // TO BE COMPLETED
    if(vehicles_.find(vehicleID)==vehicles_.end()) throw std::logic_error("the vehicle does not exist!");
    Vehicle* v = &vehicles_.find(vehicleID)->second;
    EventList el;
    if(v->rowDir){
        row_occupancy[v->rowIndex][v->colIndex]-=1;
        v->colIndex+=1;
    } else{
        col_occupancy[v->rowIndex][v->colIndex]-=1;
        v->rowIndex+=1;
    }
    if(v->rowIndex==n-1&&v->colIndex==m-1){//if reach the terminal
        v->end=ts;
        completed_.push_back(*v);
        return el;
    }
    //calculate for next segment
    int time;
    if(shouldChooseRow(v->rowIndex,v->colIndex)){//add row occupancy
        int row_car = getRowOccupancy(v->rowIndex,v->colIndex);
        time = max(1,(1.0+row_car)/rowCapacities_[v->rowIndex])*SCALE_FACTOR;
        vehicles_.find(v->id)->second.rowDir = true;
        row_occupancy[v->rowIndex][v->colIndex]+=1;
    }
    else{//add col occupancy
        int col_car = getColOccupancy(v->rowIndex,v->colIndex);
        time = max(1,(1.0+col_car)/colCapacities_[v->colIndex])*SCALE_FACTOR;
        vehicles_.find(v->id)->second.rowDir = false;
        col_occupancy[v->rowIndex][v->colIndex]+=1;
    }

    ArrivalEvent* arrivalEvent = new ArrivalEvent(ts+time,*this,vehicleID);
    el.push_back(arrivalEvent);
    return el;
}

EventList StreetGrid::addVehicle(const Vehicle& v)
{
    // TO BE COMPLETED
    //if the vehicle already exists
    if(vehicles_.find(v.id)!=vehicles_.end()) throw std::logic_error("the vehicle has been added!");
    //create the event list
    EventList el;
    vehicles_.insert(std::pair<std::string,Vehicle>(v.id,v));
    if(v.rowIndex==n-1 && v.colIndex==m-1){ //add to the terminal
        vehicles_.find(v.id)->second.end=v.start;
        completed_.push_back(vehicles_.find(v.id)->second);
    }
    int time;
    if(shouldChooseRow(v.rowIndex,v.colIndex)){//add row occupancy
        int row_car = getRowOccupancy(v.rowIndex,v.colIndex);
        time = max(1,(1.0+row_car)/rowCapacities_[v.rowIndex])*SCALE_FACTOR;
        vehicles_.find(v.id)->second.rowDir = true;
        row_occupancy[v.rowIndex][v.colIndex]+=1;
    }
    else{//add col occupancy
        int col_car = getColOccupancy(v.rowIndex,v.colIndex);
        time = max(1,(1.0+col_car)/colCapacities_[v.colIndex])*SCALE_FACTOR;
        vehicles_.find(v.id)->second.rowDir = false;
        col_occupancy[v.rowIndex][v.colIndex]+=1;
    }
    //trigger event
    ArrivalEvent* arrivalEvent = new ArrivalEvent(v.start+time,*this,v.id);
    el.push_back(arrivalEvent);
    return el;
}

double StreetGrid::max(double a, double b)
{
    if(a>=b) return a;
    else return b;
}
StreetGrid::~StreetGrid() {
    for(unsigned int i=0;i<n;i++){
        if(i<n-1){
            delete[] col_occupancy[i];
            delete[] col_block[i];
        }
        delete[] last_turn[i];
        delete[] row_occupancy[i];
        delete[] row_block[i];
    }
    delete[] last_turn;
    delete[] row_occupancy;
    delete[] row_block;
    delete[] col_occupancy;
    delete[] col_block;
}
