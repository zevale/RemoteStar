#include "ShowDomain.h"

std::vector<std::string> ShowDomain::showDomainCode() {
    std::vector<std::string> code;
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
            "        // Select parts",
            "        MeshOperationPart meshOperationPartObj = ((MeshOperationPart) activeSimulation.get(SimulationPartManager.class).getPart(\"Subtract\"));",
            "        PartCurve partCurveDomainEdges = meshOperationPartObj.getPartCurveManager().getPartCurve(\"Domain.Edges\");",
            "        PartCurve partCurveIntersection = meshOperationPartObj.getPartCurveManager().getPartCurve(\"Intersection\");",
            "        PartSurface partSurfaceFarSide = ((PartSurface) meshOperationPartObj.getPartSurfaceManager().getPartSurface(\"Domain.FarSide\"));",
            "        PartSurface partSurfaceInlet = ((PartSurface) meshOperationPartObj.getPartSurfaceManager().getPartSurface(\"Domain.Inlet\"));",
            "        PartSurface partSurfaceOutlet = ((PartSurface) meshOperationPartObj.getPartSurfaceManager().getPartSurface(\"Domain.Outlet\"));",
            "        PartSurface partSurfaceSym = ((PartSurface) meshOperationPartObj.getPartSurfaceManager().getPartSurface(\"Domain.Sym\"));",
            "        PartSurface partSurfaceAircraft = ((PartSurface) meshOperationPartObj.getPartSurfaceManager().getPartSurface(\"SurfMesh.Surface\"));",
            "",
            "        // Send parts to displayer",
            "        PartDisplayer partDisplayerObj = ((PartDisplayer) sceneObj.getDisplayerManager().getDisplayer(\"Geometry 1\"));",
            "        partDisplayerObj.getInputParts().setObjects(partCurveDomainEdges, partCurveIntersection, partSurfaceFarSide, partSurfaceInlet, partSurfaceOutlet, partSurfaceSym, partSurfaceAircraft);",
            "",
            "       // Apply geometry representation",
            "        PartRepresentation partRepresentationObj = ((PartRepresentation) activeSimulation.getRepresentationManager().getObject(\"Geometry\"));",
            "        partDisplayerObj.setRepresentation(partRepresentationObj);",
            "    }",
    };
    return code;
}