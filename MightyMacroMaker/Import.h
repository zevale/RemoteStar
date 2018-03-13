#ifndef IMPORT_H
#define IMPORT_H

#include "MightyTypes.h"
#include "MightyConstants.h"

#include <vector>
#include <string>

class Import {
private:
    PrismLayer prismLayer;
public:
    // Constructor
    explicit Import(const PrismLayer& _prismLayer = {Default::boundaryMarchAngle,
                                                     Default::minimumThickness,
                                                     Default::layerChoppingPercentage,
                                                     Default::scalar,
                                                     Default::scalar,
                                                     Default::scalar});

    std::vector<std::string> importCode();
};
#endif //IMPORT_H
