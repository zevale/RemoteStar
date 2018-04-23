/*
 * CLASS CloseSim
 *
 * DESCRIPTION
 * Save simulation and close
 *
 *          Creator: Nuno Alves de Sousa
 *           E-mail: nunoalvesdesousa@me.com
 *
 */
#ifndef CLOSESIM_H
#define CLOSESIM_H

#include <vector>
#include <string>
#include "MightyConstants.h"

class CloseSim {
private:
    std::string simFilePath;
public:
    // Constructor
    explicit CloseSim(const std::string& _simFilePath = Default::filePath);

    // Member function
    std::vector<std::string> closeSimCode();
};
#endif //CLOSESIM_H
