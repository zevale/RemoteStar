#ifndef PHYSICSCONTINUUM_H
#define PHYSICSCONTINUUM_H


#include <vector>
#include <string>

class PhysicsContinuum {
private:
    double dynamicViscosity;
public:
    // Constructor
    explicit PhysicsContinuum(double _dynamicViscosity = {});

    std::vector<std::string> physicsContinuumCode();
};
#endif //PHYSICSCONTINUUM_H