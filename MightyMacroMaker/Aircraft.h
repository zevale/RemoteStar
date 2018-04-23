/*
 * CLASS Aircraft
 *
 * DESCRIPTION
 * Holds the file path of the aircraft geometry and generates
 * the geometry import code accordingly.
 *
 *          Creator: Nuno Alves de Sousa
 *           E-mail: nunoalvesdesousa@me.com
 *
 */
#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include <string>
#include <vector>
#include "MightyConstants.h"

class Aircraft {
private:
    std::string geometryFilePath;
public:
    // Constructor
    explicit Aircraft(const std::string& _geometryFilePath = Default::filePath);

    // Member function
    std::vector<std::string> aircraftCode();
};
#endif //AIRCRAFT_H
