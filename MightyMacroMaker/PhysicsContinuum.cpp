#include "PhysicsContinuum.h"

PhysicsContinuum::PhysicsContinuum(double _dynamicViscosity) : dynamicViscosity(_dynamicViscosity) {}

std::vector<std::string> PhysicsContinuum::physicsContinuumCode() {
    std::vector<std::string> code;
    std::vector<std::string> codeBuffer;
    code = {
            "",
            "    private void physicsContinuum(){",
            "        Simulation activeSimulation = getActiveSimulation();",
            "",
            "        // Create physics continuum to define physics",
            "        PhysicsContinuum physicsContinuumObj = activeSimulation.getContinuumManager().createContinuum(PhysicsContinuum.class);",
            "",
            "        // Physics",
            "        physicsContinuumObj.enable(ThreeDimensionalModel.class);",
            "        physicsContinuumObj.enable(SteadyModel.class);",
            "        physicsContinuumObj.enable(SingleComponentGasModel.class);",
            "        physicsContinuumObj.enable(CoupledFlowModel.class);",
            "        physicsContinuumObj.enable(IdealGasModel.class);",
            "        physicsContinuumObj.enable(CoupledEnergyModel.class);",
    };

    // Inviscid
    if(dynamicViscosity < 0) {
        codeBuffer = {
                "        physicsContinuumObj.enable(InviscidModel.class);",
                "    }"
        };
        //RANS
    } else {
        codeBuffer = {
                "        physicsContinuumObj.enable(TurbulentModel.class);",
                "        physicsContinuumObj.enable(RansTurbulenceModel.class);",
                "        physicsContinuumObj.enable(KOmegaTurbulence.class);",
                "        physicsContinuumObj.enable(SstKwTurbModel.class);",
                "        physicsContinuumObj.enable(KwAllYplusWallTreatment.class);",
                "    }"
        };
    }
    code.insert(code.end(), codeBuffer.begin(), codeBuffer.end());

    return code;
}
