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
public:
    // Constructor
    explicit VolumetricControls(const Block& _block = {}, const Cylinder& _cylinder = {});

    // Getters
    Block getBlock() const;

    std::vector<std::string> volumetricControlsCode();
};

#endif //VOLUMETRICCONTROLS_H
