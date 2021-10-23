#ifndef INPUT_READER_H
#define INPUT_READER_H
#include <vector>
#include <iostream>
#include "derived_events.h"
#include "streetgrid.h"

/**
 * Reads the row and column street capacities (first 2 lines)
 * of the configuration file
 *
 * @pre istr's get pointer must be on the 1st line of the
 *      configuration file
 *
 * @param[in] istr
 *    Input stream representing the configuration file
 * @param[out] rowCapacities
 *    Vector containing the capacities of the row streets.
 *    The size of this vector is the `n` dimension of the street grid
 * @param[out] colCapacities
 *    Vector containing the capacities of the column streets.
 *    The size of this vector is the `m` dimension of the street grid
 *
 * @throws std::logic_error for certain format errors
 */
void readConfig(std::istream& istr,
                std::vector<unsigned int>& rowCapacities,
                std::vector<unsigned int>& colCapacities);
/**
 * Reads the add and blockage events from the configuration file
 *
 * @pre This function should be called after readConfig() without any
 *      changes to the istr stream between the calls
 *
 * @param[in] istr
 *    Input stream representing the configuration file
 * @param[in] n
 *    Number of row streets
 * @param[in] m
 *    Number of column streets
 * @param[inout] streets
 *    StreetGrid object to be passed to events when they are created
 *    and to set the expected number of added vehicles.
 * @returns List of dynamically allocated events to be processed by the
 *    simulation
 *
 * @throws std::logic_error for certain format errors
 */
EventList readEvents(std::istream& istr,
                     size_t n, size_t m,
                     StreetGrid& streets);

#endif
