#include "MeshValues.h"
#include "MightyMath.h"
#include "MightyConstants.h"

#include <iostream>

MeshValues::MeshValues(double                          _baseSize,
                       const PrismLayer&               _prismLayer,
                       const std::vector<double>&      _surfaceSize,
                       const std::vector<std::string>& _regionName,
                       const std::vector<std::string>& _boundaryCondition) :
        baseSize         (_baseSize),
        prismLayer       (_prismLayer),
        surfaceSize      (_surfaceSize),
        regionName       (_regionName),
        boundaryCondition(_boundaryCondition){}

// Precision: 6 decimal places (check if this is ok)
std::vector<std::string> MeshValues::meshValuesCode(){
    std::vector<std::string> code;
    std::vector<std::string> codeBuffer;

    code = {
            "",
            "    private void meshValues(){",
            "        Simulation activeSimulation = getActiveSimulation();",
            "",
            "        // Data",
            "        double valueBaseSize                = " + javaScientific(baseSize) + ";"
    };

    // Prism layer data
    codeBuffer = {
            "",
            "        // Prism layer",
            "        double valueBoundaryMarchAngle      = " + javaScientific(prismLayer.boundaryMarchAngle) + ";",
            "        double valueMinimumThickness        = " + javaScientific(prismLayer.minimumThickness) + ";",
            "        double valueLayerChoppingPercentage = " + javaScientific(prismLayer.layerChoppingPercentage) + ";",
            "        int    valueNumPrismLayers          = " + std::to_string(prismLayer.numPrismLayers) + ";",
            "        double valuePrismLayerThickness     = " + javaScientific(prismLayer.prismLayerThickness) + ";",
            "        double valueNearWallThickness       = " + javaScientific(prismLayer.nearWallThickness) + ";"
    };
    code.insert(code.end(), codeBuffer.begin(), codeBuffer.end());

    // Surface sizing
    codeBuffer = {
            "",
            "        // Surface sizing"
    };
    code.insert(code.end(), codeBuffer.begin(), codeBuffer.end());
    std::string line;
    for(int i = 0; i < regionName.size(); ++i){
        // Generate variable for the region size
        line = std::string( "        double valueMinimumTarget") + regionName[i];

        // If the region name is shorter than 10 characters add white spaces to align the equals sign
        if(regionName[i].size() < 10)
            line.append(10 - regionName[i].size(), ' ');

        // Assign correct surface value
        line.append(" = " + javaScientific(surfaceSize[i]) + ";");
        code.emplace_back(line);
    }

    // Base size
    codeBuffer = {
            "",
            "        // Get the mesh continuum object",
            "        MeshContinuum meshContinuumObj = ((MeshContinuum) activeSimulation.getContinuumManager().getContinuum(\"Mesh 1\"));",
            "",
            "        // Base size",
            "        meshContinuumObj.getReferenceValues().get(BaseSize.class).setValue(valueBaseSize);"
    };
    code.insert(code.end(), codeBuffer.begin(), codeBuffer.end());

    // Parallel meshing
    codeBuffer = {
            "",
            "        // Set parallel meshing",
            "        meshContinuumObj.setMeshInParallel(true);"
    };
    code.insert(code.end(), codeBuffer.begin(), codeBuffer.end());

    // Prism mesher
    codeBuffer = {
            "",
            "        // PRISM MESHER",
            "        // Model",
            "        PrismMesherModel prismMesherModelObj = meshContinuumObj.getModelManager().getModel(PrismMesherModel.class);",
            "        prismMesherModelObj.setBoundaryMarchAngle(valueBoundaryMarchAngle);",
            "        prismMesherModelObj.setMinimumThickness(valueMinimumThickness);",
            "        prismMesherModelObj.setLayerChoppingPercentage(valueLayerChoppingPercentage);",
            "        prismMesherModelObj.getPrismStretchingOption().setSelected(PrismStretchingOption.Type.WALL_THICKNESS);",
            "        // Number of prism layers",
            "        NumPrismLayers numPrismLayersObj = meshContinuumObj.getReferenceValues().get(NumPrismLayers.class);",
            "        numPrismLayersObj.setNumLayers(valueNumPrismLayers);",
            "        // Prism layer thickness and near wall thickness",
            "        PrismThickness prismThicknessObj = meshContinuumObj.getReferenceValues().get(PrismThickness.class);",
            "        prismThicknessObj.getRelativeOrAbsoluteOption().setSelected(RelativeOrAbsoluteOption.Type.ABSOLUTE);",
            "        ((ScalarPhysicalQuantity) prismThicknessObj.getAbsoluteSizeValue()).setValue(valuePrismLayerThickness);",
            "        meshContinuumObj.getReferenceValues().get(PrismWallThickness.class).setValue(valueNearWallThickness);"
    };
    code.insert(code.end(), codeBuffer.begin(), codeBuffer.end());

    // Boundary conditions
    codeBuffer = {
            "",
            "        // TYPE OF BOUNDARY CONDITIONS",
            "",
            "        // Get the region object",
            "        Region regionAir = activeSimulation.getRegionManager().getRegion(\"Air\");"
    };
    code.insert(code.end(), codeBuffer.begin(), codeBuffer.end());

    // Assign wall boundary to aircraft and declare free stream boundary object
    codeBuffer = {
            "",
            "        // WALL",
            "        // Wall type is default but is explicitly enforced here for consistency",
            "        WallBoundary wallBoundaryObj = ((WallBoundary) activeSimulation.get(ConditionTypeManager.class).get(WallBoundary.class));",
            "        // Aircraft is the first entry in regions",
            "        Boundary boundary" + regionName[0] + " = regionAir.getBoundaryManager().getBoundary(\"Subtract." +
                    regionName[0] + ".Surface\");",
            "        boundary" + regionName[0] + ".setBoundaryType(wallBoundaryObj);",
            "",
            "        // FREE STREAM",
            "        FreeStreamBoundary freeStreamBoundaryObj = ((FreeStreamBoundary) activeSimulation.get(ConditionTypeManager.class).get(FreeStreamBoundary.class));"
    };
    code.insert(code.end(), codeBuffer.begin(), codeBuffer.end());

    // Assign free stream boundaries and search for one symmetry plane (not required)
    for(int i = 1; i < boundaryCondition.size(); i++){
        // Free stream
        if(boundaryCondition[i] == "freeStream"){
            codeBuffer = {
                    "        // " + regionName[i],
                    "        Boundary boundary" + regionName[i] +
                            " = regionAir.getBoundaryManager().getBoundary(\"Subtract.Domain." +
                            regionName[i] + "\");",
                    "        boundary" + regionName[i] + ".setBoundaryType(freeStreamBoundaryObj);"
            };
            // Symmetry plane
        } else {
            codeBuffer = {
                    "",
                    "        // SYMETRY PLAME",
                    "        SymmetryBoundary symmetryBoundaryObj = ((SymmetryBoundary) activeSimulation.get(ConditionTypeManager.class).get(SymmetryBoundary.class));",
                    "        // " + regionName[i],
                    "        Boundary boundary" + regionName[i] +
                            " = regionAir.getBoundaryManager().getBoundary(\"Subtract.Domain."
                    + regionName[i] + "\");",
                    "        boundary" + regionName[i] + ".setBoundaryType(symmetryBoundaryObj);"
            };
        }
        code.insert(code.end(), codeBuffer.begin(), codeBuffer.end());
    }

    // Mesh sizing
    codeBuffer = {
            "",
            "        // MESH SIZING",
    };
    code.insert(code.end(), codeBuffer.begin(), codeBuffer.end());

    // Absolute mesh sizing
    for(int i = 0; i < regionName.size(); i++){
        codeBuffer = {
                "",
                "        // " + regionName[i],
                "        // Option surface size - ON",
                "        SurfaceSizeOption surfaceSizeOption" + regionName[i] + " = boundary" + regionName[i] +
                        ".get(MeshConditionManager.class).get(SurfaceSizeOption.class);",
                "        surfaceSizeOption" + regionName[i] + ".setSurfaceSizeOption(true);",
                "        // Surface size relative or absolute - ABSOLUTE",
                "        SurfaceSize surfaceSize" + regionName[i] + " = boundary" + regionName[i] +
                        ".get(MeshValueManager.class).get(SurfaceSize.class);",
                "        surfaceSize" + regionName[i] +
                        ".getRelativeOrAbsoluteOption().setSelected(RelativeOrAbsoluteOption.Type.ABSOLUTE);",
                "        // Set surface size - Minimum = Target",
                "        AbsoluteMinimumSize absoluteMinimumSize" + regionName[i] + " = surfaceSize" + regionName[i] +
                        ".getAbsoluteMinimumSize();",
                "        absoluteMinimumSize" + regionName[i] + ".getValue().setValue(valueMinimumTarget" +
                        regionName[i] + ");",
                "        AbsoluteTargetSize absoluteTargetSize" + regionName[i] + " = surfaceSize" + regionName[i] +
                ".getAbsoluteTargetSize();",
                "        absoluteTargetSize" + regionName[i] + ".getValue().setValue(valueMinimumTarget" +
                        regionName[i] + ");"
        };
        code.insert(code.end(), codeBuffer.begin(), codeBuffer.end());
    }
    code.emplace_back("    }");
    return code;
}