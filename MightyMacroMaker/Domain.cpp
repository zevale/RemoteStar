#include "Domain.h"

Domain::Domain(const std::string& _geometryFilePath) : geometryFilePath (_geometryFilePath) {}

std::vector<std::string> Domain::domainCode() {
    std::vector<std::string> code;
    code = {
            "",
            "    private void importDomainGeometry(){",
            "        Simulation activeSimulation = getActiveSimulation();",
            "",
            "        // Data",
            "        String domainGeometryFile = \"" + geometryFilePath + "\";",
            "",
            "        // Import domain geometry",
            "        PartImportManager partImportManager = ",
            "               activeSimulation.get(PartImportManager.class);",
            "        partImportManager.importCadPart(resolvePath(domainGeometryFile), \"SharpEdges\", 30.0, 2, true, 1.0E-5, true, false, false, false);",
            "",
            "        // Optional: create geometry scene",
            "        activeSimulation.getSceneManager().createGeometryScene(\"Domain\", \"Outline\", \"Geometry\", 1);",
            "    }",
            "",
            "    private void generateAirDomain(){",
            "        Simulation activeSimulation = getActiveSimulation();",
            "",
            "        // Imprint operation",
            "        MeshPart meshPartAircraft = ((MeshPart) activeSimulation.get(SimulationPartManager.class).getPart(\"SurfMesh\"));",
            "        CadPart cadPartDomain = ((CadPart) activeSimulation.get(SimulationPartManager.class).getPart(\"Domain\"));",
            "        ImprintPartsOperation imprintAircraftDomain =  (ImprintPartsOperation) activeSimulation.get(MeshOperationManager.class).createImprintPartsOperation(new NeoObjectVector(new Object[] {cadPartDomain, meshPartAircraft}));",
            "        imprintAircraftDomain.execute();",
            "",
            "        // Subtract operation",
            "        SubtractPartsOperation subtractAircraftDomain = (SubtractPartsOperation) activeSimulation.get(MeshOperationManager.class).createSubtractPartsOperation(new NeoObjectVector(new Object[] {cadPartDomain, meshPartAircraft}));",
            "        subtractAircraftDomain.setTargetPart(cadPartDomain);",
            "        subtractAircraftDomain.setPerformCADBoolean(false);",
            "        subtractAircraftDomain.execute();"
            "    }"
    };
    return code;
}
