/*
 * CLASS PhysicsValues
 * Initial and boundary conditions
 *
 * DESCRIPTION
 *
 *
 *          Creator: Nuno Alves de Sousa
 *           E-mail: nunoalvesdesousa@me.com
 *
 */
#ifndef PHYSICSVALUES_H
#define PHYSICSVALUES_H

#include <vector>
#include <string>
#include "MightyTypes.h"
#include "MightyConstants.h"

class PhysicsValues {
private:
    double                   machNumber;
    double                   dynamicViscosity;
    double                   referencePressure;
    double                   staticTemperature;
    FlowDirection            flowDirection;
    Velocity                 velocity;
    std::vector<std::string> regionName;
    std::vector<std::string> boundaryCondition;
public:
    // Constructor
    explicit PhysicsValues(double                          _machNumber        = Default::scalar,
                           double                          _dynamicViscosity  = Default::scalar,
                           double                          _referencePressure = Default::scalar,
                           double                          _staticTemperature = Default::scalar,
                           const FlowDirection&            _flowDirection     = {Default::scalar,
                                                                                 Default::scalar,
                                                                                 Default::scalar},
                           const Velocity&                 _velocity          = {Default::scalar,
                                                                                 Default::scalar,
                                                                                 Default::scalar},
                           const std::vector<std::string>& _regionName        = {},
                           const std::vector<std::string>& _boundaryCondition = {});

    // Code generator
    std::vector<std::string> physicsValuesCode();
};
#endif //PHYSICSVALUES_H
