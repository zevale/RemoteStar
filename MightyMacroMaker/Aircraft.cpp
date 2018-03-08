#include "Aircraft.h"

Aircraft::Aircraft(const std::string & _geometryFilePath) : geometryFilePath(_geometryFilePath) {}

std::vector<std::string> Aircraft::aircraftCode() {
    std::vector<std::string> code;
    code = {
            "",
            "    private void importAircraftGeometry(){",
            "        Simulation activeSimulation = getActiveSimulation();",
            "",
            "        // Data",
            "        String aircraftGeometryFile = \"" + geometryFilePath + "\";",
            "",
            "        // Get default length units",
            "        Units units = activeSimulation.getUnitsManager().getPreferredUnits(new IntVector(new int[] {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}));",
            "",
            "        // Import aircraft geometry",
            "        PartImportManager partImportManager = ",
            "                activeSimulation.get(PartImportManager.class);",
            "        partImportManager.importStlPart(resolvePath(aircraftGeometryFile), \"OneSurfacePerPatch\", units, true, 1.0E-5);",
            "",
            "        // Optional: create geometry scene",
            "        activeSimulation.getSceneManager().createGeometryScene(\"Aircraft Geometry\", \"Outline\", \"Geometry\", 1);",
            "    }"
    };
    return code;
}