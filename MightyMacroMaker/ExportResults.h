/*
 * CLASS ExportResults
 *
 * DESCRIPTION
 * Export the sum of pressure and friction forces on cell centers as .csv file
 *
 *          Creator: Nuno Alves de Sousa
 *           E-mail: nunoalvesdesousa@me.com
 *
 */
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

    // Member function
    std::vector<std::string> exportResultsCode();
};
#endif //EXPORTRESULTS_H
