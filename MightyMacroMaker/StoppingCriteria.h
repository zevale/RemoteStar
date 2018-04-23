/*
 * CLASS StoppingCriteria
 *
 * DESCRIPTION
 * Macro stopping criteria: max steps (and asymptotic criteria)
 *
 *          Creator: Nuno Alves de Sousa
 *           E-mail: nunoalvesdesousa@me.com
 *
 */
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
    double asymptoticCD;
public:
    // Constructor
    explicit StoppingCriteria(int    _maximumSteps = Default::scalar,
                              int    _numSamples   = Default::scalar,
                              double _asymptoticCL = Default::asymptoticCL,
                              double _asymptoticCD = Default::asymptoticCD);

    // Member function
    std::vector<std::string> stoppingCriteriaCode();
};
#endif //STOPPINGCRITERIA_H
