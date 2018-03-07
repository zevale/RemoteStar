#ifndef SOLVEROPTIONS_H
#define SOLVEROPTIONS_H


#include <vector>
#include <string>
#include "MightyConstants.h"

class SolverOptions {
private:
    double CFL;
public:
    // Constructor
    explicit SolverOptions(double _CFL = Default::scalar);

    // Code generator
    std::vector<std::string> solverOptionsCode();
};
#endif //SOLVEROPTIONS_H