#include "MeshContinuum.h"

MeshContinuum::MeshContinuum(const PrismLayer& _prismLayer) : prismLayer(_prismLayer) {}

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
            "        meshContinuumObj.enable(DualMesherModel.class);"
    };

    // Enable prism mesher
    if(prismLayer.numPrismLayers > 0)
        code.emplace_back("        meshContinuumObj.enable(PrismMesherModel.class);");
    code.emplace_back("    }");

    return code;
}