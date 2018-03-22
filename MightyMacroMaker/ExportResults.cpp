#include "ExportResults.h"

ExportResults::ExportResults(const std::string& _resultsFilePath, double _dynamicViscosity) :
        resultsFilePath(_resultsFilePath),
        dynamicViscosity(_dynamicViscosity) {}

std::vector<std::string> ExportResults::exportResultsCode() {
    std::vector<std::string> code;
    std::vector<std::string> codeBuffer;
    code = {
            "",
            "    private void exportResults(){",
            "        Simulation activeSimulation = getActiveSimulation();",
            "",
            "        // Data",
            "        String saveForcesPath = \"" + resultsFilePath + "\";",
            "",
            "        // Create ForcePressure field function",
            "        UserFieldFunction forcePressureFieldFunction = activeSimulation.getFieldFunctionManager().createFieldFunction();",
            "        forcePressureFieldFunction.getTypeOption().setSelected(FieldFunctionTypeOption.Type.VECTOR);",
            "        forcePressureFieldFunction.setFunctionName(\"ForcePressure\");",
            "        // Set units (1xPressure 2xLength)",
            "        Units unitsPressure = ",
            "                activeSimulation.getUnitsManager().getPreferredUnits(new IntVector(new int[] {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}));",
            "        Units unitsArea = ",
            "                activeSimulation.getUnitsManager().getPreferredUnits(new IntVector(new int[] {0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}));",
            "        // Function definition and presentation name",
            "        forcePressureFieldFunction.setDefinition(\"${Pressure}*$${Area}\");",
            "        forcePressureFieldFunction.setPresentationName(\"Force (Pressure)\");",
    };

    // Force friction
    if(dynamicViscosity > 0) {
        codeBuffer = {
                "",
                "        // Create ForceFriction field function",
                "        UserFieldFunction forceFrictionFieldFunction = activeSimulation.getFieldFunctionManager().createFieldFunction();",
                "        forceFrictionFieldFunction.getTypeOption().setSelected(FieldFunctionTypeOption.Type.VECTOR);",
                "        forceFrictionFieldFunction.setFunctionName(\"ForceFriction\");",
                "        forceFrictionFieldFunction.setDefinition(\"$${WallShearStress}*$$Area.mag()\");",
                "        forceFrictionFieldFunction.setPresentationName(\"Force (Friction)\");"
        };
        code.insert(code.end(), codeBuffer.begin(), codeBuffer.end());
    }

    // ForceSum (= ForcePressure for inviscid, = ForcePressure + ForceFriction for RANS)
    codeBuffer = {
            "",
            "        // Create ForceSum field function",
            "        UserFieldFunction forceSumFieldFunction = activeSimulation.getFieldFunctionManager().createFieldFunction();",
            "        forceSumFieldFunction.getTypeOption().setSelected(FieldFunctionTypeOption.Type.VECTOR);",
            "        forceSumFieldFunction.setFunctionName(\"ForceSum\");",
            "        // Set units (can be left blank because we haven't defined it",
            "        Units unitsPressure2 = ",
            "                activeSimulation.getUnitsManager().getPreferredUnits(new IntVector(new int[] {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}));",
            "        // Function definition and presentation name",
            "        forceSumFieldFunction.setDefinition(\"$${ForcePressure}" + std::string((dynamicViscosity > 0)? " + $${ForceFriction}" : "") + "\");",
            "        forceSumFieldFunction.setPresentationName(\"Force (Sum)\");",
            "",
            "        // Create XYZ Internal table",
            "        XyzInternalTable forceTable = activeSimulation.getTableManager().createTable(XyzInternalTable.class);",
            "",
            "        // Get all components",
            "        VectorComponentFieldFunction xComponentForceSumFieldFunction =",
            "                ((VectorComponentFieldFunction) forceSumFieldFunction.getComponentFunction(0));",
            "        VectorComponentFieldFunction yComponentForceSumFieldFunction = ",
            "                ((VectorComponentFieldFunction) forceSumFieldFunction.getComponentFunction(1));",
            "        VectorComponentFieldFunction zComponentForceSumFieldFunction = ",
            "                ((VectorComponentFieldFunction) forceSumFieldFunction.getComponentFunction(2));",
            "        // Add field functions to the table",
            "        forceTable.setFieldFunctions(new NeoObjectVector(new Object[] {xComponentForceSumFieldFunction, yComponentForceSumFieldFunction, zComponentForceSumFieldFunction}));",
            "",
            "        // Select parts for the table (aircraft)",
            "        forceTable.getParts().setQuery(null);",
            "        Region regionAir = ",
            "                activeSimulation.getRegionManager().getRegion(\"Air\");",
            "        Boundary aircraftBoundary = ",
            "                regionAir.getBoundaryManager().getBoundary(\"Subtract.SurfMesh.Surface\");",
            "        forceTable.getParts().setObjects(aircraftBoundary);",
            "",
            "        // Set presentation name",
            "",
            "        forceTable.setPresentationName(\"Force Table\");",
            "",
            "        // Extract values and export (string delimiter = \",\")",
            "        forceTable.extract();",
            "        forceTable.export(saveForcesPath , \",\");",
            "    }"
    };
    code.insert(code.end(), codeBuffer.begin(), codeBuffer.end());

    return code;
}