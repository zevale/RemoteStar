#include "MightyScene.h"

MightyScene::MightyScene(const std::vector<std::string>& _regionName) :
        regionName(_regionName) {}

std::vector<std::string> MightyScene::mightySceneCode() {
    std::vector<std::string> code;
    std::vector<std::string> codeBuffer;

    code = {
            "",
            "    private void createScenes(){",
            "        Simulation activeSimulation = getActiveSimulation();",
            "",
            "        // Domain",
            "        Region regionAir = activeSimulation.getRegionManager().getRegion(\"Air\");"
    };

    // Domain regions will hold the domain parts
    std::string domainBoundaries = "boundary" + regionName[1];
    for(int i = 1; i < regionName.size(); i++){
        code.emplace_back("        Boundary boundary" + regionName[i] + " = regionAir.getBoundaryManager().getBoundary(\"Subtract.Domain." + regionName[i] + "\");");
        if(i > 1)
            domainBoundaries.append(", boundary" + regionName[i]);
    }

    // Mach number
    codeBuffer = {
            "        ",
            "        // MACH NUMBER",
            "",
            "        // Create new scene",
            "        activeSimulation.getSceneManager().createScalarScene(\"Mach Number\", \"Outline\", \"Scalar\");",
            "        Scene sceneObj = activeSimulation.getSceneManager().getScene(\"Mach Number 1\");",
            "        sceneObj.initializeAndWait();",
            "        PartDisplayer partDisplayerMachNumberObj = ((PartDisplayer) sceneObj.getDisplayerManager().getDisplayer(\"Outline 1\"));",
            "        partDisplayerMachNumberObj.initialize();",
            "        ScalarDisplayer scalarDisplayerMachNumberObj = ((ScalarDisplayer) sceneObj.getDisplayerManager().getDisplayer(\"Scalar 1\"));",
            "        scalarDisplayerMachNumberObj.initialize();",
            "        // Input parts",
            "        scalarDisplayerMachNumberObj.getInputParts().setQuery(null);",
            "        scalarDisplayerMachNumberObj.getInputParts().setObjects(" + domainBoundaries + ");",
            "        // Select scalar",
            "        PrimitiveFieldFunction primitiveFieldFunctionMachNumberObj = ((PrimitiveFieldFunction) activeSimulation.getFieldFunctionManager().getFunction(\"MachNumber\"));",
            "        scalarDisplayerMachNumberObj.getScalarDisplayQuantity().setFieldFunction(primitiveFieldFunctionMachNumberObj);",
            "        // Smooth filled",
            "        scalarDisplayerMachNumberObj.setFillMode(ScalarFillMode.NODE_FILLED);",
            "        // Apply volume mesh representation",
            "        FvRepresentation fvRepresentationObj =  ((FvRepresentation) activeSimulation.getRepresentationManager().getObject(\"Volume Mesh\"));",
            "        partDisplayerMachNumberObj.setRepresentation(fvRepresentationObj);",
            "        scalarDisplayerMachNumberObj.setRepresentation(fvRepresentationObj);"
    };
    code.insert(code.end(), codeBuffer.begin(), codeBuffer.end());

    // Pressure coefficient
    codeBuffer = {
            "        ",
            "        // PRESSURE COEFFICIENT",
            "        // Boundaries",
            "        Boundary boundaryAircraft = regionAir.getBoundaryManager().getBoundary(\"Subtract." + regionName[0] + ".Surface\");",
            "        // New displayer",
            "        ScalarDisplayer scalarDisplayerCpObj =  sceneObj.getDisplayerManager().createScalarDisplayer(\"Scalar\");",
            "        scalarDisplayerCpObj.initialize();",
            "        scalarDisplayerCpObj.setPresentationName(\"Cp\");",
            "        // Input parts",
            "        scalarDisplayerCpObj.getInputParts().setQuery(null);",
            "        scalarDisplayerCpObj.getInputParts().setObjects(boundaryAircraft);",
            "        // Select scalar",
            "        PressureCoefficientFunction pressureCoefficientFunctionObj = ((PressureCoefficientFunction) activeSimulation.getFieldFunctionManager().getFunction(\"PressureCoefficient\"));",
            "        scalarDisplayerCpObj.getScalarDisplayQuantity().setFieldFunction(pressureCoefficientFunctionObj);",
            "        // Smooth filled",
            "        scalarDisplayerCpObj.setFillMode(ScalarFillMode.NODE_FILLED);",
            "    }"
    };
    code.insert(code.end(), codeBuffer.begin(), codeBuffer.end());

    return code;
}
