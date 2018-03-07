#include "MeshContinuum.h"

std::vector<std::string> MeshContinuum::meshContinuumCode() {
    std::vector<std::string> code;
    code = {
            "",
            "    private void meshContinuum(){",
            "        Simulation activeSimulation = getActiveSimulation();",
            "",
            "        // Create mesh continuum to define mesh models",
            "        MeshContinuum meshContinuumObj = activeSimulation.getContinuumManager().createContinuum(MeshContinuum.class);",
            "",
            "        // Mesh models",
            "        meshContinuumObj.enable(ResurfacerMeshingModel.class);",
            "        meshContinuumObj.enable(SurfaceWrapperMeshingModel.class);",
            "        meshContinuumObj.enable(DualMesherModel.class);",
            "        meshContinuumObj.enable(PrismMesherModel.class);",
            "    }"
    };
    return code;
}