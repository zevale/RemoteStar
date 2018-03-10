#include "ShowDomain.h"
ShowDomain::ShowDomain(const std::vector<std::string>& _regionName) :
        regionName(_regionName) {}

std::vector<std::string> ShowDomain::showDomainCode() {
    std::vector<std::string> code;
    std::vector<std::string> codeBuffer;
    code = {
            "",
            "    private void showDomain(){",
            "        Simulation activeSimulation = getActiveSimulation();",
            "",
            "        // New geometry scene",
            "        activeSimulation.getSceneManager().createGeometryScene(\"Air Domain\", \"Outline\", \"Geometry\", 1);",
            "",
            "        // Get the scene object",
            "        Scene sceneObj = activeSimulation.getSceneManager().getScene(\"Air Domain 1\");",
            "",
            "        // Get object",
            "        MeshOperationPart meshOperationPartObj = ((MeshOperationPart) activeSimulation.get(SimulationPartManager.class).getPart(\"Subtract\"));",
            "",
            "        // Select curve parts",
            "        PartCurve partCurveDomainEdges = meshOperationPartObj.getPartCurveManager().getPartCurve(\"Domain.Edges\");",
            "        PartCurve partCurveIntersection = meshOperationPartObj.getPartCurveManager().getPartCurve(\"Intersection\");",
            "",
            "        // Select surface parts"
    };

    // inPutParts will hold all the part curve and part surface objects
    std::string inputParts("partCurveDomainEdges, partCurveIntersection");

    // Select aircraft surface which corresponds to first entry
    code.emplace_back( "        PartSurface partSurface" + regionName[0] + " = ((PartSurface) meshOperationPartObj.getPartSurfaceManager().getPartSurface(\"" + regionName[0] + ".Surface\"));");
    inputParts.append(", partSurface" + regionName[0]);

    // Add surfaces in the domain
    for(int i = 1; i < regionName.size(); i++) {
        code.emplace_back( "        PartSurface partSurface" + regionName[i] + " = ((PartSurface) meshOperationPartObj.getPartSurfaceManager().getPartSurface(\"Domain." + regionName[i] + "\"));");
        inputParts.append(", partSurface" + regionName[i]);
    }

    // Send parts to displayer and apply geometry representation
    codeBuffer = {
            "",
            "        // Send parts to displayer",
            "        PartDisplayer partDisplayerObj = ((PartDisplayer) sceneObj.getDisplayerManager().getDisplayer(\"Geometry 1\"));",
            "        partDisplayerObj.getInputParts().setObjects(" + inputParts + ");",
            "",
            "       // Apply geometry representation",
            "        PartRepresentation partRepresentationObj = ((PartRepresentation) activeSimulation.getRepresentationManager().getObject(\"Geometry\"));",
            "        partDisplayerObj.setRepresentation(partRepresentationObj);",
            "    }",
    };
    code.insert(code.end(), codeBuffer.begin(), codeBuffer.end());

    return code;
}