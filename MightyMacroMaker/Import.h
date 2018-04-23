/*
 * CLASS Import
 *
 * DESCRIPTION
 * Generates the macro imports which depend on the mesh and physics models
 *
 *          Creator: Nuno Alves de Sousa
 *           E-mail: nunoalvesdesousa@me.com
 */
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

    // Member function
    std::vector<std::string> importCode();
};
#endif //IMPORT_H
