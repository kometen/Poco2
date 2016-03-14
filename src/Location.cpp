/*
 * Location.cpp
 *
 *  Created on: 14/03/2016
 *      Author: claus
 */

#include "Location.hpp"

Location::Location(std::string name, std::string latitude, std::string longitude) :
   _measurementSiteName {name}, _latitude {latitude}, _longitude {longitude} {}

Location::~Location() {}

std::string Location::measurementSiteName() {
    return _measurementSiteName;
}

std::string Location::latitude() {
    return _latitude;
}

std::string Location::longitude() {
    return _longitude;
}
