#ifndef IMPORT_H
#define IMPORT_H

#include "MightyTypes.h"
#include "MightyConstants.h"

#include <vector>
#include <string>

class Import {
private:
    PrismLayer prismLayer;
    double     dynamicViscosity;
public:
    // Constructor
    explicit Import(const PrismLayer& _prismLayer       = {},
                    double            _dynamicViscosity = {});

    std::vector<std::string> importCode();
};
#endif //IMPORT_H
