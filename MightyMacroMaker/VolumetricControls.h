//
// Created by Nuno on 3/15/2018.
//

#ifndef VOLUMETRICCONTROLS_H
#define VOLUMETRICCONTROLS_H

#include "MightyTypes.h"

#include <vector>
#include <string>

class VolumetricControls {
private:
    Block    block;
    Cylinder cylinder;
    Cone     cone;
public:
    // Constructor
    explicit VolumetricControls(const Block& _block = {}, const Cylinder& _cylinder = {}, const Cone& _cone = {});

    // Getters
    Block    getBlock()    const;
    Cylinder getCylinder() const;
    Cone     getCone()     const;

    // Member function
    std::vector<std::string> volumetricControlsCode();
};

#endif //VOLUMETRICCONTROLS_H
