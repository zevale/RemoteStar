#ifndef MESHVALUES_H
#define MESHVALUES_H


#include <vector>
#include <string>
#include "MightyTypes.h"
#include "MightyConstants.h"

class MeshValues {
private:
    double                   baseSize;          // Base size                [m]
    PrismLayer               prismLayer;        // Prism layer mesh values  [c.f. MightyTypes.h]
    std::vector<double>      surfaceSize;       // Boundaries > Mesh Values [m] {aircraftSurfaceSize, ...}
    std::vector<std::string> regionName;
    std::vector<std::string> boundaryCondition;

public:
    // Constructor
    explicit MeshValues(double                          _baseSize          = Default::scalar,
                        const PrismLayer&               _prismLayer        = {Default::boundaryMarchAngle,
                                                                              Default::minimumThickness,
                                                                              Default::layerChoppingPercentage,
                                                                              Default::scalar,
                                                                              Default::scalar,
                                                                              Default::scalar},
                        const std::vector<double>&      _surfaceSize       = {0.05, 3, 3, 3, 3},
                        const std::vector<std::string>& _regionName        = {},
                        const std::vector<std::string>& _boundaryCondition = {});

    // Member function
    std::vector<std::string> meshValuesCode();
};
#endif //MESHVALUES_H