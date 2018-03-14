#ifndef MESHCONTINUUM_H
#define MESHCONTINUUM_H


#include <vector>
#include <string>
#include "MightyTypes.h"
#include "MightyConstants.h"

class MeshContinuum {
private:
    PrismLayer prismLayer;
public:
    // Constructor
    explicit MeshContinuum(const PrismLayer& _prismLayer = {Default::boundaryMarchAngle,
                                                            Default::minimumThickness,
                                                            Default::layerChoppingPercentage,
                                                            Default::scalar,
                                                            Default::scalar,
                                                            Default::scalar});

    std::vector<std::string> meshContinuumCode();
};
#endif //MESHCONTINUUM_H