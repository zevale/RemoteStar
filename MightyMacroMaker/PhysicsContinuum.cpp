#include "PhysicsContinuum.h"

std::vector<std::string> PhysicsContinuum::physicsContinuumCode() {
    std::vector<std::string> code;
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
            "        physicsContinuumObj.enable(TurbulentModel.class);",
            "        physicsContinuumObj.enable(RansTurbulenceModel.class);",
            "        physicsContinuumObj.enable(KOmegaTurbulence.class);",
            "        physicsContinuumObj.enable(SstKwTurbModel.class);",
            "        physicsContinuumObj.enable(KwAllYplusWallTreatment.class);",
            "    }"
    };
    return code;
}