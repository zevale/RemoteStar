#ifndef STOPPINGCRITERIA_H
#define STOPPINGCRITERIA_H


#include <vector>
#include <string>
#include "MightyConstants.h"

class StoppingCriteria {
private:
    int    maximumSteps;
    int    numSamples;
    double asymptoticCL;
public:
    // Constructor
    explicit StoppingCriteria(int    _maximumSteps      = Default::scalar,
                              int    _numSamples = Default::scalar,
                              double _asymptoticCL      = Default::scalar);

    std::vector<std::string> stoppingCriteriaCode();
};
#endif //STOPPINGCRITERIA_H
