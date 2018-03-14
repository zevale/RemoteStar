#ifndef SOLUTIONMONITORS_H
#define SOLUTIONMONITORS_H

#include <vector>
#include <string>
#include "MightyTypes.h"
#include "MightyConstants.h"

class SolutionMonitors {
private:
    double                   referenceDensity;     // [kg/m3]
    double                   referenceVelocity;    // [m/s]
    LiftDirection            liftDirection;        // Lift direction   [c.f. MightyTypes.h]
    DragDirection            dragDirection;        // Drag direction   [c.f. MightyTypes.h]
    ViewUp                   viewUp;               // ViewUp direction [c.f. MightyConstants.h]
    Normal                   normal;               // Normal direction [c.f. MightyConstants.h]
    std::vector<std::string> boundaryCondition;
public:
    // Constructor
    explicit SolutionMonitors(double                          _referenceDensity  = Default::scalar,
                              double                          _referenceVelocity = Default::scalar,
                              const LiftDirection&            _liftDirection     = {Default::scalar,
                                                                                    Default::scalar,
                                                                                    Default::scalar},
                              const DragDirection&            _dragDirection     = {Default::scalar,
                                                                                    Default::scalar,
                                                                                    Default::scalar},
                              const ViewUp&                   _viewUp            = {Default::viewUpX,
                                                                                    Default::viewUpY,
                                                                                    Default::viewUpZ},
                              const Normal&                   _normal            = {Default::normalX,
                                                                                    Default::normalY,
                                                                                    Default::normalZ},
                              const std::vector<std::string>& _boundaryCondition = {});

    // Member function
    std::vector<std::string> solutionMonitorsCode();
};
#endif //SOLUTIONMONITORS_H