/*
 * Location.hpp
 *
 *  Created on: 14/03/2016
 *      Author: claus
 */

#ifndef LOCATION_HPP_
#define LOCATION_HPP_

#include <string>

class Location {
private:
    std::string _measurementSiteName {};
    std::string _latitude {};
    std::string _longitude {};

public:
    Location(std::string, std::string, std::string);
    std::string measurementSiteName();
    std::string latitude();
    std::string longitude();
    virtual ~Location();
};

#endif /* LOCATION_HPP_ */
