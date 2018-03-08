#include "MightyMath.h"
#include <sstream>
#include <iomanip>
#include <algorithm>
#include "MightyConstants.h"

std::string javaScientific(double _number){
    std::ostringstream scientificStream;
    scientificStream << std::scientific << std::setprecision(Math::precision) << _number;
    std::string scientificStr = scientificStream.str();

    // Remove plus sign from exponent
    scientificStr.erase(std::remove(scientificStr.begin(), scientificStr.end(), '+'), scientificStr.end());

    return scientificStr;
}