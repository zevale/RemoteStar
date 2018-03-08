#ifndef MESHVALUES_H
#define MESHVALUES_H


#include <vector>
#include <string>
#include "MightyTypes.h"
#include "MightyConstants.h"

class MeshValues {
private:
    double              baseSize;        // Base size                [m]
    PrismLayer          prismLayer;      // Prism layer mesh values  [c.f. MightyTypes.h]
    std::vector<double> surfaceSize;     // Boundaries > Mesh Values [m] {aircraftSurfaceSize, domain1, domain2, ...}

public:
    // Constructor
    explicit MeshValues(double                     _baseSize   = Default::scalar,
                        const PrismLayer&          _prismLayer = {Default::boundaryMarchAngle,
                                                                  Default::minimumThickness,
                                                                  Default::layerChoppingPercentage,
                                                                  Default::scalar,
                                                                  Default::scalar,
                                                                  Default::scalar},
                        const std::vector<double>& _surfaceSize = {0.05, 3, 3, 3, 3});

    // Member function
    std::vector<std::string> meshValuesCode();
};
#endif //MESHVALUES_H