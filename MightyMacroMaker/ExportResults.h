#ifndef EXPORTRESULTS_H
#define EXPORTRESULTS_H

#include <vector>
#include <string>
#include "MightyConstants.h"

class ExportResults {
private:
    std::string resultsFilePath;
    double      dynamicViscosity;
public:
    // Constructor
    explicit ExportResults(const std::string& _resultsFilePath  = Default::filePath,
                           double             _dynamicViscosity = {});

    std::vector<std::string> exportResultsCode();
};
#endif //EXPORTRESULTS_H
