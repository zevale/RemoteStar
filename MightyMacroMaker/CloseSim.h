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

    std::vector<std::string> closeSimCode();
};
#endif //CLOSESIM_H
